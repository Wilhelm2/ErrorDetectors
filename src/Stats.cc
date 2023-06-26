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

#include "Stats.h"

Define_Module(Stats);

void Stats::initialize()
{
    char tmp[40];
    params =  dynamic_cast<SimulationParameters*> (getModuleByPath((string(PACKAGENAME)+".simParams").c_str()));

    for(unsigned int i=0;i<params->nbNodes;i++)
    {
        sprintf(tmp, (string(PACKAGENAME)+".Nodes[%d]").c_str(),i);
        nodes.push_back( dynamic_cast<NodeBase*> (getModuleByPath(tmp)));
    }

    scheduleAt(simTime()+SimTime(1, SIMTIME_S), new cMessage());

    nbDeliveredMessagesFile.open("data/nbDeliveredMessagesFile.dat",std::ios::out);
    nbFalseDeliveredFile.open("data/nbFalseDeliveredFile.dat",std::ios::out);

    if(params->DeliveryController == SimulationParameters::Delivery::Mostefaoui || usesHashDetector())
        trueFalseNegativesFile.open("data/trueFalseNegativesFile.dat",std::ios::out);
    if(usesHashDetector())
    {
        nbSentDependenciesFile.open("data/nbSentDependenciesFile.dat",std::ios::out);
        controlDataSizeFile.open("data/controlDataSizeFile.dat",std::ios::out);
        nbHashsFile.open("data/nbHashsFile.dat",std::ios::out);
        nbMsgToCombineFile.open("data/nbMsgToCombineFile.dat",std::ios::out);
        nbMsgWronglyConsideredCausalDepFile.open("data/nbMsgWronglyConsideredCausalDepFile.dat",std::ios::out);
        doneCombinationsToFindHashFile.open("data/doneCombinationsToFindHashFile.dat",std::ios::out);
        nbOperationsForHashFile.open("data/nbOperationsForHashFile.dat",std::ios::out);
    }
    if(params->recovering)
    {
        nbRecoveriesFile.open("data/nbRecoveriesFile.dat",std::ios::out);
        nbAvoidedRecoveriesFile.open("data/nbAvoidedRecoveriesFile.dat",std::ios::out);
    }

}

void Stats::handleMessage(cMessage *msg)
{
    std::cerr << "Simulation time: " << simTime()<<endl;
    scheduleAt(simTime()+ SimTime(1, SIMTIME_S), msg);

    writeNbDeliveredMessages();
    writeNbFalseDeliveredMessages();
    writeNbSentDependencies();
    writeControlDataSize();

    cerr<<"value of delivery " << (int)params->DeliveryController<<endl;
    if(params->DeliveryController != SimulationParameters::Delivery::NOTHING)
    {
        printBuffersStates();
    }
    if(params->DeliveryController == SimulationParameters::Delivery::Mostefaoui || usesHashDetector())
    {
        writeTrueFalseNegatives();
    }
    if(usesHashDetector())
    {
        WriteTotalNbHashs();
        writeNbMsgToCombine();
        writeNbMsgWronglyConsideredCausalDep();
        writeDoneCombinationsToFindHash();
        writeNbOperationsForHashFile();
    }
    if(params->recovering)
    {
        WriteTotalNbRecoveries();
        writeNbAvoidedRecoveries();
        printNbMessagesToRecover();
    }

    if(simTime()==100)
        exit(0);
}

void Stats::writeNbDeliveredMessages()
{
    unsigned int nbDeliveredMessages = 0;
    cerr<< "Number of delivered messages by each process :"<<endl;
    for(NodeBase* n : nodes)
    {
        nbDeliveredMessages += n->stats.nbDeliveredMessages;
        cerr<<n->stats.nbDeliveredMessages<< " ";
    }
    cerr<<endl;
    nbDeliveredMessagesFile << nbDeliveredMessages<<endl;
}

void Stats::writeNbFalseDeliveredMessages()
{
    unsigned int nbFalseDeliveredMessages = 0;
    cerr<<"Number of False delivered messages"<<endl;
    for(NodeBase* n : nodes)
    {
        nbFalseDeliveredMessages += n->stats.nbFalseDeliveredMessages;
        cerr<<n->stats.nbFalseDeliveredMessages<<" ";
    }
    cerr<<endl;
    nbFalseDeliveredFile << nbFalseDeliveredMessages<<endl;
}

