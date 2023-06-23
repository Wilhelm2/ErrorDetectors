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

MostefaouiErrorDetector::MostefaouiErrorDetector() {
    // TODO Auto-generated constructor stub

}

MostefaouiErrorDetector::~MostefaouiErrorDetector() {
    // TODO Auto-generated destructor stub
}

bool MostefaouiErrorDetector::test(unsigned int idMsg, ProbabilisticClock& PCMsg, vector<unsigned int> incrementedEntries, ProbabilisticClock processClock)
{
    // test if exists an entry $i$ such that V[i] == m.V[i] -1
    for(int i : incrementedEntries)
        if(processClock[i] == (PCMsg[i] -1))
            return true;
    return false;
}
