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

#ifndef DETECTORS_MOSTEFAOUIERRORDETECTOR_H_
#define DETECTORS_MOSTEFAOUIERRORDETECTOR_H_

#include "../Clocks/ProbabilisticClock.h"
#include "../Detectors/ErrorDetector.h"

/**Error detector proposed by Mostéfaoui and Weiss. */
class MostefaouiErrorDetector : public ErrorDetector {
   public:
    MostefaouiErrorDetector();
    virtual ~MostefaouiErrorDetector();
    virtual bool test(messageInfo message,
                      const vector<unsigned int>& incrementedClockEntries,
                      const ProbabilisticClock& processClock,
                      const TotalDependencies& processDependencies,
                      Controller* control,
                      SimulationParameters* params,
                      const vector<messageInfo>& delivered);
    virtual AppMsg* prepareMessage(AppMsg* m,
                                   const vector<messageInfo>& delivered,
                                   const ProbabilisticClock& clock,
                                   const TotalDependencies& processDependencies);
};

#endif /* DETECTORS_MOSTEFAOUIERRORDETECTOR_H_ */
