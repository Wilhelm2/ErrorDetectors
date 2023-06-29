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

#ifndef NODES_NODEWITHCONTROL_H_
#define NODES_NODEWITHCONTROL_H_

#include "NodeBase.h"

using namespace omnetpp;
using namespace std;

/** Base class for nodes that use probabilistic clocks to causally order application messages.
 */
class NodeWithControl : public NodeBase
{
    public:
        NodeWithControl();
        virtual ~NodeWithControl();

    protected:
        virtual void initialize() override;

        virtual AppMsg* createAppMsg();
        virtual AppMsg* prepareBroadcast();
        void removeOldMessages();
        virtual bool deliverMsg(const messageInfo& message);
        void RecvAppMsg(AppMsg*m);
        const vector<unsigned int>& getIndexIncrementedEntries(unsigned int idSource);

        virtual void processMessage(cMessage* msg) = 0;
        virtual bool testDeliverMessage(const messageInfo& message) = 0;
        virtual void iterativeDelivery() = 0;

        /** The node's local Probabilistic clock*/
        ProbabilisticClock clock;
        /** Causal information about the messages the node has delivered.*/
        TotalDependencies deliveredMessagesTracker ;
        /** Contains the messages whose clock does not satisfy the deliver condition.*/
        vector<messageInfo> pendingMsg;
        /** Contains information about the messages the node delivered. */
        vector<messageInfo> delivered;

        friend class Stats;
};

#endif /* NODES_NODEWITHCONTROL_H_ */
