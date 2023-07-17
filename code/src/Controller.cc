////
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

#include "Controller.h"

Controller::Controller() {}

Controller::~Controller() {}

/** Initializes the controller
 * */
void Controller::initialize() {
    cSimpleModule::initialize();
    params = dynamic_cast<SimulationParameters*>(getModuleByPath((string(PACKAGENAME) + ".simParams").c_str()));
    nodeDependencies.resize(params->nbNodes);
    for (TotalDependencies& it : nodeDependencies)
        it = TotalDependencies(params->nbNodes);
    nodeBroadcastedMessages.resize(params->nbNodes);
}

/** Called by nodees when broadcasting a message to save the broadcasted message at the controller.
 * @param idSender Identificator of the node that broadcasted the message.
 * @param seq Sequence number idSender associated to the message.
 */
void Controller::notifySendMessage(unsigned int idSender, unsigned int seq) {
    msg t;
    t.seq = seq;
    nodeDependencies[idSender][idSender]++;
    t.dependencies = nodeDependencies[idSender];
    t.psHasDelivered.assign(params->nbNodes, false);
    t.sendTime = simTime();
    t.deliveredAtTime.assign(params->nbNodes, 0);
    nodeBroadcastedMessages[idSender].push_back(t);
    //    std::cout << "BROADCASTCONTROLLER: MSG ADDED from " << idSrc <<" seq " << t->seq << " || ";
    //    printclock(t->clock);
}

/** Searches the structure associated to a broadcasted message.
 * Raises an error if the message is not registered.
 * @param idM the message's unique identificator.
 * @return Returns an iterator pointing to the message's structure.
 * */
vector<msg>::iterator Controller::searchMessage(idMsg idM) {
    if (nodeBroadcastedMessages.size() < idM.id)
        throw "Source node not contained in nodeBroadcastedMessages";

    for (vector<msg>::iterator it = nodeBroadcastedMessages[idM.id].begin();
         it != nodeBroadcastedMessages[idM.id].end(); ++it) {
        if (it->seq == idM.seq)
            return it;
    }
    throw "Message not contained in vector of sent messages of node";
}

/** Called by a node when delivering a message to notify the controller of the delivery, which verifies that the node
 * delivers the message only once and in causal order. The function exists if the node has already delivered the
 * message.
 * @param idM the message's identificator.
 * @param idDest the id of the node which delivers the message.
 * @return boolean set to true if the node delivered the message in cuasal order and false otherwise.
 */
bool Controller::notifyDeliverMessage(idMsg idM, unsigned int idDest) {
    vector<msg>::iterator m = searchMessage(idM);
    bool deliverInCausalOrder = canCausallyDeliverMessage(idM, idDest);

    //    if(!deliverInCausalOrder)
    //        printDeliveryError("Out of causal order delivery", idM, idDest, m.dependencies, nodeDependencies[idDest]);
    nodeDependencies[idDest][idM.id] =
        max(m->dependencies[idM.id],
            nodeDependencies[idDest][idM.id]);  // increments local clock of idDest MUST BE MAX TO HANDLE THE CASE WHERE
                                                // DELIVERED A MESSAGE OUT OF CAUSAL ORDER
    if (m->psHasDelivered[idDest]) {
        printDeliveryError("Multiple delivery", idM, idDest, m->dependencies, nodeDependencies[idDest]);
        exit(0);
    }

    m->deliveredAtNbPs++;
    m->psHasDelivered[idDest] = true;
    m->deliveredAtTime[idDest] = simTime();
    if (m->deliveredAtNbPs == params->nbNodes)  // delete le message car reçu par tous les mobiles ET stations
        deleteMessage(idM);
    return deliverInCausalOrder;
}

/** Called to print delivery errors.
 * @param errorReason The delivery error.
 * @param idM The identificator of the wrongly delivered message.
 * @param destnode The identificator of the process which delivered the message.
 * @param messageDependencies The message's causal dependencies.
 * @param nodeDependencies Tracks the messages delivered by the process.
 */
void Controller::printDeliveryError(string errorReason,
                                    idMsg idM,
                                    unsigned int destnode,
                                    const TotalDependencies& messageDependencies,
                                    const TotalDependencies& nodeDependencies) {
    cerr << "BroadcastController " << errorReason << " idMsg " << idM.id << "," << idM.seq << " destination "
         << destnode << endl;
    cerr << "node clock: ";
    nodeDependencies.printErr();
    cerr << "Message clock: ";
    messageDependencies.printErr();
}

/** Verifies that a proces can causally deliver a message.
 * @param idM The message to deliver.
 * @param idDest The process that will deliver the message.
 * @return True if the process can deliver the message in causal order and false otherwise.
 */
bool Controller::canCausallyDeliverMessage(idMsg idM, unsigned int idDest) {
    vector<msg>::iterator m = searchMessage(idM);
    return (nodeDependencies[idDest].SatisfiesDeliveryConditions(m->dependencies, idM.id));
}

/** Called to remove messages delivered by all processes.
 * Raises an error if the message is not registered.
 * @param idM The message's identificator.
 */
void Controller::deleteMessage(idMsg idM) {
    //    std::cerr<< "BROADCASTCONTROLLER: DELETE MESSAGE (" << idM.id <<"," <<idM.seq<<")"<<endl;
    vector<msg>::iterator it;
    for (it = nodeBroadcastedMessages[idM.id].begin(); it != nodeBroadcastedMessages[idM.id].end(); ++it)
        if (it->seq == idM.seq)
            break;
    if (it == nodeBroadcastedMessages[idM.id].end())
        throw "Message not found";
    nodeBroadcastedMessages[idM.id].erase(it);
    return;
}

/** Checks if a message is a causal dependency of another message
 * @param idM Message of which wants to check the dependency.
 * @param idDep Dependency to be checked.
 * @return true if idDep is a dependency of idM and false otherwise.
 */
bool Controller::isDependency(idMsg idM, idMsg idDep) {
    vector<msg>::iterator m = searchMessage(idM);
    return m->dependencies[idDep.id] >= idDep.seq;
}
