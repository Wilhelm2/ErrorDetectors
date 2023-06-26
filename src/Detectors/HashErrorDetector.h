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


typedef struct s_stats_hashErrorDetector{
    unsigned int nbHash=0;
    vector<unsigned int> nbMsgWronglyConsideredCausalDep; // entry i : in msgACombiner number of concurrent messages wronly considered as being causal dependencies
    vector<unsigned int> nbMsgToCombine;
    vector<unsigned int> doneCombinationsToFindHash; // entry i : required i hashs to find the message hash
    vector<unsigned int> nbOperationsForHash;
}stats_hashErrorDetector;

class HashErrorDetector : public ErrorDetector{
public:
    HashErrorDetector();
    virtual ~HashErrorDetector();

    virtual bool test(messageInfo message, const vector<unsigned int>& incrementedClockEntries, const ProbabilisticClock& processClock, const TotalDependencies& processDependencies, Controller* control, SimulationParameters* params, const vector<messageInfo>& delivered);
    bool hashPartialDependencies(const messageInfo& message, const vector<messageInfo>& delivered, const vector<unsigned int>& incrementedClockEntries, Controller* controller, SimulationParameters* params);
    bool hashTotalDependencies(const messageInfo& message, const vector<messageInfo>& delivered, const vector<unsigned int>& incrementedClockEntries, const TotalDependencies& processDependencies, Controller* controller, SimulationParameters* params);
    TotalDependencies createBaseDependencies(const messageInfo& message, const vector<messageInfo>& delivered, const vector<unsigned int>& incrementedClockEntries, const TotalDependencies& processDependencies);
    bool isPossibleDependency(const messageInfo& message, const messageInfo& possibleDep);
    vector<messageInfo> createPossibleDependenciesSet(const messageInfo& message, const vector<messageInfo>& messageToChooseFrom, const vector<unsigned int>& incrementedClockEntries, Controller* controller);
    void incrementnbMsgWronglyConsideredCausalDep(unsigned int entry);
    void incrementnbMsgToCombine(unsigned int entry);
    void incrementnbOperationsForHash(unsigned int entry);
    size_t hashTotalDependencies(const TotalDependencies& dependencies);
    size_t hashPartialDependencies(const PartialDependencies& dependencies);
    size_t hashDependencies(const vector<unsigned int>& dependencies);
    size_t hashingF(const vector<uint32_t>& vec);

    virtual bool isConsideredAsDependency(messageInfo message, messageInfo possibleDep) = 0;
    virtual bool isConsideredAsPossibleDependency(messageInfo message, messageInfo possibleDep) = 0;
    virtual vector<idMsg> sortPossibleDependenciesSet(messageInfo message, vector<messageInfo> baseCombineSet, Controller* controller) = 0;

    AppMsg* prepareMessage(AppMsg* m, const vector<messageInfo>& delivered, const ProbabilisticClock& clock, const TotalDependencies& processDependencies);


    virtual PartialDependencies getPartialDependencies(const vector<messageInfo>& delivered, const ProbabilisticClock& clock) = 0;
    virtual vector<idMsg> determineAndGetAppendedDependencies(const vector<messageInfo>& delivered, const ProbabilisticClock& clock) = 0;

    hash<string> hasher;
    static map<int,vector<unsigned int>> collisionController;
    stats_hashErrorDetector hashStats;

};

#endif /* DETECTORS_HASHERRORDETECTOR_H_ */
