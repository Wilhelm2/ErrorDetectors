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

bool HashErrorDetector::hashTest(messageInfo message, const vector<messageInfo>& delivered, const vector<unsigned int>& incrementedClockEntries, TotalDependencies processDependencies, Controller* controller, SimulationParameters* params)
{
    if(SimulationParameters::depAppended == SimulationParameters::Dependencies::Total)
        return hashTotalDependencies(message, delivered, incrementedClockEntries, processDependencies, controller, params);
    else
        return hashPartialDependencies(message, delivered, incrementedClockEntries, controller, params);
}

bool HashErrorDetector::hashPartialDependencies(messageInfo message, const vector<messageInfo>& delivered, const vector<unsigned int>& incrementedClockEntries, Controller* controller, SimulationParameters* params)
{
    PartialDependencies baseDependencies;
    unsigned int nbHashs = 0;

    vector<idMsg> possibleDependencies = sortPossibleDependenciesSet(message, createPossibleDependenciesSet(message, delivered, incrementedClockEntries, controller), controller);

    vector<vector<bool>> testedDependencySets = params->getDependencyCombinations(possibleDependencies.size());
    for(vector<bool> isDepConsideredVec : testedDependencySets)
    {
        baseDependencies.clear();
        for(unsigned int i=0; i<isDepConsideredVec.size(); i++)
        {
            if(isDepConsideredVec[i])
                baseDependencies.set(possibleDependencies[i]);
        }
        nbHashs++;
        if(message.hash == hashPartialDependencies(baseDependencies))
        {
            hashStats.doneCombinationsToFindHash[nbHashs]++;
            return true;
        }
    }
    nbHashs++;
    baseDependencies.clear();
    if(message.hash == hashPartialDependencies(baseDependencies))
    {
        hashStats.doneCombinationsToFindHash[nbHashs]++;
        return true;
    }
    return false;
}

bool HashErrorDetector::hashTotalDependencies(messageInfo message, const vector<messageInfo>& delivered, const vector<unsigned int>& incrementedClockEntries, TotalDependencies processDependencies, Controller* controller, SimulationParameters* params)
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
    if(message.hash == hashTotalDependencies(baseDependencies))
    {
        hashStats.doneCombinationsToFindHash[nbHashs]++;
        return true;
    }
    vector<messageInfo> set = createPossibleDependenciesSet(message, delivered, incrementedClockEntries, controller);
    vector<idMsg> possibleDependencies = sortPossibleDependenciesSet(message, set, controller);

    vector<vector<bool>> testedDependencySets = params->getDependencyCombinations(possibleDependencies.size());
    for(vector<bool> isDepConsideredVec : testedDependencySets)
    {
        copieBaseDependencies = baseDependencies;
        for(unsigned int i=0; i<isDepConsideredVec.size(); i++)
        {
            if(isDepConsideredVec[i])
                copieBaseDependencies[possibleDependencies[i].id] = possibleDependencies[i].seq;
        }
        nbHashs++;
        if(message.hash == hashTotalDependencies(copieBaseDependencies))
        {
            hashStats.doneCombinationsToFindHash[nbHashs]++;
            return true;
        }
    }
//    if(compareHashedDepToHash(vModele, message.hash, nbHashs))
  //      return true;
    return false;
}

TotalDependencies HashErrorDetector::createBaseDependencies(messageInfo message, const vector<messageInfo>& delivered, const vector<unsigned int>& incrementedClockEntries, TotalDependencies processDependencies)
{
//    for (unsigned int entry : incrementedClockEntries) commented because interfers with hash computed with clock difference (+small impact)
  //          message.clock.decreaseEntry(entry); // improvment to detect concurrent messages
    TotalDependencies baseDependencies = processDependencies;
    // check for concurrent messages to remove from the base dependencies
    for(messageInfo possibleDep : delivered)
    {
        if(!isPossibleDependency(message, possibleDep) || !isConsideredAsDependency(message, possibleDep)) // message cannot be a dependency or received to recently (in which case it is considered as possible dependency)
            baseDependencies[possibleDep.id.id] = min(possibleDep.id.seq-1, baseDependencies[possibleDep.id.id]);
    }
    return baseDependencies;
}

bool HashErrorDetector::isPossibleDependency(messageInfo message, messageInfo possibleDep)
{
    if(!(possibleDep.clock <= message.clock))
        return false;
    if(APPENDDEPENDENCIES)
    {
        if(isConsideredAsPossibleDependency(message, possibleDep))
        {
            for(idMsg dependency: message.dependencies)
            {
                if(IDMSG_EQ(dependency, possibleDep.id))
                    return true; // received too recently but in dependencies
            }
            return false; // received too recently
        }
    }
    return true;
}

vector<messageInfo> HashErrorDetector::createPossibleDependenciesSet(messageInfo message, const vector<messageInfo>& messageToChooseFrom, const vector<unsigned int>& incrementedClockEntries, Controller* controller)
{
//    for (unsigned int entry : incrementedClockEntries) commented because interfers with hash computed with clock difference (+small impact)
  //      message.clock.decreaseEntry(entry); // Improvement to detect concurrent messages

    vector<messageInfo> BaseCombineSet;
    unsigned int falseDetected = 0;

    for(messageInfo possibleDep : messageToChooseFrom)
    {
        if(isPossibleDependency(message, possibleDep) && isConsideredAsPossibleDependency(message, possibleDep))
        {
            BaseCombineSet.push_back(possibleDep);
            if(!controller->isDependency(message.id, possibleDep.id))
            {
                falseDetected ++;
            }
        }
    }

    incrementnbMsgWronglyConsideredCausalDep(falseDetected);
    incrementnbMsgToCombine(BaseCombineSet.size());
    return BaseCombineSet;
}

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

size_t HashErrorDetector::hashTotalDependencies(TotalDependencies dependencies)
{
    hashStats.nbHash++;
    incrementnbOperationsForHash(dependencies.getDependencies().size());
    return hashDependencies(dependencies.getDependencies());
}

size_t HashErrorDetector::hashPartialDependencies(const PartialDependencies& dependencies)
{
    map<unsigned int, idMsg> tmp = dependencies.getDependencies();
    vector<unsigned int> depvector;
    for(auto entry : tmp)
        depvector.push_back(entry.second.seq*entry.second.id); // multiply by id too to decrease collisions

    hashStats.nbHash++;
    incrementnbOperationsForHash(depvector.size());
    return hashDependencies(depvector);
}

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

AppMsg* HashErrorDetector::prepareMessage(AppMsg* m, const vector<messageInfo>& delivered, const ProbabilisticClock& clock, const vector<unsigned int>& processDependencies)
{
    if(SimulationParameters::depAppended == SimulationParameters::Dependencies::Total)
        m->setHash(hashTotalDependencies(processDependencies));
    else
        m->setHash(hashPartialDependencies(getPartialDependencies(delivered,clock)));

    if(APPENDDEPENDENCIES)
        m->setDependencies(determineAndGetAppendedDependencies(delivered)); //, m->getPC()));
    return m;
}

