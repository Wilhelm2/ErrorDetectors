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

#include "communicationDispatcher.h"

Define_Module(CommunicationDispatcher);

/** @brief Initializes the communicationDispatcher
 */
void CommunicationDispatcher::initialize() {
    cSimpleModule::initialize();
    params = dynamic_cast<SimulationParameters*>(getModuleByPath((string(PACKAGENAME) + ".simParams").c_str()));
    gates.resize(params->nbNodes);

    for (unsigned int i = 0; i < params->nbNodes; i++)
        channelRandNumber.push_back(rand() % 2);
}

/** @brief handles messages to forward
 * @param msg Message to forward
 */
void CommunicationDispatcher::handleMessage(cMessage* msg) {
    if (Init* m = dynamic_cast<Init*>(msg)) {
        std::string output =
            std::regex_replace(m->getArrivalGate()->getFullName(), std::regex("[^0-9]*([0-9]+).*"), std::string("$1"));
        gates[m->getNodeId()] = this->gate("neighbour$o", stoi(output));
        delete msg;
    } else if (AppMsg* m = dynamic_cast<AppMsg*>(msg)) {
        broadcastAppMessage(m);
        delete msg;
    } else if (DepReq* m = dynamic_cast<DepReq*>(msg)) {
        m->setDelay(computeDelay(m->getIdRequester(), m->getIdMsgToRecover().id));
        send(m, gates[m->getIdMsgToRecover().id]);
    } else if (DepRsp* m = dynamic_cast<DepRsp*>(msg)) {
        m->setDelay(computeDelay(m->getIdRequster(), m->getIdMsgToRecover().id));
        send(m, gates[m->getIdRequster()]);
    } else {
        cerr << "Error message not found" << msg << endl;
        exit(1);
    }
}

/** @brief Broadcasts an application message to all nodes.
 * @param m Message to broadcast
 */
void CommunicationDispatcher::broadcastAppMessage(AppMsg* m) {
    for (unsigned int i = 0; i < params->nbNodes; i++) {
        if (i == m->getId().id)  // does not send message back to sender
            continue;
        m->setDelay(computeDelay(m->getId().id, i));
        incrementDelayIntervals((int)(m->getDelay().dbl() / 10000));
        AppMsg* copy = m->dup();
        send(copy, gates[i]);
    }
}

/** @brief Computes the propagation delay of a message following the node id of the source and target processes
 * @param idSourceProcess node id of the process that sent the message
 * @param idTargetProcess node id of the destination process of the message
 * @return message propagation delay
 */
simtime_t CommunicationDispatcher::computeDelay(unsigned int idSourceProcess, unsigned int idTargetProcess) {
    int delayInUs;
    if ((channelRandNumber[idSourceProcess] + channelRandNumber[idTargetProcess]) % 2 == 0)
        delayInUs = distributionChannelDelayPair(generatorChannelDelay);
    else
        delayInUs = distributionChannelDelayImpair(generatorChannelDelay);
    if (delayInUs < 0)
        delayInUs = 0;
    return SimTime(delayInUs, SIMTIME_US);
}

/** @brief Takes statistics of the propagation delay interval
 * @param entry propagation delay interval of message
 */
void CommunicationDispatcher::incrementDelayIntervals(unsigned int entry) {
    if (entry < (sizeof(stats.delayIntervals) / sizeof(int)))
        stats.delayIntervals[entry]++;
    else {
        cerr << "INCREMENTS delayIntervals OUT OF BOUND :" << entry
             << " bound: " << sizeof(stats.delayIntervals) / sizeof(int) << endl;
        throw "INCREMENTS delayIntervals OUT OF BOUND";
    }
}
