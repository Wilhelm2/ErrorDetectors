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

#ifndef NODES_NODEWITHRECOVERYTEST_H_
#define NODES_NODEWITHRECOVERYTEST_H_

#include "NodeWithRecovery.h"

/** Nodes that recover messages and which try to first retrieve the causal dependencies of a message locally before
 * requesting its dependencies to its sending node.*/
class NodeWithRecoveryTest : public NodeWithRecovery {
   public:
    NodeWithRecoveryTest();
    virtual ~NodeWithRecoveryTest();

   protected:
    bool testDeliverMessage(const messageInfo& m);
    bool recoveryTest(const messageInfo& message,
                      const vector<messageInfo>& delivered,
                      const vector<unsigned int>& incrementedClockEntries);
    bool tryHash(const messageInfo& message, const TotalDependencies& dependencies, unsigned doneHashes);
    bool TryCombinationsRecoveringMessages(const messageInfo& message,
                                           unsigned int& nbHashs,
                                           const TotalDependencies& baseDependencies,
                                           const vector<idMsg>& messagesToCombine);
    vector<idMsg> createRecoveredMessagesToCombine(const messageInfo& message,
                                                   const vector<unsigned int>& incrementedClockEntries);
};

#endif /* NODES_NODEWITHRECOVERYTEST_H_ */
