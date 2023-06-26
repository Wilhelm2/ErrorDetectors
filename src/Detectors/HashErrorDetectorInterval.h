/*
 * ErrorDetectorInterval.h
 *
 *  Created on: Mar 10, 2023
 *      Author: wilhelm
 */

#ifndef ERRORDETECTORINTERVAL_H_
#define ERRORDETECTORINTERVAL_H_

#include "../Detectors/HashErrorDetector.h"

class ErrorDetectorInterval : public HashErrorDetector{
public:
    ErrorDetectorInterval();
    virtual ~ErrorDetectorInterval();

    simtime_t deltaTR = *(new SimTime(100, SIMTIME_MS)); // 2) Suppose que les messages reçues il y a plus de 2*deltaTR sont des dépendances du messages ATTENTION PAS TROP MONTER SINON IL Y A TROP DE MSG A ANALYSER ET NE TROUVE JAMAIS
    simtime_t cdelay = *(new SimTime(100, SIMTIME_MS));
    // seperate deltaTR and deltaTS so that deltaTR can be set to 0 when the message load is low
    simtime_t deltaTS = *(new SimTime(140, SIMTIME_MS));

    virtual vector<idMsg> determineAndGetAppendedDependencies(const vector<messageInfo>& delivered, const ProbabilisticClock& clock);
    bool isConsideredAsDependency(messageInfo message, messageInfo possibleDep);
    bool isConsideredAsPossibleDependency(messageInfo message, messageInfo possibleDep);
    vector<idMsg> sortPossibleDependenciesSet(messageInfo message, vector<messageInfo> baseCombineSet, Controller* controller);
    PartialDependencies getPartialDependencies(const vector<messageInfo>& delivered, const ProbabilisticClock& clock);

};

#endif /* ERRORDETECTORINTERVAL_H_ */
