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


class NodeWithControl : public NodeBase
{
    public:
        NodeWithControl();
        virtual ~NodeWithControl();

        virtual void initialize() override;

        virtual AppMsg* createAppMsg();
        virtual AppMsg* prepareBroadcast();
        void removeOldMessages();
        virtual bool deliverMsg(messageInfo message);
        void RecvAppMsg(AppMsg*m);
        const vector<unsigned int>& getIndexIncrementedEntries(unsigned int idSource);


        virtual void processMessage(cMessage* msg) = 0;
        virtual bool testDeliverMessage(messageInfo message) = 0;
        virtual void iterativeDelivery() = 0;

        ProbabilisticClock clock;
        TotalDependencies deliveredMessagesTracker ; // tracks dependencies of process
        vector<messageInfo> pendingMsg;
        vector<messageInfo> delivered;
};

#endif /* NODES_NODEWITHCONTROL_H_ */
