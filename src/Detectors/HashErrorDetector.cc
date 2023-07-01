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

#include "../Detectors/HashErrorDetector.h"

map<int,vector<unsigned int>> HashErrorDetector::collisionController;

HashErrorDetector::HashErrorDetector() {
    hashStats.nbMsgWronglyConsideredCausalDep.resize(200,0);
    hashStats.nbMsgToCombine.resize(200,0);
    hashStats.doneCombinationsToFindHash.resize(LIMIT_HASHS+1,0);
    hashStats.nbOperationsForHash.resize(200,0);
}

HashErrorDetector::~HashErrorDetector() {
    // TODO Auto-generated destructor stub
}

/** Test executed on a message to determine if it can be causally delivered.
 * @param message Message to deliver.
 * @param incrementedClockEntries Entries the sender node of message incremented when broadcasting message.
 * @param processClock Probabilistic clock of the current node.
 * @param processDependencies Tracker of the delivered messages of the current node.
 * @param control Controller module.
 * @param params Simulation parameters.
 * @param delivered Messages delivered by the current node.
 * @return true if the error detector concludes that the message can be causally delivered and false otherwise. */
bool HashErrorDetector::test(messageInfo message, const vector<unsigned int>& incrementedClockEntries, const ProbabilisticClock& processClock, const TotalDependencies& processDependencies, Controller* control, SimulationParameters* params, const vector<messageInfo>& delivered)
{
    if(SimulationParameters::depAppended == SimulationParameters::Dependencies::Total)
        return hashTotalDependencies(message, delivered, incrementedClockEntries, processDependencies, control, params);
    else
        return hashPartialDependencies(message, delivered, incrementedClockEntries, control, params);
}

/** Test executed on a message when using partial dependencies to determine if it can be causally delivered.
 * @param message Message to deliver.
 * @param delivered Messages delivered by the current node.
 * @param incrementedClockEntries Entries the sender node of message incremented when broadcasting message.
 * @param controller Controller module.
 * @param params Simulation parameters.
 * @return true if the error detector concludes that the message can be causally delivered and false otherwise. */
bool HashErrorDetector::hashPartialDependencies(const messageInfo& message, const vector<messageInfo>& delivered, const vector<unsigned int>& incrementedClockEntries, Controller* controller, SimulationParameters* params)
{
    PartialDependencies baseDependencies;
    unsigned int nbHashs = 0;

    vector<idMsg> possibleDependencies = sortPossibleDependenciesSet(message, createPossibleDependenciesSet(message, delivered, incrementedClockEntries, controller));
    vector<vector<bool>> testedDependencySets = params->getDependencyCombinations(possibleDependencies.size());
    for(vector<bool>& isDepConsideredVec : testedDependencySets)
    {
        baseDependencies.clear();
        for(unsigned int i=0; i<isDepConsideredVec.size(); i++)
        {
            if(isDepConsideredVec[i])
                baseDependencies.set(possibleDependencies[i]);
        }
        nbHashs++;
        if(hashIsEqual(message.hash, hashPartialDependencies(baseDependencies), nbHashs))
            return true;
    }
    nbHashs++;
    return hashIsEqual(message.hash, hashPartialDependencies({}), nbHashs); // tries to compute the hash with no dependencies
}

/** Verifies if the hash of the message and dependencies set are equal.
 * @param messageHash Hash of the message to deliver.
 * @param dependenciesHash Hash of the dependencies set.
 * @param nbComputedHashes Number of computed hashes in the current hash computation.
 * @return true if the hash is equal, false otherwise.*/
bool HashErrorDetector::hashIsEqual(size_t messageHash, size_t dependenciesHash, unsigned int nbComputedHashes)
{
    if(messageHash == dependenciesHash)
    {
        hashStats.doneCombinationsToFindHash[nbComputedHashes]++;
        return true;
    }
    return false;
}

/** Test executed on a message when using total dependencies to determine if it can be causally delivered.
 * @param message Message to deliver.
 * @param delivered Messages delivered by the current node.
 * @param incrementedClockEntries Entries the sender node of message incremented when broadcasting message.
 * @param processDependencies Tracker of the delivered messages of the current node.
 * @param controller Controller module.
 * @param params Simulation parameters.
 * @return true if the error detector concludes that the message can be causally delivered and false otherwise. */
