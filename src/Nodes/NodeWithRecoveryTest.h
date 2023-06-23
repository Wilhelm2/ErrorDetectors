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

#include "NodeRecovery.h"

class NodeWithRecoveryTest : public NodeRecovery{
public:
    NodeWithRecoveryTest();
    virtual ~NodeWithRecoveryTest();
    bool testDeliverMessage(dep m);
    bool recoveryTest(dep message, vector<dep>& delivered, vector<unsigned int> incrementedClockEntries);
    bool TryCombinationsRecoveringMessages(dep message, unsigned int& nbHashs, TotalDependencies baseDependencies, vector<idMsg> messagesToCombine);
    vector<idMsg> createRecoveredMessagesToCombine(dep message, vector<unsigned int> incrementedClockEntries);

};

#endif /* NODES_NODEWITHRECOVERYTEST_H_ */
