/** @file Stats.h
 *  @brief Stats module used to collect statistics and write them into files
 */
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

/** @class Stats
 *  @brief Stats is used to collect statistics and write them into files
 */
class Stats : public cSimpleModule
{
  private:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);

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
    void printNbJumpedBroadcasts();
    bool usesHashDetector();

    /** Stream to write into a file the statistics about the number of done message recoveries*/
    std::ofstream nbRecoveriesFile;
    /** Stream to write into a file the statistics about the number of computed hashes*/
    std::ofstream nbHashsFile;
    /** Stream to write into a file the statistics about the number of messages to combine in the hash combination*/
    std::ofstream nbMsgToCombineFile;
    /** Stream to write into a file the statistics about the number of messages delivered out of causal order*/
    std::ofstream nbFalseDeliveredFile;
    /** Stream to write into a file the statistics about */
    std::ofstream nbAvoidedRecoveriesFile;
    /** Stream to write into a file the statistics about */
    std::ofstream nbSentDependenciesFile;
    /** Stream to write into a file the statistics about */
    std::ofstream nbDeliveredMessagesFile;
    /** Stream to write into a file the statistics about */
    std::ofstream controlDataSizeFile;
    /** Stream to write into a file the statistics about */
    std::ofstream nbMsgWronglyConsideredCausalDepFile;
    /** Stream to write into a file the statistics about */
    std::ofstream doneCombinationsToFindHashFile;
    /** Stream to write into a file the statistics about */
    std::ofstream trueFalseNegativesFile;
    /** Stream to write into a file the statistics about */
    std::ofstream nbOperationsForHashFile;

    vector<NodeBase*> nodes;
    SimulationParameters* params;

};

#endif /* STATS_STATS_H_ */
