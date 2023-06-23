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

NodeWithoutRecovery::NodeWithoutRecovery() {
    // TODO Auto-generated constructor stub

}

NodeWithoutRecovery::~NodeWithoutRecovery() {
    // TODO Auto-generated destructor stub
}


void NodeWithoutRecovery::processMessage(cMessage* msg)
{
    simtime_t delay;
    if(AppMsg* m = dynamic_cast<AppMsg*> (msg))
    {
        delay = m->getDelay();
        m->setDelay(0);
        if(delay.isZero())
            RecvAppMsg(m);
    }
    if(!delay.isZero())
        scheduleAt(simTime()+ delay, msg);
    else
        delete msg;
}


void NodeWithoutRecovery::iterativeDelivery()
{
    bool hasDeliveredMessage;
    do {
        hasDeliveredMessage=false;
        for(vector<dep>::iterator it=pendingMsg.begin(); it!=pendingMsg.end(); /*fait dans la boucle à cause du erase*/)
        {
            if(testDeliverMessage(*it))
            {
                it=pendingMsg.erase(it);
                hasDeliveredMessage=true;
            }
            else
                it++;
        }

    } while(hasDeliveredMessage);
    return ;
}

bool NodeWithoutRecovery::testDeliverMessage(dep message)
{
    if(clock.satisfiesDeliveryCondition(message.clock, params->getEntriesIncrementedByProcess(message.id.id)))
    {
        if(!control->canCausallyDeliverMessage(message.id, id))
        {
            if(SimulationParameters::DeliveryController==SimulationParameters::Delivery::Mostefaoui && !MostefaouiErrorDetector::test(message.id.id, message.clock,params->entriesIncrementedByProcess[message.id.id%params->nbCombinations], clock))
                detector.stats.trueNegative++;
            else if(SimulationParameters::DeliveryController==SimulationParameters::Delivery::Hash && !detector.hashTest(message, delivered, params->entriesIncrementedByProcess[message.id.id%params->nbCombinations], deliveredMessagesTracker, control, params))
                detector.stats.trueNegative++;
        }
        else if(SimulationParameters::DeliveryController==SimulationParameters::Delivery::Hash && !detector.hashTest(message, delivered, params->entriesIncrementedByProcess[message.id.id%params->nbCombinations], deliveredMessagesTracker, control, params))
            detector.stats.falseNegative++;
        else if(SimulationParameters::DeliveryController==SimulationParameters::Delivery::Mostefaoui && !MostefaouiErrorDetector::test(message.id.id, message.clock,params->entriesIncrementedByProcess[message.id.id%params->nbCombinations], clock))
            detector.stats.falseNegative++;
        deliverMsg(message);
        return true;
    }
    return false;
}
