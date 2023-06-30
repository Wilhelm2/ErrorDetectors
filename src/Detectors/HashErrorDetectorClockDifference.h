/*
 * ErrorDetectorClockDifference.h
 *
 *  Created on: Mar 10, 2023
 *      Author: wilhelm
 */

#ifndef ERRORDETECTORCLOCKDIFFERENCE_H_
#define ERRORDETECTORCLOCKDIFFERENCE_H_

#include "../Detectors/HashErrorDetector.h"

/** Hash-based error detector that uses the clock difference between message to determine the causal dependencies of messages.
 * Assumes that probabilistic clocks increase over time. Thus, the greater the clock difference between two messages, the greater the duration between their broadcast.
 * Therefore, messages over a given clock difference are considered as having been delivered by all nodes, and are not considered in the hash computation. */
class ErrorDetectorClockDifference : public HashErrorDetector
{
    public:
        ErrorDetectorClockDifference();
        virtual ~ErrorDetectorClockDifference();
        virtual vector<idMsg> determineAndGetAppendedDependencies(const vector<messageInfo>& delivered, const ProbabilisticClock& clock);
        bool isConsideredAsDependency(const messageInfo& message, const messageInfo& possibleDep);
        bool isConsideredAsPossibleDependency(const messageInfo& message, const messageInfo& possibleDep);
        vector<idMsg> sortPossibleDependenciesSet(const messageInfo& message, const vector<messageInfo>& baseCombineSet);
        void setClockDifferenceConsideredDependency(unsigned int messageLoad, unsigned nbIncrementedEntries);
        PartialDependencies getPartialDependencies(const vector<messageInfo>& delivered, const ProbabilisticClock& clock);

    private:
        /** The clock difference over which a message is considered as having been delivered by all nodes and is thus not taken into account in the hash computation.*/
        unsigned int clockDifferenceConsideredDependency = 100;
        /** The clock difference under which a message is considered as not being a dependency of a message to deliver, because its low clock difference indicates that it has been broadcasted in a time interval very clock to the broadcast of the message to deliver.*/
        unsigned int clockDifferenceConsideredNotDependency = 0;
};

#endif /* ERRORDETECTORCLOCKDIFFERENCE_H_ */
