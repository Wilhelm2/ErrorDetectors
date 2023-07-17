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

#ifndef NODES_NODEDETECTOR_H_
#define NODES_NODEDETECTOR_H_

#include "../Detectors/HashErrorDetectorClockDifference.h"
#include "../Detectors/HashErrorDetectorInterval.h"
#include "../Detectors/MostefaouiErrorDetector.h"
#include "../structures.h"
#include "NodeWithControl.h"

/** Base class to implement nodes using error detectors.
 */
class NodeDetector : public NodeWithControl {
   protected:
    NodeDetector();
    virtual ~NodeDetector();
    virtual void initialize() override;

    AppMsg* createAppMsg();
    AppMsg* prepareBroadcast();

    /** Processes messages the process received (self messages and messages received from other nodes).*/
    virtual void processMessage(cMessage* msg) = 0;
    /** Tries to deliver a message. It verifies the message's clock delivery conditions, and delivers them message if
     * its delivery conditions are satisfied.
     * @param message Information about the message to deliver
     * @return true if the node delivered the message and false otherwise.*/
    virtual bool testDeliverMessage(const messageInfo& message) = 0;
    /** Tries to deliver pending messages. */
    virtual void iterativeDelivery() = 0;

    /** Error detector used to analyze messages before delivering them.*/
    ErrorDetector* detector;

    friend class Stats;
};

#endif /* NODES_NODEDETECTOR_H_ */