bool HashErrorDetector::hashTotalDependencies(const messageInfo& message, const vector<messageInfo>& delivered, const vector<unsigned int>& incrementedClockEntries, const TotalDependencies& processDependencies, Controller* controller, SimulationParameters* params)
{
    TotalDependencies baseDependencies = createBaseDependencies(message, delivered, incrementedClockEntries, processDependencies);
    TotalDependencies copieBaseDependencies;
    unsigned int nbHashs = 0;

    if(APPENDDEPENDENCIES)
    {
        if(!baseDependencies.includesDependencies(message.dependencies)) // known dependency not delivered
            return false;
    }
    nbHashs++;
    if(hashIsEqual(message.hash, hashTotalDependencies(baseDependencies), nbHashs))
        return true;
    vector<messageInfo> set = createPossibleDependenciesSet(message, delivered, incrementedClockEntries, controller);
    vector<idMsg> possibleDependencies = sortPossibleDependenciesSet(message, set);

    vector<vector<bool>> testedDependencySets = params->getDependencyCombinations(possibleDependencies.size());
    for(const vector<bool>& isDepConsideredVec : testedDependencySets)
    {
        copieBaseDependencies = baseDependencies;
        for(unsigned int i=0; i<isDepConsideredVec.size(); i++)
        {
            if(isDepConsideredVec[i])
                copieBaseDependencies[possibleDependencies[i].id] = possibleDependencies[i].seq;
        }
        nbHashs++;
        if(hashIsEqual(message.hash, hashTotalDependencies(copieBaseDependencies), nbHashs))
            return true;
    }
//    if(compareHashedDepToHash(vModele, message.hash, nbHashs))
  //      return true;
    return false;
}

/** Creates the set of messages that are considered as dependencies of the message to deliver.
 * @param message Message to deliver.
 * @param delivered Messages delivered by the current node.
 * @param incrementedClockEntries Entries the sender node of message incremented when broadcasting message.
 * @param processDependencies Tracker of the delivered messages of the current node.
 * @return The set of messages that are considered as being dependencies of message.
 * */
TotalDependencies HashErrorDetector::createBaseDependencies(const messageInfo& message, const vector<messageInfo>& delivered, const vector<unsigned int>& incrementedClockEntries, const TotalDependencies& processDependencies)
{
//    for (unsigned int entry : incrementedClockEntries) commented because interfers with hash computed with clock difference (+small impact)
  //          message.clock.decreaseEntry(entry); // improvment to detect concurrent messages
    TotalDependencies baseDependencies = processDependencies;
    // check for concurrent messages to remove from the base dependencies
    for(const messageInfo& possibleDep : delivered)
    {
        if(!isPossibleDependency(message, possibleDep) || !isConsideredAsDependency(message, possibleDep)) // message cannot be a dependency or received to recently (in which case it is considered as possible dependency)
            baseDependencies[possibleDep.id.id] = min(possibleDep.id.seq-1, baseDependencies[possibleDep.id.id]);
    }
    return baseDependencies;
}

/** Determines if a message is a possible dependency of another message.
 * @param message Message of which possibleDep might be a dependency.
 * @param possibleDep Possible dependency.
 * @return Returns true if possibleDep is a possible dependency of message. */
bool HashErrorDetector::isPossibleDependency(const messageInfo& message, const messageInfo& possibleDep)
{
    if(!(possibleDep.clock <= message.clock))
        return false;
    if(APPENDDEPENDENCIES)
    {
        if(isConsideredAsPossibleDependency(message, possibleDep))
        {
            for(const idMsg& dependency: message.dependencies)
            {
                if(IDMSG_EQ(dependency, possibleDep.id))
                    return true; // received too recently but in dependencies
            }
            return false; // received too recently
        }
    }
    return true;
}


/** Creates the set of messages that are considered as possible dependencies of the message to deliver.
 * @param message Message to deliver.
 * @param messageToChooseFrom Set containing the messages that might be possible dependencies of message.
 * @param incrementedClockEntries Entries the sender node of message incremented when broadcasting message.
 * @param controller Controller module.
 * @return The set of messages that are considered as possible dependencies of message.
 * */
vector<messageInfo> HashErrorDetector::createPossibleDependenciesSet(const messageInfo& message, const vector<messageInfo>& messageToChooseFrom, const vector<unsigned int>& incrementedClockEntries, Controller* controller)
{
//    for (unsigned int entry : incrementedClockEntries) commented because interfers with hash computed with clock difference (+small impact)
  //      message.clock.decreaseEntry(entry); // Improvement to detect concurrent messages

    vector<messageInfo> BaseCombineSet;
    unsigned int falseDetected = 0;

    for(const messageInfo& possibleDep : messageToChooseFrom)
    {
        if(isPossibleDependency(message, possibleDep) && isConsideredAsPossibleDependency(message, possibleDep))
        {
            BaseCombineSet.push_back(possibleDep);
            if(!controller->isDependency(message.id, possibleDep.id))
                falseDetected ++;
        }
    }
    incrementnbMsgWronglyConsideredCausalDep(falseDetected);
    incrementnbMsgToCombine(BaseCombineSet.size());
    return BaseCombineSet;
}

