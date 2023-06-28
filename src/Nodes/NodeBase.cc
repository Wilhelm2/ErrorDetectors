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

#include "NodeBase.h"

std::normal_distribution<double> NodeBase::sendDistribution = std::normal_distribution<double>(0.,10000.);

NodeBase::NodeBase() {
    // TODO Auto-generated constructor stub

}

NodeBase::~NodeBase() {
    // TODO Auto-generated destructor stub
}

void NodeBase::initialize()
{
    cSimpleModule::initialize();
    params =  dynamic_cast<SimulationParameters*> (getModuleByPath((string(PACKAGENAME)+".simParams").c_str()));
    scheduleAt(0, broadcastTimer);
    control = dynamic_cast<Controller*> (getModuleByPath((string(PACKAGENAME)+".control").c_str()));

    initializeCommunications();
}

void NodeBase::initializeCommunications()
{
    Init* m = new Init();
    m->setProcessId(id);
    cModule::GateIterator i(this);
    while( !i.end() && (*i)->getType()==cGate::INPUT)
        i++;
    outGate = (*i);
    send(m, outGate);
}

void NodeBase::handleMessage(cMessage *msg)
{
    if (msg == broadcastTimer) {
        if(simTime()==0)
            baseTimeBroadcast = params->determineFirstSendTimeInMs(id);
        else
        {
            send(prepareBroadcast(), outGate);
            baseTimeBroadcast += SimTime(params->delaySend*1000, SIMTIME_MS) ; // SIMTIME_MS because if done in SIMTIME_S then will truncate even if delaySend is not a round number
        }
        simtime_t nextBroadcastTime  = baseTimeBroadcast+SimTime(sendDistribution(generatorSendDistribution), SIMTIME_US) ;
        if(nextBroadcastTime  < simTime())
            nextBroadcastTime = baseTimeBroadcast;
        scheduleAt(nextBroadcastTime , msg);
    }
    else
        processMessage(msg);
}

AppMsg* NodeBase::prepareBroadcast()
{
    AppMsg* m = createAppMsg();
    control->notifySendMessage(id, seq);
    control->notifyDeliverMessage({id, seq}, id);
    std::cerr<<simTime()<< " BROADCASTMSG " << id << "," << seq <<endl;
    return m;
}

AppMsg* NodeBase::createAppMsg()
{
    AppMsg* m = new AppMsg("broadcast");
    seq++;
    m->setId({id,seq});
    return m;
}

bool NodeBase::deliverMsg(const messageInfo& message)
{
//        cout<<"Node " << id << " delivers " << message.id.id << ","<<message.id.seq<<endl;
    stats.nbDeliveredMessages++;
    if(!control->canCausallyDeliverMessage(message.id, id))
    {
        stats.nbFalseDeliveredMessages++;
        cerr<<"Node " << id << " delivered message " << message.id.id << "," << message.id.seq << " out of causal order" <<endl;
    }

    return control->notifyDeliverMessage(message.id, id) ;
}
