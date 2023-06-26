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

#include "NodeDetector.h"

NodeDetector::NodeDetector() {
    // TODO Auto-generated constructor stub

}

NodeDetector::~NodeDetector() {
    // TODO Auto-generated destructor stub
}

void NodeDetector::initialize()
{
    NodeWithControl::initialize();
    if(params->DeliveryController == SimulationParameters::Delivery::Mostefaoui)
        detector = new MostefaouiErrorDetector();
    else if(params->DeliveryController == SimulationParameters::Delivery::HashClockDifference)
        detector = new ErrorDetectorClockDifference();
    else
        detector = new ErrorDetectorInterval();
//    detector.setClockDifferenceConsideredDependency(params->nbNodes/params->delaySend, params->entriesIncrementedByProcess[0].size());
}

AppMsg* NodeDetector::createAppMsg()
{
    AppMsg* m = NodeWithControl::createAppMsg();
    m = detector->prepareMessage(m);
    return m;
}

AppMsg* NodeDetector::prepareBroadcast()
{
    AppMsg* m = NodeWithControl::prepareBroadcast();

    if(APPENDDEPENDENCIES)
    {
        stats.nbSentDependencies += m->getDependencies().size();
        stats.controlDataSize += clock.size() * sizeof(unsigned int) + m->getDependencies().size()*2* sizeof(unsigned int);
    }
    return m;
}
