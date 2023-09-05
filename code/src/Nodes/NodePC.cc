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

#include "NodePC.h"

Define_Module(NodePC);

NodePC::NodePC() {}

NodePC::~NodePC() {}

void NodePC::processMessage(cMessage* msg) {
    AppMsg* m = dynamic_cast<AppMsg*>(msg);
    simtime_t delay = m->getDelay();
    m->setDelay(0);
    if (delay.isZero()) {
        RecvAppMsg(m);
        delete msg;
    } else
        scheduleAt(simTime() + delay, msg);
}

/** Delivers a message if its clock satisfies the delivery conditions.
 * @param message Information about the message to deliver
 * @return true if the node delivered the message and false otherwise.
 */
bool NodePC::testDeliverMessage(const messageInfo& message) {
    if (clock.satisfiesDeliveryCondition(message.clock, getIndexIncrementedEntries(message.id.id))) {
        deliverMsg(message);
        return true;
    } else
        return false;
}

/** Tries to deliver pending messages.
 */
void NodePC::iterativeDelivery() {
    bool hasDeliveredMessage;
    do {
        hasDeliveredMessage = false;
        for (vector<messageInfo>::iterator it = pendingMsg.begin(); it != pendingMsg.end();
             /*done in loop because of erase*/) {
            if (testDeliverMessage(*it)) {
                it = pendingMsg.erase(it);
                hasDeliveredMessage = true;
            } else
                it++;
        }

    } while (hasDeliveredMessage);
    return;
}
