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

std::normal_distribution<double> NodeBase::sendDistribution = std::normal_distribution<double>(0., 10000.);
unsigned int NodeBase::idCounter = 0;

NodeBase::NodeBase() {}

NodeBase::~NodeBase() {}

/** Initializes the node by scheduling it's broadcastTimer, initializing its connection with the communication
 * dispatcher and looking up the SimulationParameters and Controller modules.
 */
void NodeBase::initialize() {
    cSimpleModule::initialize();
    params = dynamic_cast<SimulationParameters*>(getModuleByPath((string(PACKAGENAME) + ".simParams").c_str()));
    scheduleAt(0, broadcastTimer);
    control = dynamic_cast<Controller*>(getModuleByPath((string(PACKAGENAME) + ".control").c_str()));

    initializeCommunications();
}

/** Initializes the connection with the communicationDispatcher by sending an Init message containing the node's id,
 * such that the communicationDispatcher can associate the gates to the right nodes.
 */
void NodeBase::initializeCommunications() {
    Init* m = new Init();
    m->setNodeId(id);
    cModule::GateIterator i(this);
    while (!i.end() && (*i)->getType() == cGate::INPUT)
        i++;
    outGate = (*i);
    send(m, outGate);
}

/** Called upon reception of a message to proecss it, whether it be a selfmsg (broadcastTimer) or a message received
   from another process.
     @param msg The message the node received.
 */
void NodeBase::handleMessage(cMessage* msg) {
    if (msg == broadcastTimer)
        handleBroadcastTimer(msg);
    else
        processMessage(msg);
}

/** Called to handle the broadcastTimer. Determines the first broadcast time at the beginning of the simulation, and
 * broadcasts messages and schedules next broadcasts otherwise.
 * @param msg Timer used to schedule the next broadcast of application messages.
 */
void NodeBase::handleBroadcastTimer(cMessage* msg) {
    if (simTime() == 0)
        baseTimeBroadcast = params->determineFirstSendTimeInMs(id);
    else {
        send(prepareBroadcast(), outGate);
        baseTimeBroadcast +=
            SimTime(params->delaySend * 1000, SIMTIME_MS);  // SIMTIME_MS because if done in SIMTIME_S then will
                                                            // truncate even if delaySend is not a round number
    }
    simtime_t nextBroadcastTime = baseTimeBroadcast + SimTime(sendDistribution(generatorSendDistribution), SIMTIME_US);
    if (nextBroadcastTime < simTime())
        nextBroadcastTime = baseTimeBroadcast;
    scheduleAt(nextBroadcastTime, msg);
}

/** Prepares the broadcast of an application message by creating it and notifying the controller of the broadcast.
    \return The application message to broadcast.
 */
AppMsg* NodeBase::prepareBroadcast() {
    AppMsg* m = createAppMsg();
    control->notifySendMessage(id, seq);
    control->notifyDeliverMessage({id, seq}, id);
    std::cerr << simTime() << " BROADCASTMSG " << id << "," << seq << endl;
    return m;
}

/** Creates an application message and initializes it.
    @return The application message.
 */
AppMsg* NodeBase::createAppMsg() {
    AppMsg* m = new AppMsg("broadcast");
    seq++;
    m->setId({id, seq});
    return m;
}

/** Delivers a message and verifies its delivery in causal order.
 * @param message The information about the message to deliver.
 * @return true if the message has been delivered in causal order and false otherwise.
 */
bool NodeBase::deliverMsg(const messageInfo& message) {
    //        cout<<"Node " << id << " delivers " << message.id.id << ","<<message.id.seq<<endl;
    stats.nbDeliveredMessages++;
    if (!control->canCausallyDeliverMessage(message.id, id)) {
        stats.nbFalseDeliveredMessages++;
        cerr << "Node " << id << " delivered message " << message.id.id << "," << message.id.seq
             << " out of causal order" << endl;
    }

    return control->notifyDeliverMessage(message.id, id);
}
