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

NodeWithControl::NodeWithControl() {}

NodeWithControl::~NodeWithControl() {}

/** Initializes the nodes Probabilistic clock and the tracker of causal information.*/
void NodeWithControl::initialize() {
    NodeBase::initialize();
    clock = ProbabilisticClock(params->clockLength);
    deliveredMessagesTracker = TotalDependencies(params->nbNodes);
}

/** Attaches the local Probabilistic clock to the message to broadcast.
 * @return Returns the message to broadcast.
 * */
AppMsg* NodeWithControl::createAppMsg() {
    AppMsg* m = NodeBase::createAppMsg();
    m->setPC(clock);
    return m;
}

/** Increments the local clock and keeps track of the message broadcast.
 * @return Returns the message to broadcast.*/
AppMsg* NodeWithControl::prepareBroadcast() {
    clock.incrementEntries(params->getEntriesIncrementedByProcess(id));
    AppMsg* m = NodeBase::prepareBroadcast();

    deliveredMessagesTracker[id]++;
    delivered.push_back(s_messageInfo(m->getId(), m->getArrivalTime(), m->getHash(), m->getDependencies(), m->getPC()));
    removeOldMessages();
    return m;
}

/** Removes information about obsolete broadcasted messages based on duration since their broadcast.*/
void NodeWithControl::removeOldMessages() {
    vector<messageInfo>::iterator it = delivered.begin();
    while (it != delivered.end() && it->recvtime < (simTime() - SimTime(2, SIMTIME_S)))
        it++;
    delivered.erase(delivered.begin(), it);
}

/** Delivers an application message.
 * @param message The message to deliver.
 * @return true if the message has been delivered in causal order and false otherwise.*/
bool NodeWithControl::deliverMsg(const messageInfo& message) {
    bool causallyDelivered = NodeBase::deliverMsg(message);
    delivered.push_back(message);
    clock.incrementEntries(params->getEntriesIncrementedByProcess(message.id.id));
    deliveredMessagesTracker[message.id.id]++;
    return causallyDelivered;
}

/** Handles application message the node receives.
 * @param m The message the process received*/
void NodeWithControl::RecvAppMsg(AppMsg* m) {
    messageInfo msg(m->getId(), simTime(), m->getHash(), m->getDependencies(), m->getPC());
    if (testDeliverMessage(msg))
        iterativeDelivery();
    else
        pendingMsg.push_back(msg);
}

/** Getter of the Probabilistic clock indexes node idSource increments when broadcasting a message.
 * @param idSource Identificator of the node.
 * @return Probabilistic clock indexes*/
const vector<unsigned int>& NodeWithControl::getIndexIncrementedEntries(unsigned int idSource) {
    return params->entriesIncrementedByProcess[idSource % params->entriesIncrementedByProcess.size()];
}
