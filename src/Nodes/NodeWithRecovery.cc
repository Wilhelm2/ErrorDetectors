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

#include "NodeWithRecovery.h"

Define_Module(NodeWithRecovery);

NodeWithRecovery::NodeWithRecovery() {
    // TODO Auto-generated constructor stub

}

NodeWithRecovery::~NodeWithRecovery() {
    // TODO Auto-generated destructor stub
}

void NodeWithRecovery::handleMessage(cMessage *msg)
{
    if(msg == broadcastTimer && inRecovery)
    {
        delayedAppMgsBroadcast = true;
        statsRecovery.jumpedBroadcasts++;
        baseTimeBroadcast += SimTime(params->delaySend*1000, SIMTIME_MS) ; // SIMTIME_MS because if done in SIMTIME_S then will truncate even if delaySend is not a round number
        simtime_t nextBroadcastTime  = baseTimeBroadcast+SimTime(sendDistribution(generatorSendDistribution), SIMTIME_US) ;
        if(nextBroadcastTime  < simTime())
            nextBroadcastTime = baseTimeBroadcast;
        scheduleAt(nextBroadcastTime , msg);
    }
    else
        NodeWithControl::handleMessage(msg);
}

void NodeWithRecovery::processMessage(cMessage* msg)
{
    simtime_t delay;
    if(AppMsg* m = dynamic_cast<AppMsg*> (msg))
    {
        delay = m->getDelay();
        m->setDelay(0);
        if(delay.isZero())
            RecvAppMsg(m);
    }
    else if(DepReq* m = dynamic_cast<DepReq*> (msg))
    {
        delay = m->getDelay();
        m->setDelay(0);
        if(delay.isZero())
            processDepReq(m);
    }
    else if(DepRsp* m = dynamic_cast<DepRsp*> (msg))
    {
        delay = m->getDelay();
        m->setDelay(0);
        if(delay.isZero())
            processRcvRspDep(m);
    }

    if(!delay.isZero())
        scheduleAt(simTime()+ delay, msg);
    else
        delete msg;
}

void NodeWithRecovery::processDepReq(DepReq* m)
{
//    std::cerr<< simTime()<<"Node " << id << " receives depreq from Node "<<m->getIdRequester() << " which requests the dependencies of msg: " << m->getIdMsgToRecover().id<<","<<m->getIdMsgToRecover().seq<<endl;
    DepRsp* msg = new DepRsp();
    msg->setIdMsgToRecover(m->getIdMsgToRecover());
    for(const msgDependency& msgDep: localSentMsgDependencies)
    {
        if(msgDep.seqMsg == m->getIdMsgToRecover().seq)
        {
            msg->setDependencies(msgDep.dependencies);
            msg->setIdRequster(m->getIdRequester());
            send(msg,outGate);
            return;
        }
    }
    cerr<<"Message not found"<<endl;
    exit(1);
}

void NodeWithRecovery::processRcvRspDep(DepRsp* m)
{
//    cerr<<"node "<< id << " got reply for message " << m->getIdMsgToRecover().id <<"," << m->getIdMsgToRecover().seq<<endl;
    pushbackMessagesInRecovery();
    inRecovery = false;
    if(deliveredMessagesTracker.SatisfiesDeliveryConditions(m->getDependencies(),m->getIdMsgToRecover().id))
    {
//        cerr<< "Node "<<id<<"delivers message " << currRecovery.id.id << "," << currRecovery.id.seq<<endl;
        deliverMsg(currRecovery);
    }
    else
        recoveredMessages.push_back({currRecovery,m->getDependencies()});
    iterativeDelivery(); // checks for messages that can now be delivered or if there is another recovery to start
    if(delayedAppMgsBroadcast && !inRecovery)
    {
//        sendAppMsg();
        delayedAppMgsBroadcast = false;
    }
}

void NodeWithRecovery::pushbackMessagesInRecovery()
{
    for(const messageInfo& msgR : messagesToRecover)
    {
        vector<messageInfo>::iterator it = pendingMsg.begin();
        while(it != pendingMsg.end() && it->recvtime > msgR.recvtime)
            it++;
        pendingMsg.insert(it,msgR);
    }
    messagesToRecover.clear();
    return;
}

void NodeWithRecovery::iterativeDelivery()
{
    bool messageDelivered;
    do
    {
        messageDelivered=false;
        if(tryDeliverMessageInRecovery())
        {
            messageDelivered = true;
            continue; // to skip pending messages and directly pass again to see if recovered messages can be delivered
        }
        if(tryDeliverPendingMessages())
            messageDelivered = true;
    } while(messageDelivered);
    return ;
}

bool NodeWithRecovery::tryDeliverMessageInRecovery()
{
    vector<recoveredMessage>::iterator it;
    for(it=recoveredMessages.begin() ; it!= recoveredMessages.end() ; it++ )
    {
        if(deliveredMessagesTracker.SatisfiesDeliveryConditions(it->dependencies,it->msg.id.id))
        {
            deliverMsg(it->msg);
            recoveredMessages.erase(it);
            return true;
        }
    }
    return false;
}

bool NodeWithRecovery::tryDeliverPendingMessages()
{
    for(vector<messageInfo>::iterator it=pendingMsg.begin(); it!=pendingMsg.end(); it++)
    {
        if(testDeliverMessage(*it))
        {
            pendingMsg.erase(it);
            return true;
        }
    }
    return false;
}

bool NodeWithRecovery::testDeliverMessage(const messageInfo& m)
{
    if(clock.satisfiesDeliveryCondition(m.clock, params->getEntriesIncrementedByProcess(m.id.id)))
    {
        if(detector->test(m, getIndexIncrementedEntries(m.id.id), clock, deliveredMessagesTracker, control, params, delivered))
            deliverMsg(m);
        else
        {
            if(control->canCausallyDeliverMessage(m.id, id))
                detector->stats.falseNegative++;
            else
                detector->stats.trueNegative++;

            if(inRecovery)
                messagesToRecover.push_back(m);
            else
            {
                requestDependencies(m);
//                cerr<<"node " << id << " can causally deliver message " << m.id.id << "," << m.id.seq<< " "<< control->canCausallyDeliverMessage(m.id, id)<<endl;
            }
        }
        return true;
    }
    return false;
}

void NodeWithRecovery::requestDependencies(const messageInfo& m)
{
    statsRecovery.nbRecoveries++;
    inRecovery = true;
    currRecovery = m;
    DepReq* req= new DepReq();
    req->setIdMsgToRecover(m.id);
    req->setIdRequester(id);
    send(req,outGate);
}

AppMsg* NodeWithRecovery::prepareBroadcast()
{
    AppMsg* m = NodeDetector::prepareBroadcast();
    localSentMsgDependencies.push_back({seq,deliveredMessagesTracker});
    return m;
}
