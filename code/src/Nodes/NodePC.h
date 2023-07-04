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

#ifndef NODES_NODEPC_H_
#define NODES_NODEPC_H_

#include "NodeWithControl.h"
/** Node using Probabilistic clocks to causally order messages (without error detectors). */
class NodePC : public NodeWithControl
{
    public:
        NodePC();
        virtual ~NodePC();

    protected:
        virtual void processMessage(cMessage* msg);
        virtual bool testDeliverMessage(const messageInfo& message);
        virtual void iterativeDelivery();

};

#endif /* NODES_NODEPC_H_ */
