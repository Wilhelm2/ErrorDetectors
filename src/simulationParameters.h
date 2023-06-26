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

#ifndef __DETECTEURSERREURSWITHRECOVERY_SIMULATIONPARAMETERS_H_
#define __DETECTEURSERREURSWITHRECOVERY_SIMULATIONPARAMETERS_H_

#include <omnetpp.h>
#include <vector>
#include <random>
#include <algorithm>
#include "structures.h"

using namespace std;
using namespace omnetpp;

/**
 * TODO - Generated class
 */
class SimulationParameters : public cSimpleModule
{
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    void initializeClockEntriesToIncrement();
    vector<vector<unsigned int>> EvenCombinations(unsigned int N, unsigned int k, unsigned int M);
    vector<vector<unsigned int>> computeEntryCombinations(unsigned int N, unsigned int K);

    void computeDependecyCombinationsArray();
    vector<vector<bool>> DependencyCombinations(unsigned int nbPossibleDep);
    vector<vector<bool>> DependencyCombinationsTwo(unsigned int nbPossibleDep);
    vector<vector<bool>> binominalCoefficient(unsigned int N, unsigned int K);


  public:
    simtime_t determineFirstSendTimeInMs(unsigned int id);
    const vector<unsigned int>& getEntriesIncrementedByProcess(unsigned int processId);
    const vector<vector<bool>>& getDependencyCombinations(unsigned int nbPossibleDependencies);
    unsigned int nbNodes;
    unsigned int clockLength;
    double PEAKSPERDELAY;
    double delaySend;

    vector<vector<unsigned int>>  entriesIncrementedByProcess;
    unsigned int nbCombinations;
    vector<vector<vector<bool>>> dependencyCombinations;

    #define APPENDDEPENDENCIES false
    #define COLLISION_CONTROL false
    enum class Delivery {NOTHING, ClockComparison, Mostefaoui, HashClockDifference, HashIntervalDifference};
    static Delivery DeliveryController;
    enum class Dependencies {Total, Partial};
    static Dependencies depAppended;
    bool recovering;


    #define LIMIT_HASHS 200 // ATTENTION DOIT ÊTRE LE MÊME POUR LES MSG DANS COMPLETEHASHTEST ET RECOVERYTEST SINON DOIT
                        // AJOUTER ITERATIVEDELIVERY APRÈS RECOVERYMSG.PUSH_BACK DANS RECOVERYTEST SINON PEUT TROUVER
                        // UN ENSEMBLE DE DEPENDANCES QUE N'A PAS TROUVÉ DANS COMLPETEHASHTEST MAIS NE VA JAMAIS LE DÉLIVRER
    #define ASSUMED_CONCURRENT_MESSAGES 10

};

#endif
