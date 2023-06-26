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

#ifndef STATS_STATS_H_
#define STATS_STATS_H_

#include <omnetpp.h>
#include <random>
#include <fstream>

#include "Nodes/NodeWithControl.h"
#include "Nodes/NodeDetector.h"
#include "Nodes/NodeWithRecovery.h"
#include "simulationParameters.h"
#include "structures.h"
using namespace omnetpp;


class NodeBase;


class Stats : public cSimpleModule
{
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);

    vector<NodeBase*> nodes;
    SimulationParameters* params;

  public :
    void writeNbDeliveredMessages();
    void writeNbFalseDeliveredMessages();
    void writeNbSentDependencies();
    void writeControlDataSize();
    void printBuffersStates();
    void writeTrueFalseNegatives();
    void WriteTotalNbHashs();
    void writeNbMsgToCombine();
    void writeNbMsgWronglyConsideredCausalDep();
    void writeDoneCombinationsToFindHash();
    void writeNbOperationsForHashFile();
    void WriteTotalNbRecoveries();
    void writeNbAvoidedRecoveries();
    void printNbMessagesToRecover();
    bool usesHashDetector();

    std::ofstream nbRecoveriesFile;
    std::ofstream nbHashsFile;
    std::ofstream nbMsgToCombineFile;
    std::ofstream nbFalseDeliveredFile;
    std::ofstream nbAvoidedRecoveriesFile;
    std::ofstream nbSentDependenciesFile;
    std::ofstream nbDeliveredMessagesFile;
    std::ofstream controlDataSizeFile;
    std::ofstream nbMsgWronglyConsideredCausalDepFile;
    std::ofstream doneCombinationsToFindHashFile;
    std::ofstream trueFalseNegativesFile;
    std::ofstream nbOperationsForHashFile;
};

#endif /* STATS_STATS_H_ */
