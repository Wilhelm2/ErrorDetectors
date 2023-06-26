/*
 * ErrorDetectorClockDifference.h
 *
 *  Created on: Mar 10, 2023
 *      Author: wilhelm
 */

#ifndef ERRORDETECTORCLOCKDIFFERENCE_H_
#define ERRORDETECTORCLOCKDIFFERENCE_H_

#include "../Detectors/HashErrorDetector.h"

class ErrorDetectorClockDifference : public HashErrorDetector
{
    public:
        ErrorDetectorClockDifference();
        virtual ~ErrorDetectorClockDifference();
        vector<idMsg> determineAndGetAppendedDependencies(const vector<messageInfo>& delivered, const ProbabilisticClock& messageClock);
        bool isConsideredAsDependency(messageInfo message, messageInfo possibleDep);
        bool isConsideredAsPossibleDependency(messageInfo message, messageInfo possibleDep);
        vector<idMsg> sortPossibleDependenciesSet(messageInfo message, vector<messageInfo> baseCombineSet, Controller* controller);
        void setClockDifferenceConsideredDependency(unsigned int messageLoad, unsigned nbIncrementedEntries);
        PartialDependencies getPartialDependencies(const vector<messageInfo>& delivered, const ProbabilisticClock& clock);

        unsigned int clockDifferenceConsideredDependency = 100;
        unsigned int clockDifferenceConsideredNotDependency = 0;
};

#endif /* ERRORDETECTORCLOCKDIFFERENCE_H_ */
