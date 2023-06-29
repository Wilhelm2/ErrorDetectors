/**@file Contains the simulation parameters defined in the .ini file found in the simulations folder
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

#ifndef __DETECTEURSERREURSWITHRECOVERY_SIMULATIONPARAMETERS_H_
#define __DETECTEURSERREURSWITHRECOVERY_SIMULATIONPARAMETERS_H_

#include <omnetpp.h>
#include <vector>
#include <random>
#include <algorithm>
#include "structures.h"

using namespace std;
using namespace omnetpp;

/** @class SimulationParameters
 *  @brief Contains the simulation parameters set in the .ini file found in the simulations folder.
 *  The class reads the simulation parameters at the beginning of the simulation and other classes get access to those parameters through this class.
 */
class SimulationParameters : public cSimpleModule
{
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg) {};
    void initializeDeliveryController();
    void initializeClockEntriesToIncrement();
    vector<vector<unsigned int>> EvenCombinations(unsigned int N, unsigned int k, unsigned int M);
    vector<vector<unsigned int>> computeEntryCombinations(unsigned int M, unsigned int K);

    void computeDependecyCombinationsArray();
    vector<vector<bool>> DependencyCombinations(unsigned int nbPossibleDep);
    vector<vector<bool>> DependencyCombinationsTwo(unsigned int nbPossibleDep);
    vector<vector<bool>> binominalCoefficient(unsigned int N, unsigned int K);

  public:
    simtime_t determineFirstSendTimeInMs(unsigned int id);
    const vector<unsigned int>& getEntriesIncrementedByProcess(unsigned int nodeId);
    const vector<vector<bool>>& getDependencyCombinations(unsigned int nbPossibleDependencies);
    bool usesHashDetector();
    bool usesRecoveries();

    /** Number of nodes the system contains*/
    unsigned int nbNodes;
    /** Clock size that process use to track causality*/
    unsigned int clockLength;
    /** All nodes broadcast a message in delaySend ms*/
    double delaySend;
    /** Number of broadcast peaks per delaySend, i.e. there are PEAKSPERDELAY bell distributions per delaySend*/
    double PEAKSPERDELAY;

    /** Vector containing the set of entries incremented by processes when broadcasting a message. entriesIncrementedByProcess[0] contains the entries p_0 increments when broadcasting a message.*/
    vector<vector<unsigned int>>  entriesIncrementedByProcess;
    /** Vector containing the dependencies to set in the hash computation of hash-based error detectors following the number of messages that are considered of might being causal dependencies of the message to deliver*/
    vector<vector<vector<bool>>> dependencyCombinations;

    /**@def Determines if nodes append the dependencies to the messages they broadcast */
    #define APPENDDEPENDENCIES false
    /**@def Determines if the collision controller is activated. It controls if two different dependencies sets have the same hash. It is set to false by default because collision control uses a lot of memory.*/
    #define COLLISION_CONTROL false
    /** Defines the simulation's type of delivery control*/
    enum class Delivery {
        /** No control, meaning that messages are delivered straight upon reception without any control.*/
        NOTHING,
        /** Uses Probabilistic clocks to causally order messages*/
        ClockComparison,
        /** Uses Probabilistic clocks to causally order messages and executes the error detector of Mostéfaoui on a message $m$'s clock before delivering $m$*/
        Mostefaoui,
        /** Uses Probabilistic clocks to causally order messages and executes the hash-based error detector with the clock difference technique on a message $m$'s clock before delivering $m$*/
        HashClockDifference,
        /** Uses Probabilistic clocks to causally order messages and executes the hash-based error detector with the interval technique on a message $m$'s clock before delivering $m$*/
        HashIntervalDifference,
        /** Uses Probabilistic clocks to causally order messages and executes the hash-based error detector with the clock difference technique on a message $m$'s clock before delivering $m$, and recovers the dependencies of $m$ if the error detector concludes that $m$ might not be causally ordered*/
        Recovery,
        /** Uses Probabilistic clocks to causally order messages and executes the hash-based error detector with the clock difference technique on a message $m$'s clock before delivering $m$, executes the recoveryTest if the error detector concludes that $m$ might not be causally ordered, and recovers the dependencies of $m$ if the recovery test does not succeed in finding a dependencies set for $m$.*/
        RecoveryTest
    };
    /** Determines the type of the simulation's delivery control.*/
    static Delivery DeliveryController;
    /** Defines the type of dependencies used by hash-based error detectors to compute the hash of broadcasted messages.*/
    enum class Dependencies {
        /** A node computes the hash of messages it broadcasts with the full dependency vector of the message to broadcast, ie with a dependency vector containing one entry per process in the system.*/
        Total,
        /** A node computes the hash of messages it broadcasts with only a subset of dependencies of the message to broadcast.*/
        Partial
    };
    /** Determines the type of dependencies used by hash-based error detectors*/
    static Dependencies depAppended;

    /** @def Maximal number of hashes computed by hash-based error detectors when computing hashes for messages*/
    #define LIMIT_HASHS 200 // ATTENTION DOIT ÊTRE LE MÊME POUR LES MSG DANS COMPLETEHASHTEST ET RECOVERYTEST SINON DOIT
                        // AJOUTER ITERATIVEDELIVERY APRÈS RECOVERYMSG.PUSH_BACK DANS RECOVERYTEST SINON PEUT TROUVER
                        // UN ENSEMBLE DE DEPENDANCES QUE N'A PAS TROUVÉ DANS COMLPETEHASHTEST MAIS NE VA JAMAIS LE DÉLIVRER
    /** @def Number of assumed concurrent messages propagating simultaneously inside the system*/
    #define ASSUMED_CONCURRENT_MESSAGES 10
};

#endif