/** Increments the vector nbMsgWronglyConsideredCausalDep while avoiding out of array bounds increments. Raises an error in case of out of array bounds increment.
 * @param entry Entry to increment.*/
void HashErrorDetector::incrementnbMsgWronglyConsideredCausalDep(unsigned int entry)
{
    if(entry < hashStats.nbMsgWronglyConsideredCausalDep.size())
        hashStats.nbMsgWronglyConsideredCausalDep[entry]++;
    else
    {
        cerr<<"INCREMENTS nbMsgWronglyConsideredCausalDep OUT OF BOUND"<<endl;
        throw "INCREMENTS nbMsgWronglyConsideredCausalDep OUT OF BOUND";
    }
}

/** Increments the vector nbMsgToCombine while avoiding out of array bounds increments. Raises an error in case of out of array bounds increment.
 * @param entry Entry to increment.*/
void HashErrorDetector::incrementnbMsgToCombine(unsigned int entry)
{
    if(entry < hashStats.nbMsgToCombine.size())
        hashStats.nbMsgToCombine[entry]++;
    else
    {
        cerr<<"INCREMENTS nbMsgToCombine OUT OF BOUND "<<entry<<endl;
//        throw "INCREMENTS nbMsgToCombine OUT OF BOUND";
    }
}

/** Hashes total dependencies.
 * @param dependencies The total dependencies to hash.
 * @return The hash.*/
size_t HashErrorDetector::hashTotalDependencies(const TotalDependencies& dependencies)
{
    hashStats.nbHash++;
    incrementnbOperationsForHash(dependencies.size());
    return hashDependencies(dependencies.getDependencies());
}

/** Hashes partial dependencies.
 * @param dependencies The partial dependencies to hash.
 * @return The hash.*/
size_t HashErrorDetector::hashPartialDependencies(const PartialDependencies& dependencies)
{
    map<unsigned int, idMsg> tmp = dependencies.getDependencies();
    vector<unsigned int> depvector;
    for(const auto& entry : tmp)
        depvector.push_back(entry.second.seq*entry.second.id); // multiply by id too to decrease collisions

    hashStats.nbHash++;
    incrementnbOperationsForHash(depvector.size());
    return hashDependencies(depvector);
}

/** Increments the vector nbOperationsForHash while avoiding out of array bounds increments.
 * @param entry Entry to increment.*/
void HashErrorDetector::incrementnbOperationsForHash(unsigned int entry)
{
    if(entry < hashStats.nbOperationsForHash.size())
        hashStats.nbOperationsForHash[entry]++;
    else
    {
        hashStats.nbOperationsForHash.resize(entry+1);
        hashStats.nbOperationsForHash[entry]++;
        cerr<<"resize nbOperationsForHash "<<entry<<endl;
//        throw "INCREMENTS nbMsgToCombine OUT OF BOUND";
    }
}

/** Hashes the set of dependencies.
 * @param dependencies The dependencies to hash.
 * @return The hash.*/
size_t HashErrorDetector::hashDependencies(const vector<unsigned int>& dependencies)
{
    size_t entry = hashingF(dependencies);
    if(COLLISION_CONTROL)
    {
        if(collisionController.find(entry)!= collisionController.end())// hash already computed for another message
        {
            if( !(dependencies == collisionController[entry]) )
            {
                cerr<<"collision for hash "<< entry <<endl;
                    //throw "hashTable collision";
            }
        }
        else
            collisionController[entry] = dependencies;
    }
    return entry;
}

/** Hashes a vector.
 * @param vec The vector to hash.
 * @return The hash.*/
size_t HashErrorDetector::hashingF(const vector<uint32_t>& vec){
  size_t seed = vec.size();
  for(auto x : vec) {
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    seed ^= x + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  }
  return seed;
}

/** Prepares the message to broadcast when using hash-based error detectors.
 * @param m The message to broadcast.
 * @param delivered Messages delivered by the broadcasting node, ie dependencies of the message to broadcast.
 * @param clock Probabilistic clock attached to the message.
 * @param processDependencies Dependencies tracker of the broadcasting node.
 * @return The application message to broadcast. */
AppMsg* HashErrorDetector::prepareMessage(AppMsg* m, const vector<messageInfo>& delivered, const ProbabilisticClock& clock, const TotalDependencies& processDependencies)
{
    if(SimulationParameters::depAppended == SimulationParameters::Dependencies::Total)
        m->setHash(hashTotalDependencies(processDependencies));
    else
        m->setHash(hashPartialDependencies(getPartialDependencies(delivered,clock)));

    if(APPENDDEPENDENCIES)
        m->setDependencies(determineAndGetAppendedDependencies(delivered, m->getPC()));
    return m;
}


