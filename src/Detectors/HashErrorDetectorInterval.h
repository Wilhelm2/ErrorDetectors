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

    vector<idMsg> determineAndGetAppendedDependencies(vector<dep>& delivered, ProbabilisticClock messageClock);
    bool isConsideredAsDependency(dep message, dep possibleDep);
    bool isConsideredAsPossibleDependency(dep message, dep possibleDep);
    vector<idMsg> sortPossibleDependenciesSet(dep message, vector<dep> baseCombineSet, Controller* controller);

};

#endif /* ERRORDETECTORINTERVAL_H_ */
