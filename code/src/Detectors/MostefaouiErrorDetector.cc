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

#include "../Detectors/MostefaouiErrorDetector.h"

MostefaouiErrorDetector::MostefaouiErrorDetector() {}

MostefaouiErrorDetector::~MostefaouiErrorDetector() {}

bool MostefaouiErrorDetector::test(messageInfo message,
                                   const vector<unsigned int>& incrementedClockEntries,
                                   const ProbabilisticClock& processClock,
                                   const TotalDependencies& processDependencies,
                                   Controller* control,
                                   SimulationParameters* params,
                                   const vector<messageInfo>& delivered) {
    // test if exists an entry $i$ such that V[i] == m.V[i] -1
    for (int i : incrementedClockEntries)
        if (processClock[i] == (message.clock[i] - 1))
            return true;
    return false;
}

AppMsg* MostefaouiErrorDetector::prepareMessage(AppMsg* m,
                                                const vector<messageInfo>& delivered,
                                                const ProbabilisticClock& clock,
                                                const TotalDependencies& processDependencies) {
    return m;
}
