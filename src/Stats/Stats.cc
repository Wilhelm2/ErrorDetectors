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
        nodes.push_back( dynamic_cast<Node*> (getModuleByPath(tmp)));
    }

    scheduleAt(simTime()+SimTime(1, SIMTIME_S), new cMessage());

    nbRecoveriesFile.open("data/nbRecoveriesFile.dat",std::ios::out);
    nbHashsFile.open("data/nbHashsFile.dat",std::ios::out);
    nbMsgToCombineFile.open("data/nbMsgToCombineFile.dat",std::ios::out);
    nbFalseDeliveredFile.open("data/nbFalseDeliveredFile.dat",std::ios::out);
    nbAvoidedRecoveriesFile.open("data/nbAvoidedRecoveriesFile.dat",std::ios::out);
    nbSentDependenciesFile.open("data/nbSentDependenciesFile.dat",std::ios::out);
    nbDeliveredMessagesFile.open("data/nbDeliveredMessagesFile.dat",std::ios::out);
    controlDataSizeFile.open("data/controlDataSizeFile.dat",std::ios::out);
    trueFalseNegativesFile.open("data/trueFalseNegativesFile.dat",std::ios::out);
    doneCombinationsToFindHashFile.open("data/doneCombinationsToFindHashFile.dat",std::ios::out);
    nbMsgWronglyConsideredCausalDepFile.open("data/nbMsgWronglyConsideredCausalDepFile.dat",std::ios::out);
    nbOperationsForHashFile.open("data/nbOperationsForHashFile.dat",std::ios::out);
}





void Stats::WriteTotalNbHashs()
{
    unsigned int totalNbHashs = 0;
    unsigned int totalNbDelivered = 0;
    for(Node* node: nodes)
    {
        totalNbHashs += node->detector.stats.nbHash;
        totalNbDelivered += node->stats.nbDeliveredMessages;
        nbHashsFile << node->detector.stats.nbHash << " ";
    }
    nbHashsFile << endl;
    nbHashsFile << ((float)totalNbHashs)/totalNbDelivered<<endl;
}

void Stats::writeNbMsgToCombine()
{
    vector<unsigned int> nbMsgToCombine;
    nbMsgToCombine.resize(nodes[0]->detector.stats.nbMsgToCombine.size(),0);

    for(Node* n : nodes)
    {
        for(unsigned int i=0; i<nbMsgToCombine.size(); i++)
            nbMsgToCombine[i] = n->detector.stats.nbMsgToCombine[i];
    }
    for(unsigned int i: nbMsgToCombine)
        nbMsgToCombineFile << i << " ";
}

void Stats::writeNbFalseDeliveredMessages()
{
    unsigned int nbFalseDeliveredMessages = 0;
    cerr<<"Number of False delivered messages"<<endl;
    for(Node* n : nodes)
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
    for(Node* n : nodes)
        nbSentDependencies += n->stats.nbSentDependencies;
    nbSentDependenciesFile << nbSentDependencies<<endl;
    cerr << "Number of sent dependencies " << nbSentDependencies << endl;
}

void Stats::writeNbDeliveredMessages()
{
    unsigned int nbDeliveredMessages = 0;
    cerr<< "Number of delivered messages by each process :"<<endl;
    for(Node* n : nodes)
    {
        nbDeliveredMessages += n->stats.nbDeliveredMessages;
        cerr<<n->stats.nbDeliveredMessages<< " ";
    }
    cerr<<endl;
    nbDeliveredMessagesFile << nbDeliveredMessages<<endl;
}

void Stats::writeControlDataSize()
{
    unsigned int controlDataSize = 0;
    for(Node* n : nodes)
        controlDataSize += n->stats.controlDataSize;
    controlDataSizeFile << controlDataSize<<endl;
}

void Stats::writeNbMsgWronglyConsideredCausalDep()
{
    vector<unsigned int> nbMsgWronglyConsideredCausalDep;
    nbMsgWronglyConsideredCausalDep.resize(nodes[0]->detector.stats.nbMsgWronglyConsideredCausalDep.size(),0);

    for(Node* n : nodes)
    {
        for(unsigned int i=0; i<nbMsgWronglyConsideredCausalDep.size(); i++)
            nbMsgWronglyConsideredCausalDep[i] += n->detector.stats.nbMsgWronglyConsideredCausalDep[i];
    }
    for(unsigned int i: nbMsgWronglyConsideredCausalDep)
        nbMsgWronglyConsideredCausalDepFile << i << " ";
}

void Stats::writeDoneCombinationsToFindHash()
{
    vector<unsigned int> doneCombinationsToFindHash;
    doneCombinationsToFindHash.resize(nodes[0]->detector.stats.doneCombinationsToFindHash.size(),0);

    for(Node* n : nodes)
    {
        for(unsigned int i=0; i<doneCombinationsToFindHash.size(); i++)
            doneCombinationsToFindHash[i] += n->detector.stats.doneCombinationsToFindHash[i];
    }
    for(unsigned int i: doneCombinationsToFindHash)
        doneCombinationsToFindHashFile << i << " ";
    doneCombinationsToFindHashFile << endl;
}

void Stats::writeTrueFalseNegatives()
{
    unsigned int falseNegative = 0;
    unsigned int trueNegative = 0;
    for(Node* n : nodes)
    {
        falseNegative += n->detector.stats.falseNegative;
        trueNegative += n->detector.stats.trueNegative;
    }
    trueFalseNegativesFile << falseNegative << " " << trueNegative <<endl;
}


void Stats::writeNbOperationsForHashFile()
{
    vector<unsigned int> nbOperationsForHash(nodes[0]->detector.stats.nbOperationsForHash.size(),0);
    unsigned int nbOperations = 0;
    unsigned int totalComputedHashes = 1;
    for(Node* n : nodes)
    {
        for(unsigned int i=0; i<nbOperationsForHash.size(); i++)
        {
            nbOperationsForHash[i] += n->detector.stats.nbOperationsForHash[i];
            nbOperations += i*n->detector.stats.nbOperationsForHash[i];
        }
        totalComputedHashes+= n->detector.stats.nbHash;
    }
    for(unsigned int i: nbOperationsForHash)
        nbOperationsForHashFile << i << " ";
    nbOperationsForHashFile << endl;
    nbOperationsForHashFile << ((float) nbOperations / totalComputedHashes)<<endl;
}


void Stats::printBuffersStates()
{
    cerr<< "Number of messages in pendingApp for each process :"<<endl;
    for(Node* n : nodes)
        cerr<<n->pendingMsg.size()<< " ";
    cerr<<endl;
}

void Stats::handleMessage(cMessage *msg)
{

    std::cerr << "Simulation time: " << simTime()<<endl;
    scheduleAt(simTime()+ SimTime(1, SIMTIME_S), msg);

    WriteTotalNbHashs();
    writeNbMsgToCombine();
    writeNbFalseDeliveredMessages();
    writeNbSentDependencies();
    writeNbDeliveredMessages();
    writeControlDataSize();
    writeNbMsgWronglyConsideredCausalDep();
    writeTrueFalseNegatives();
    writeDoneCombinationsToFindHash();
    writeNbOperationsForHashFile();

    printBuffersStates();
    if(simTime()==100)
        exit(0);
}
