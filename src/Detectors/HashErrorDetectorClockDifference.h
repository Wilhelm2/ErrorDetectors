/*
 * ErrorDetectorClockDifference.h
 *
 *  Created on: Mar 10, 2023
 *      Author: wilhelm
 */

#ifndef ERRORDETECTORCLOCKDIFFERENCE_H_
#define ERRORDETECTORCLOCKDIFFERENCE_H_

#include "../Detectors/HashErrorDetector.h"

class ErrorDetectorClockDifference : public HashErrorDetector{
public:
    ErrorDetectorClockDifference();
    virtual ~ErrorDetectorClockDifference();
    vector<idMsg> determineAndGetAppendedDependencies(const vector<dep>& delivered, const ProbabilisticClock& messageClock);
    bool isConsideredAsDependency(dep message, dep possibleDep);
    bool isConsideredAsPossibleDependency(dep message, dep possibleDep);
    vector<idMsg> sortPossibleDependenciesSet(dep message, vector<dep> baseCombineSet, Controller* controller);
    void setClockDifferenceConsideredDependency(unsigned int messageLoad, unsigned nbIncrementedEntries);
    PartialDependencies getPartialDependencies(const vector<dep>& delivered, const ProbabilisticClock& clock);

    unsigned int clockDifferenceConsideredDependency = 100;
    unsigned int clockDifferenceConsideredNotDependency = 0;

};

#endif /* ERRORDETECTORCLOCKDIFFERENCE_H_ */
