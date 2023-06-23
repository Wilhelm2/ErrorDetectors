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

#ifndef DETECTORS_HASHERRORDETECTOR_H_
#define DETECTORS_HASHERRORDETECTOR_H_

#include "../structures.h"
#include "../Controller.h"
#include "../Dependencies/Dependencies.h"
#include "../Dependencies/PartialDependencies.h"
#include "../Dependencies/TotalDependencies.h"
#include "../Detectors/ErrorDetector.h"


typedef struct s_stats_errorDetector{
    unsigned int nbHash=0;
    vector<unsigned int> nbMsgWronglyConsideredCausalDep; // entry i : in msgACombiner number of concurrent messages wronly considered as being causal dependencies
    vector<unsigned int> nbMsgToCombine;
    vector<unsigned int> doneCombinationsToFindHash; // entry i : required i hashs to find the message hash
    vector<unsigned int> nbOperationsForHash;
    unsigned int falseNegative = 0; // hashTest returns false even though all dependencies are delivered
    unsigned int trueNegative = 0; // hashTest returns false and message can effectively not be delivered
}stats_errorDetector;

class HashErrorDetector : public ErrorDetector{
public:
    HashErrorDetector();
    virtual ~HashErrorDetector();
    bool hashTest(dep message, const vector<dep>& delivered, const vector<unsigned int>& incrementedClockEntries, TotalDependencies processDependencies, Controller* controller, SimulationParameters* params);
    bool hashPartialDependencies(dep message, const vector<dep>& delivered, const vector<unsigned int>& incrementedClockEntries, Controller* controller, SimulationParameters* params);
    bool hashTotalDependencies(dep message, const vector<dep>& delivered, const vector<unsigned int>& incrementedClockEntries, TotalDependencies processDependencies, Controller* controller, SimulationParameters* params);
    TotalDependencies createBaseDependencies(dep message, const vector<dep>& delivered, const vector<unsigned int>& incrementedClockEntries, TotalDependencies processDependencies);
    bool isPossibleDependency(dep message, dep possibleDep);
    vector<dep> createPossibleDependenciesSet(dep message, const vector<dep>& messageToChooseFrom, const vector<unsigned int>& incrementedClockEntries, Controller* controller);
    void incrementnbMsgWronglyConsideredCausalDep(unsigned int entry);
    void incrementnbMsgToCombine(unsigned int entry);
    void incrementnbOperationsForHash(unsigned int entry);
    size_t hashTotalDependencies(TotalDependencies dependencies);
    size_t hashPartialDependencies(const PartialDependencies& dependencies);
    size_t hashDependencies(const vector<unsigned int>& dependencies);
    size_t hashingF(const vector<uint32_t>& vec);
    vector<idMsg> determineAndGetAppendedDependencies(const vector<dep>& delivered);

    virtual bool isConsideredAsDependency(dep message, dep possibleDep) = 0;
    virtual bool isConsideredAsPossibleDependency(dep message, dep possibleDep) = 0;
    virtual vector<idMsg> sortPossibleDependenciesSet(dep message, vector<dep> baseCombineSet, Controller* controller) = 0;

    hash<string> hasher;
    static map<int,vector<unsigned int>> collisionController;
    stats_errorDetector stats;

};

#endif /* DETECTORS_HASHERRORDETECTOR_H_ */
