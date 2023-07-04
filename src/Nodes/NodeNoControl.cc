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

#include "NodeNoControl.h"

Define_Module(NodeNoControl);

NodeNoControl::NodeNoControl() {
    // TODO Auto-generated constructor stub

}

NodeNoControl::~NodeNoControl() {
    // TODO Auto-generated destructor stub
}

/** Processes received messages. Delivers application messages without any control.
 * @param msg Message to process*/
void NodeNoControl::processMessage(cMessage* msg)
{
    AppMsg* m = dynamic_cast<AppMsg*> (msg);
    simtime_t delay = m->getDelay();
    if(delay.isZero())
    {
        deliverMsg(s_messageInfo(m->getId(), m->getArrivalTime(), -1, {}, {}));
        delete m;
    }
    else
    {
        m->setDelay(0);
        scheduleAt(simTime() + delay, msg);
    }
}
