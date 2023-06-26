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

#include "NodeWithControl.h"

NodeWithControl::NodeWithControl() {
    // TODO Auto-generated constructor stub

}

NodeWithControl::~NodeWithControl() {
    // TODO Auto-generated destructor stub
}

void NodeWithControl::initialize()
{
    NodeBase::initialize();

    clock = ProbabilisticClock(params->clockLength);
    deliveredMessagesTracker = TotalDependencies(params->nbNodes);
}

AppMsg* NodeWithControl::createAppMsg()
{
    AppMsg* m = NodeBase::createAppMsg();
    m->setPC(clock);
    cout<<"added clock to message "<<endl;
    return m;
}

AppMsg* NodeWithControl::prepareBroadcast()
{
    clock.incrementEntries(params->getEntriesIncrementedByProcess(id));
    AppMsg* m = NodeBase::prepareBroadcast();

    deliveredMessagesTracker[id]++;
    delivered.push_back(s_messageInfo(m->getId(), m->getArrivalTime(), m->getHash(), m->getDependencies(), m->getPC()));
    removeOldMessages();
    return m;
}

void NodeWithControl::removeOldMessages()
{
    vector<messageInfo>::iterator it = delivered.begin();
    while(it != delivered.end() && it->recvtime < (simTime()-SimTime(2, SIMTIME_S))
            )
        it++;
    delivered.erase(delivered.begin(),it);
}

bool NodeWithControl::deliverMsg(messageInfo message)
{
    bool causallyDelivered = NodeBase::deliverMsg(message);

    delivered.push_back(message);
    clock.incrementEntries(params->getEntriesIncrementedByProcess(message.id.id));
    deliveredMessagesTracker[message.id.id]++;
    return causallyDelivered;
}

void NodeWithControl::RecvAppMsg(AppMsg*m)
{
    messageInfo msg (m->getId(), simTime(), m->getHash(), m->getDependencies(), m->getPC());
    if(testDeliverMessage(msg))
    {
        deliverMsg(msg);
        iterativeDelivery();
    }
    else
        pendingMsg.push_back(msg);
}

const vector<unsigned int>& NodeWithControl::getIndexIncrementedEntries(unsigned int idSource)
{
    return params->entriesIncrementedByProcess[idSource%params->nbCombinations];
}
