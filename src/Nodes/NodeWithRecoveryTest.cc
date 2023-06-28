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

#include "NodeWithRecoveryTest.h"

Define_Module(NodeWithRecoveryTest);

NodeWithRecoveryTest::NodeWithRecoveryTest() {
    // TODO Auto-generated constructor stub

}

NodeWithRecoveryTest::~NodeWithRecoveryTest() {
    // TODO Auto-generated destructor stub
}

bool NodeWithRecoveryTest::testDeliverMessage(const messageInfo& m)
{
    if(clock.satisfiesDeliveryCondition(m.clock, params->getEntriesIncrementedByProcess(m.id.id)))
    {
        if(detector->test(m, getIndexIncrementedEntries(m.id.id), clock, deliveredMessagesTracker, control, params, delivered))
            deliverMsg(m);
        else
        {
            if(control->canCausallyDeliverMessage(m.id, id))
                detector->stats.falseNegative++;
            else
                detector->stats.trueNegative++;

            if(inRecovery)
                messagesToRecover.push_back(m);
            else
            {
                if(recoveryTest(m, delivered, params->entriesIncrementedByProcess[m.id.id%params->nbCombinations]))
                {
                    requestDependencies(m);
//                    cerr<<"node " << id << " can causally deliver message " << m.id.id << "," << m.id.seq<< " "<< control->canCausallyDeliverMessage(m.id, id)<<endl;
                }
                else
                    statsRecovery.nbAvoidedRecoveries++;
            }
        }
        return true;
    }
    return false;
}

// begins recovery if returns true. Returns false if all dependencies were received but not delivered yet (because of current recovery or clock delivery conditions not satisfied yet)
bool NodeWithRecoveryTest::recoveryTest(const messageInfo& message, const vector<messageInfo>& delivered, const vector<unsigned int>& incrementedClockEntries)
{
    HashErrorDetector* detector = dynamic_cast<HashErrorDetector*> (this->detector);
    TotalDependencies baseDependencies = detector->createBaseDependencies(message, delivered, incrementedClockEntries, deliveredMessagesTracker);
    unsigned int nbHashs = 0;

    for(const idMsg& t: message.dependencies)
    {
        if(baseDependencies[t.id] < t.seq)
            baseDependencies[t.id] = t.seq;
    }
    nbHashs++;
    if(tryHash(message, baseDependencies, nbHashs))
        return false;

    TotalDependencies tmpBaseDependencies = baseDependencies;
    vector<idMsg> messagesToCombine = detector->sortPossibleDependenciesSet(message, detector->createPossibleDependenciesSet(message, delivered, incrementedClockEntries, control), control);
    vector<idMsg> messagesToCombineRecovery = createRecoveredMessagesToCombine(message, incrementedClockEntries);
    vector<vector<bool>> testedDependencySets = params->getDependencyCombinations(messagesToCombine.size());
    for(const vector<bool>& isDepConsideredVec : testedDependencySets)
    {
        tmpBaseDependencies = baseDependencies;
        for(unsigned int i=0; i<isDepConsideredVec.size(); i++)
        {
            if(isDepConsideredVec[i])
                tmpBaseDependencies[messagesToCombine[i].id] = messagesToCombine[i].seq;
        }
        nbHashs++;
        if(tryHash(message, tmpBaseDependencies, nbHashs))
            return false;

        // tests with taking into account messages in recovery
        if(!TryCombinationsRecoveringMessages(message, nbHashs, tmpBaseDependencies, messagesToCombineRecovery))
            return false;
    }

    // test by taking into account messages in recovery
    if(!TryCombinationsRecoveringMessages(message, nbHashs, baseDependencies, messagesToCombineRecovery))
        return false;
    return true;
}

bool NodeWithRecoveryTest::tryHash(const messageInfo& message, const TotalDependencies& dependencies, unsigned doneHashes)
{
    HashErrorDetector* detector = dynamic_cast<HashErrorDetector*> (this->detector);
    detector->hashStats.nbHash++;
    if(message.hash == detector->hashTotalDependencies(dependencies))
    {
        detector->hashStats.doneCombinationsToFindHash[doneHashes]++;
        recoveredMessages.push_back({message, dependencies});
        return true;
    }
    return false;
}


// tests with messages in recovery
bool NodeWithRecoveryTest::TryCombinationsRecoveringMessages(const messageInfo& message, unsigned int& nbHashs, const TotalDependencies& baseDependencies, const vector<idMsg>& messagesToCombine)
{
    TotalDependencies tmpBaseDependencies;
    vector<vector<bool>> testedDependencySets = params->getDependencyCombinations(messagesToCombine.size());
    for(const vector<bool>& isDepConsideredVec : testedDependencySets)
    {
        tmpBaseDependencies = baseDependencies;
        for(unsigned int i=0; i<isDepConsideredVec.size(); i++)
        {
            if(isDepConsideredVec[i])
                tmpBaseDependencies[messagesToCombine[i].id] = messagesToCombine[i].seq;
        }
        nbHashs++;
        if(tryHash(message, tmpBaseDependencies, nbHashs))
            return false;
    }
    return true;
}

vector<idMsg> NodeWithRecoveryTest::createRecoveredMessagesToCombine(const messageInfo& message, const vector<unsigned int>& incrementedClockEntries)
{
    vector<messageInfo> recoveredTmp;
    HashErrorDetector* detector = dynamic_cast<HashErrorDetector*> (this->detector);

    for(const recoveredMessage& tmp : recoveredMessages)
        recoveredTmp.push_back(tmp.msg);
    if(inRecovery)
        recoveredTmp.push_back(currRecovery);

    vector<idMsg> recoveredMessagesToCombine = detector->sortPossibleDependenciesSet(message, detector->createPossibleDependenciesSet(message, recoveredTmp, incrementedClockEntries, control), control);
    vector<idMsg> messagesToRecoverToCombine = detector->sortPossibleDependenciesSet(message, detector->createPossibleDependenciesSet(message, messagesToRecover, incrementedClockEntries, control), control);
    recoveredMessagesToCombine.insert(recoveredMessagesToCombine.end(), messagesToRecoverToCombine.begin(), messagesToRecoverToCombine.end());
    return recoveredMessagesToCombine;
}