void Stats::writeNbSentDependencies()
{
    unsigned int nbSentDependencies = 0;
    for(NodeBase* n : nodes)
        nbSentDependencies += n->stats.nbSentDependencies;
    nbSentDependenciesFile << nbSentDependencies<<endl;
    cerr << "Number of sent dependencies " << nbSentDependencies << endl;
}

void Stats::writeControlDataSize()
{
    unsigned int controlDataSize = 0;
    for(NodeBase* n : nodes)
        controlDataSize += n->stats.controlDataSize;
    controlDataSizeFile << controlDataSize<<endl;
}

void Stats::printBuffersStates()
{
    cerr<< "Number of messages in pendingApp for each process :"<<endl;
    for(NodeBase* n : nodes)
    {
        NodeWithControl* nc = dynamic_cast<NodeWithControl*>(n);
        cerr<<nc->pendingMsg.size()<< " ";
    }
    cerr<<endl;
}

void Stats::writeTrueFalseNegatives()
{
    unsigned int falseNegative = 0;
    unsigned int trueNegative = 0;
    for(NodeBase* n : nodes)
    {
        NodeDetector* nd = dynamic_cast<NodeDetector*>(n);
        falseNegative += nd->detector->stats.falseNegative;
        trueNegative += nd->detector->stats.trueNegative;
    }
    trueFalseNegativesFile << falseNegative << " " << trueNegative <<endl;
}

void Stats::WriteTotalNbHashs()
{
    unsigned int totalNbHashs = 0;
    unsigned int totalNbDelivered = 0;
    for(NodeBase* node: nodes)
    {
        NodeDetector* nd = dynamic_cast<NodeDetector*>(node);
        HashErrorDetector* detector = dynamic_cast<HashErrorDetector*>(nd->detector);
        totalNbHashs += detector->hashStats.nbHash;
        totalNbDelivered += node->stats.nbDeliveredMessages;
        nbHashsFile << detector->hashStats.nbHash << " ";
    }
    nbHashsFile << endl;
    nbHashsFile << ((float)totalNbHashs)/totalNbDelivered<<endl;
}

void Stats::writeNbMsgToCombine()
{
    vector<unsigned int> nbMsgToCombine;
    NodeDetector* nd = dynamic_cast<NodeDetector*>(nodes[0]);
    HashErrorDetector* detector = dynamic_cast<HashErrorDetector*>(nd->detector);
    nbMsgToCombine.resize(detector->hashStats.nbMsgToCombine.size(),0);

    for(NodeBase* n : nodes)
    {
        nd = dynamic_cast<NodeDetector*>(n);
        detector = dynamic_cast<HashErrorDetector*>(nd->detector);
        for(unsigned int i=0; i<nbMsgToCombine.size(); i++)
            nbMsgToCombine[i] = detector->hashStats.nbMsgToCombine[i];
    }
    for(unsigned int i: nbMsgToCombine)
        nbMsgToCombineFile << i << " ";
}

void Stats::writeNbMsgWronglyConsideredCausalDep()
{
    vector<unsigned int> nbMsgWronglyConsideredCausalDep;
    NodeDetector* nd = dynamic_cast<NodeDetector*>(nodes[0]);
    HashErrorDetector* detector = dynamic_cast<HashErrorDetector*>(nd->detector);
    nbMsgWronglyConsideredCausalDep.resize(detector->hashStats.nbMsgWronglyConsideredCausalDep.size(),0);

    for(NodeBase* n : nodes)
    {
        nd = dynamic_cast<NodeDetector*>(n);
        detector = dynamic_cast<HashErrorDetector*>(nd->detector);
        for(unsigned int i=0; i<nbMsgWronglyConsideredCausalDep.size(); i++)
            nbMsgWronglyConsideredCausalDep[i] += detector->hashStats.nbMsgWronglyConsideredCausalDep[i];
    }
    for(unsigned int i: nbMsgWronglyConsideredCausalDep)
        nbMsgWronglyConsideredCausalDepFile << i << " ";
}

