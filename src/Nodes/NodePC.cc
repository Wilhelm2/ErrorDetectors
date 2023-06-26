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

NodePC::NodePC() {
    // TODO Auto-generated constructor stub

}

NodePC::~NodePC() {
    // TODO Auto-generated destructor stub
}

void NodePC::processMessage(cMessage* msg)
{
    AppMsg* m = dynamic_cast<AppMsg*> (msg);
    simtime_t delay = m->getDelay();
    m->setDelay(0);
    if(delay.isZero())
    {
        RecvAppMsg(m);
        delete msg;
    }
    else
        scheduleAt(simTime()+ delay, msg);
}

bool NodePC::testDeliverMessage(messageInfo message)
{
    return clock.satisfiesDeliveryCondition(message.clock, getIndexIncrementedEntries(message.id.id));
}

void NodePC::iterativeDelivery()
{
    bool hasDeliveredMessage;
    do {
        hasDeliveredMessage=false;
        for(vector<messageInfo>::iterator it=pendingMsg.begin(); it!=pendingMsg.end(); /*fait dans la boucle à cause du erase*/)
        {
            if(testDeliverMessage(*it))
            {
                deliverMsg(*it);
                it=pendingMsg.erase(it);
                hasDeliveredMessage=true;
            }
            else
                it++;
        }

    } while(hasDeliveredMessage);
    return ;
}
