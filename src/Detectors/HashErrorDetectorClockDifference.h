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
        virtual vector<idMsg> determineAndGetAppendedDependencies(const vector<messageInfo>& delivered, const ProbabilisticClock& clock);
        bool isConsideredAsDependency(const messageInfo& message, const messageInfo& possibleDep);
        bool isConsideredAsPossibleDependency(const messageInfo& message, const messageInfo& possibleDep);
        vector<idMsg> sortPossibleDependenciesSet(const messageInfo& message, const vector<messageInfo>& baseCombineSet, Controller* controller);
        void setClockDifferenceConsideredDependency(unsigned int messageLoad, unsigned nbIncrementedEntries);
        PartialDependencies getPartialDependencies(const vector<messageInfo>& delivered, const ProbabilisticClock& clock);

    private:
        unsigned int clockDifferenceConsideredDependency = 100;
        unsigned int clockDifferenceConsideredNotDependency = 0;
};

#endif /* ERRORDETECTORCLOCKDIFFERENCE_H_ */
