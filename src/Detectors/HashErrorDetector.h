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


/** Statistics relevant to hash error detectors*/
typedef struct s_stats_hashErrorDetector{
    /** Total number of hashes the node computed. */
    unsigned int nbHash=0;
    /** Number of messages the node wrongly considered as being causal dependencies in the hash computation of messages it delivered (obtained by calling the controler).
     * entry i : Number of times where in the hash computation to deliver a message $m$ the node wrongly considered i messages as being causal dependencies of $m$. */
    vector<unsigned int> nbMsgWronglyConsideredCausalDep;
    /** Number of possible dependencies in the hash computation of messages.
     * entry i : Number of times where the node considered i messages as possible dependencies in the hash computation of a message m. */
    vector<unsigned int> nbMsgToCombine;
    /** Number of done combinations to find the hash of delivere messages.
     * entry i : Number of times where the node computed i hashes to find a dependency set whose hash was equal to the one of the message to deliver.*/
    vector<unsigned int> doneCombinationsToFindHash;
    /** Number of operations to compute a hash.
     * entry i : Number of times where the node did i operations to compute the hash of the causal dependencies of a message. */
    vector<unsigned int> nbOperationsForHash;
}stats_hashErrorDetector;

/**Base class of Hash-based error detectors.*/
class HashErrorDetector : public ErrorDetector{
    public:
        HashErrorDetector();
        virtual ~HashErrorDetector();

        virtual bool test(messageInfo message, const vector<unsigned int>& incrementedClockEntries, const ProbabilisticClock& processClock, const TotalDependencies& processDependencies, Controller* control, SimulationParameters* params, const vector<messageInfo>& delivered);
        bool hashPartialDependencies(const messageInfo& message, const vector<messageInfo>& delivered, const vector<unsigned int>& incrementedClockEntries, Controller* controller, SimulationParameters* params);
        bool hashTotalDependencies(const messageInfo& message, const vector<messageInfo>& delivered, const vector<unsigned int>& incrementedClockEntries, const TotalDependencies& processDependencies, Controller* controller, SimulationParameters* params);
        bool hashIsEqual(size_t messageHash, size_t dependenciesHash, unsigned int nbComputedHashes);
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

        AppMsg* prepareMessage(AppMsg* m, const vector<messageInfo>& delivered, const ProbabilisticClock& clock, const TotalDependencies& processDependencies);

        virtual bool isConsideredAsDependency(const messageInfo& message, const messageInfo& possibleDep) = 0;
        virtual bool isConsideredAsPossibleDependency(const messageInfo& message, const messageInfo& possibleDep) = 0;
        virtual vector<idMsg> sortPossibleDependenciesSet(const messageInfo& message, const vector<messageInfo>& baseCombineSet) = 0;
        virtual PartialDependencies getPartialDependencies(const vector<messageInfo>& delivered, const ProbabilisticClock& clock) = 0;
        virtual vector<idMsg> determineAndGetAppendedDependencies(const vector<messageInfo>& delivered, const ProbabilisticClock& clock) = 0;

    private:
        /**Used to compute hashes.*/
        hash<string> hasher;
        /**Used to detect collisions when hashing dependencies sets, ie detect when different dependencies sets have the same hash. Is inactivated by default because it uses a lot of memory space. To activate, set COLLISION_CONTROL in simulatioParameters to true.*/
        static map<int,vector<unsigned int>> collisionController;
        /** Statistics about hash-based error detectors. */
        stats_hashErrorDetector hashStats;
    friend class Stats;
    friend class NodeWithRecoveryTest;
};

#endif /* DETECTORS_HASHERRORDETECTOR_H_ */
