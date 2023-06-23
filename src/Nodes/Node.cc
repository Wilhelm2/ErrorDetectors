//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "Node.h"

std::normal_distribution<double> Node::sendDistribution = std::normal_distribution<double>(0.,10000.);

void Node::initialize()
{
    cSimpleModule::initialize();
    params =  dynamic_cast<SimulationParameters*> (getModuleByPath((string(PACKAGENAME)+".simParams").c_str()));
    scheduleAt(0, broadcastTimer);
    clock = ProbabilisticClock(params->clockLength);
    deliveredMessagesTracker = TotalDependencies(params->nbNodes);
    control = dynamic_cast<Controller*> (getModuleByPath((string(PACKAGENAME)+".control").c_str()));
    detector.setClockDifferenceConsideredDependency(params->nbNodes/params->delaySend, params->entriesIncrementedByProcess[0].size());

    Init* m = new Init();
    m->setProcessId(id);
    cModule::GateIterator i(this);
    while( !i.end() && (*i)->getType()==cGate::INPUT)
        i++;
    outGate = (*i);
    send(m, outGate);
}

void Node::handleMessage(cMessage *msg)
{
    if (msg == broadcastTimer) {
        if(simTime()==0)
        {
            baseTimeBroadcast = params->determineFirstSendTimeInMs(id);
            simtime_t nextBroadcastTime  = baseTimeBroadcast+SimTime(sendDistribution(generatorSendDistribution), SIMTIME_US) ;
            if(nextBroadcastTime  < simTime())
                nextBroadcastTime = baseTimeBroadcast;
            scheduleAt(nextBroadcastTime , broadcastTimer);
        }
        else
        {
            sendAppMsg();
            baseTimeBroadcast += SimTime(params->delaySend*1000, SIMTIME_MS) ; // SIMTIME_MS because if done in SIMTIME_S then will truncate even if delaySend is not a round number
            simtime_t nextBroadcastTime = baseTimeBroadcast+SimTime(sendDistribution(generatorSendDistribution), SIMTIME_US);
            if(nextBroadcastTime  < simTime())
                nextBroadcastTime = simTime();
            scheduleAt(nextBroadcastTime, msg);
        }
    }
    else
        processMessage(msg);
}

void Node::RecvAppMsg(AppMsg*m)
{
    dep msg = createDep(m->getId(), simTime(), m->getHash(), m->getDependencies(), m->getPC());
    if(SimulationParameters::DeliveryController == SimulationParameters::Delivery::NOTHING)
        deliverMsg(msg);
    else
    {
        if(testDeliverMessage(msg))
            iterativeDelivery();
        else
            pendingMsg.push_back(msg);
    }
}

bool Node::deliverMsg(dep message)
{
//        cout<<"Node " << id << " delivers " << message.id.id << ","<<message.id.seq<<endl;
    stats.nbDeliveredMessages++;
    if(!control->canCausallyDeliverMessage(message.id, id))
    {
        stats.nbFalseDeliveredMessages++;
        cerr<<"Node " << id << " delivered message " << message.id.id << "," << message.id.seq << " out of causal order" <<endl;
    }
    delivered.push_back(message);
    clock.incrementEntries(params->getEntriesIncrementedByProcess(message.id.id));
    deliveredMessagesTracker[message.id.id]++;
    return control->notifyDeliverMessage(message.id, id) ;
}

void Node::sendAppMsg()
{
    clock.incrementEntries(params->getEntriesIncrementedByProcess(id));
    AppMsg *m = createAppMsg();
    control->notifySendMessage(id, seq);
    control->notifyDeliverMessage({id, seq}, id);
    deliveredMessagesTracker[id]++;
    delivered.push_back(createDep(m->getId(), simTime(), m->getHash(), m->getDependencies(), m->getPC()));
    send(m,outGate);
    clearDelivered();
    return;
}

AppMsg* Node::createAppMsg()
{
    AppMsg *m = new AppMsg("broadcast");
    seq++;
    if(SimulationParameters::depAppended == SimulationParameters::Dependencies::Total)
        m->setHash(detector.hashTotalDependencies(deliveredMessagesTracker));
    else
        m->setHash(detector.hashPartialDependencies(detector.getPartialDependencies(delivered,clock)));

    m->setId({id,seq});
    m->setPC(clock);

    if(APPENDDEPENDENCIES)
    {
        m->setDependencies(detector.determineAndGetAppendedDependencies(delivered, m->getPC()));
        stats.nbSentDependencies += m->getDependencies().size();
        stats.controlDataSize += clock.size() * sizeof(unsigned int) + m->getDependencies().size()*2* sizeof(unsigned int);
    }

    std::cerr<<simTime()<< " BROADCASTMSG "<<id<<","<<seq <<" number dependencies " << m->getDependencies().size()<<endl;
    return m;
}

void Node::clearDelivered()
{
    vector<dep>::iterator it = delivered.begin();
    while(it != delivered.end() && it->recvtime < simTime()-SimTime(2, SIMTIME_S))
        it++;
    delivered.erase(delivered.begin(),it);
}

dep Node::createDep(idMsg id, simtime_t recvtime, size_t hash, vector<idMsg> dependencies, ProbabilisticClock clock)
{
    dep m;
    m.id = id;
    m.recvtime = recvtime;
    m.hash = hash;
    m.dependencies = dependencies;
    m.clock = clock;
    return m;
}