void Stats::writeDoneCombinationsToFindHash()
{
    vector<unsigned int> doneCombinationsToFindHash;
    NodeDetector* nd = dynamic_cast<NodeDetector*>(nodes[0]);
    HashErrorDetector* detector = dynamic_cast<HashErrorDetector*>(nd->detector);
    doneCombinationsToFindHash.resize(detector->hashStats.doneCombinationsToFindHash.size(),0);

    for(NodeBase* n : nodes)
    {
        nd = dynamic_cast<NodeDetector*>(n);
        detector = dynamic_cast<HashErrorDetector*>(nd->detector);
        for(unsigned int i=0; i<doneCombinationsToFindHash.size(); i++)
            doneCombinationsToFindHash[i] += detector->hashStats.doneCombinationsToFindHash[i];
    }
    for(unsigned int i: doneCombinationsToFindHash)
        doneCombinationsToFindHashFile << i << " ";
    doneCombinationsToFindHashFile << endl;
}

void Stats::writeNbOperationsForHashFile()
{
    NodeDetector* nd = dynamic_cast<NodeDetector*>(nodes[0]);
    HashErrorDetector* detector = dynamic_cast<HashErrorDetector*>(nd->detector);
    vector<unsigned int> nbOperationsForHash(detector->hashStats.nbOperationsForHash.size(),0);
    unsigned int nbOperations = 0;
    unsigned int totalComputedHashes = 1;
    for(NodeBase* n : nodes)
    {
        nd = dynamic_cast<NodeDetector*>(n);
        detector = dynamic_cast<HashErrorDetector*>(nd->detector);
        for(unsigned int i=0; i<nbOperationsForHash.size(); i++)
        {
            nbOperationsForHash[i] += detector->hashStats.nbOperationsForHash[i];
            nbOperations += i*detector->hashStats.nbOperationsForHash[i];
        }
        totalComputedHashes+= detector->hashStats.nbHash;
    }
    for(unsigned int i: nbOperationsForHash)
        nbOperationsForHashFile << i << " ";
    nbOperationsForHashFile << endl;
    nbOperationsForHashFile << ((float) nbOperations / totalComputedHashes)<<endl;
}

void Stats::WriteTotalNbRecoveries()
{
    unsigned int totalNbRecoveries = 0;
    unsigned int totalNbDeliveries = 0;
    for(NodeBase* node: nodes)
    {
        NodeWithRecovery* nr = dynamic_cast<NodeWithRecovery*>(node);
        totalNbRecoveries += nr->statsRecovery.nbRecoveries;
        totalNbDeliveries += nr->stats.nbDeliveredMessages;
        nbRecoveriesFile << nr->statsRecovery.nbRecoveries << " ";
    }
    nbRecoveriesFile<<endl;
    nbRecoveriesFile<< totalNbRecoveries <<endl;
    nbRecoveriesFile << ((float)totalNbRecoveries) / totalNbDeliveries << endl;
    cerr << "Number of recoveries " << totalNbRecoveries <<endl;
    cerr << "Rate of recoveries " << ((float)totalNbRecoveries) / totalNbDeliveries << endl;
}

void Stats::writeNbAvoidedRecoveries()
{
    unsigned int nbAvoidedRecoveries = 0;
    unsigned int totalNbDeliveries = 0;
    for(NodeBase* n : nodes)
    {
        NodeWithRecovery* nr = dynamic_cast<NodeWithRecovery*>(n);
        nbAvoidedRecoveries += nr->statsRecovery.nbAvoidedRecoveries;
        totalNbDeliveries += nr->stats.nbDeliveredMessages;
    }
    nbAvoidedRecoveriesFile << nbAvoidedRecoveries << " " << ((float)nbAvoidedRecoveries)/totalNbDeliveries<<endl;
}

void Stats::printNbMessagesToRecover()
{

    cerr<< "Number of messages to recover for each process :"<<endl;
    for(NodeBase* n : nodes)
    {
        NodeWithRecovery* nr = dynamic_cast<NodeWithRecovery*>(n);
        cerr<<nr->messagesToRecover.size()<< " ";
    }
    cerr<<endl;
}

bool Stats::usesHashDetector()
{
    return params->DeliveryController == SimulationParameters::Delivery::HashClockDifference ||params->DeliveryController == SimulationParameters::Delivery::HashIntervalDifference;
}
