/*
 * ErrorDetectorInterval.h
 *
 *  Created on: Mar 10, 2023
 *      Author: wilhelm
 */

#ifndef ERRORDETECTORINTERVAL_H_
#define ERRORDETECTORINTERVAL_H_

#include "../Detectors/HashErrorDetector.h"


/** Hash-based error detector that uses propagation delay hypothesises to determine the causal dependencies of messages. */
class ErrorDetectorInterval : public HashErrorDetector
{
    public:
        ErrorDetectorInterval();
        virtual ~ErrorDetectorInterval();
        virtual vector<idMsg> determineAndGetAppendedDependencies(const vector<messageInfo>& delivered, const ProbabilisticClock& clock);
        bool isConsideredAsDependency(const messageInfo& message, const messageInfo& possibleDep);
        bool isConsideredAsPossibleDependency(const messageInfo& message, const messageInfo& possibleDep);
        vector<idMsg> sortPossibleDependenciesSet(const messageInfo& message, const vector<messageInfo>& baseCombineSet);
        PartialDependencies getPartialDependencies(const vector<messageInfo>& delivered, const ProbabilisticClock& clock);

    private:
        /**Assumes that messages received more than 2*deltaTR seconds ago are causal dependencies. Do not take a value that is too high. */
        simtime_t deltaTR = *(new SimTime(100, SIMTIME_MS));
        /**Communication channel delay */
        simtime_t cdelay = *(new SimTime(100, SIMTIME_MS));
        /**Same function as deltaTR but separates them so that deltaTR can set to 0 when the message load is low.*/
        simtime_t deltaTS = *(new SimTime(140, SIMTIME_MS));
};

#endif /* ERRORDETECTORINTERVAL_H_ */
