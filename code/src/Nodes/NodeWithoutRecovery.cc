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

#include "NodeWithoutRecovery.h"

Define_Module(NodeWithoutRecovery);

NodeWithoutRecovery::NodeWithoutRecovery() {}

NodeWithoutRecovery::~NodeWithoutRecovery() {}

void NodeWithoutRecovery::processMessage(cMessage* msg) {
    AppMsg* m = dynamic_cast<AppMsg*>(msg);
    simtime_t delay = m->getDelay();
    m->setDelay(0);
    if (delay.isZero()) {
        RecvAppMsg(m);
        delete msg;
    } else
        scheduleAt(simTime() + delay, msg);
}

void NodeWithoutRecovery::iterativeDelivery() {
    bool hasDeliveredMessage;
    do {
        hasDeliveredMessage = false;
        for (vector<messageInfo>::iterator it = pendingMsg.begin(); it != pendingMsg.end();
             /*fait dans la boucle à cause du erase*/) {
            if (testDeliverMessage(*it)) {
                it = pendingMsg.erase(it);
                hasDeliveredMessage = true;
            } else
                it++;
        }

    } while (hasDeliveredMessage);
    return;
}

/** Tries to deliver the message and increments statistics to keep track of true and false error detector positives.
 * @param message The information about the message to deliver.
 * @return true if the process delivers the message and false otherwise. */
bool NodeWithoutRecovery::testDeliverMessage(const messageInfo& message) {
    if (clock.satisfiesDeliveryCondition(message.clock, params->getEntriesIncrementedByProcess(message.id.id))) {
        if (!control->canCausallyDeliverMessage(message.id, id)) {
            if (!detector->test(message, getIndexIncrementedEntries(message.id.id), clock, deliveredMessagesTracker,
                                control, params, delivered))
                detector->stats.trueNegative++;
        } else {
            if (!detector->test(message, getIndexIncrementedEntries(message.id.id), clock, deliveredMessagesTracker,
                                control, params, delivered))
                detector->stats.falseNegative++;
        }
        deliverMsg(message);
        return true;
    }
    return false;
}
