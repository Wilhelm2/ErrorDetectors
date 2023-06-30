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

#ifndef ERRORDETECTOR_H_
#define ERRORDETECTOR_H_
#include <string>
#include <regex>
#include <omnetpp.h>

#include "../Messages/AppMsg_m.h"
#include "../Controller.h"

using namespace std;
using namespace omnetpp;

/** Stats relevant to error detectors. */
typedef struct s_stats_errorDetector{
    /** Cases where the error detector returns false even though all dependencies are delivered.*/
    unsigned int falseNegative = 0;
    /** Cases where the error detector returns false and message are effectively not causally ordered.*/
    unsigned int trueNegative = 0;
}stats_errorDetector;

/** Base class of error detectors*/
class ErrorDetector {
    public:
        ErrorDetector();
        virtual ~ErrorDetector() = 0;
        /** Appends to the message to broadcast the information the destination error detector requires.*/
        virtual AppMsg* prepareMessage(AppMsg* m, const vector<messageInfo>& delivered, const ProbabilisticClock& clock, const TotalDependencies& processDependencies) = 0;
        /** Error detector test to determine if the message to deliver can be delivered in causal order.*/
        virtual bool test(messageInfo message, const vector<unsigned int>& incrementedClockEntries, const ProbabilisticClock& processClock, const TotalDependencies& processDependencies, Controller* control, SimulationParameters* params, const vector<messageInfo>& delivered) = 0;
    private:
        stats_errorDetector stats;
    friend class Stats;
    friend class NodeWithoutRecovery;
    friend class NodeWithRecovery;
    friend class NodeWithRecoveryTest;
};

#endif /* ERRORDETECTOR_H_ */
