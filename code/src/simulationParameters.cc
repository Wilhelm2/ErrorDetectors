/** @file*/
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

#include "simulationParameters.h"

Define_Module(SimulationParameters);
SimulationParameters::Delivery SimulationParameters::DeliveryController =
    SimulationParameters::Delivery::HashClockDifference;
SimulationParameters::Dependencies SimulationParameters::depAppended = SimulationParameters::Dependencies::Partial;

/** Initializes Stats by reading the simulation parameters, and by computing the dependencies to increment as well as
 * the dependencies sets to test
 */
void SimulationParameters::initialize() {
    cSimpleModule::initialize();
    nbNodes = par("nbNodes");
    PEAKSPERDELAY = par("PEAKSPERDELAY");
    delaySend = par("delaySend");
    clockLength = par("clockLength");
    vector<vector<unsigned int>> entriesIncrementedByProcess;
    //    recovering = par("recovering");

    initializeDeliveryController();
    initializeClockEntriesToIncrement();
    computeDependecyCombinationsArray();
}

/** Converts the deliveryOption simulation parameter into the SimulationParameters::Delivery enum type.
 */
void SimulationParameters::initializeDeliveryController() {
    unsigned int deliveryOption = par("deliveryOption");
    if (deliveryOption == 1)
        DeliveryController = SimulationParameters::Delivery::NOTHING;
    else if (deliveryOption == 2)
        DeliveryController = SimulationParameters::Delivery::ClockComparison;
    else if (deliveryOption == 3)
        DeliveryController = SimulationParameters::Delivery::Mostefaoui;
    else if (deliveryOption == 5)
        DeliveryController = SimulationParameters::Delivery::HashIntervalDifference;
    else
        DeliveryController = SimulationParameters::Delivery::HashClockDifference;
}

/** Uses the formula given by Mostéfaoui to determine the number of entries $k$ a process increments when broadcasting a
 * message. Then builds vectors of $k$ random entries to increment and shuffles them to have a more random distribution.
 */
void SimulationParameters::initializeClockEntriesToIncrement() {
    // use following formula to compute the number of entries incremented by each process ln(2)*R/X with R:clocksize,
    // X:nbConcurrentMessages X = nbMessagesPerSecond * channelDelay
    float messagesPerSecond = nbNodes / delaySend;
    cerr << "Messagespersecond " << messagesPerSecond << endl;
    float channelDelay = CHANNELDELAY / 1000000;
    std::cerr << "channelDelay " << channelDelay << endl;
    int k = min(
        4., ceil(std::log1p(1) * clockLength /
                 (messagesPerSecond *
                  channelDelay)));  // limite à x sinon l'ensemble des possibilités calculés dans comb sera trop grand
    std::cerr << "Number of entries to increment " << k << endl;
    //    entriesIncrementedByProcess = computeEntryCombinations(clockLength, k);
    entriesIncrementedByProcess = EvenCombinations(nbNodes, k, clockLength);

    cerr << "Number of combinations of incrementEntry sets  " << entriesIncrementedByProcess.size() << endl;
    std::shuffle(entriesIncrementedByProcess.begin(), entriesIncrementedByProcess.end(), std::default_random_engine{});
    for (unsigned int i = 0; i < nbNodes; i++) {
        cerr << "Node " << i << " increments entries ";
        for (unsigned int j : entriesIncrementedByProcess[i % entriesIncrementedByProcess.size()])
            cerr << j << " ";
        cerr << endl;
    }
}

/** Builds vectors of $k$ entries to increment when broadcasting a message. Verifies that the entries to increment are
 * distributed almost evenly, ie that entry 0 of the probabilistic clock is contained in as much vectors as entry 1,
 * etc.
 * @param N Number of nodes inside the system.
 * @param k Number of entries to increment when broadcasting a message.
 * @param M Size of the probabilistic clock.
 * @return Vector containing sets of PC entry indexes to increment when broadcasting a message.
 */
vector<vector<unsigned int>> SimulationParameters::EvenCombinations(unsigned int N, unsigned int k, unsigned int M) {
    unsigned int nbIncrements = N * k / M + 1;  // number of times that each entry should be affected at maximum to a
                                                // process (added +1 so that if nbIncrements = x.y then will not block)
    vector<unsigned int> vectorIncr;
    vectorIncr.resize(M, 0);
    vector<vector<unsigned int>> combinations;
    unsigned int entry;
    for (unsigned int i = 0; i < N; i++)  // for each process
    {
        vector<unsigned int> res;
        for (unsigned int j = 0; j < k; j++)  // for each entry
        {
            entry = rand() % M;
            while (std::find(res.begin(), res.end(), entry) != res.end() || vectorIncr[entry] >= nbIncrements)
                entry = (entry + 1) % M;
            res.push_back(entry);
            vectorIncr[entry]++;
        }
        combinations.push_back(res);
    }
    std::shuffle(combinations.begin(), combinations.end(), std::default_random_engine{});
    return combinations;
}

/** Builds vectors of $k$ entries to increment when broadcasting a message. This function does not ensure an even
 * distribution.
 *  @param M Size of the Probabilistic clock.
 *  @param K Number of entries nodes increment when broadcasting a message.
 *  @return Vector containing sets of PC entry indexes to increment when broadcasting a message.
 */
vector<vector<unsigned int>> SimulationParameters::computeEntryCombinations(unsigned int M, unsigned int K) {
    std::string bitmask(K, 1);  // K leading 1's
    bitmask.resize(M, 0);       // M-K trailing 0's
    vector<vector<unsigned int>> res;

    do {
        vector<unsigned int> v;
        for (unsigned int i = 0; i < M; ++i)  // [0..M-1] integers
        {
            if (bitmask[i])
                v.push_back(i);
        }
        sort(v.begin(), v.end());  // tri les entrées
        res.push_back(v);
    } while (std::prev_permutation(bitmask.begin(), bitmask.end()));  // permute bitmask
    return res;
}

/** Determines the simulation time of the first broadcast of node id.
 * @param id Node identificator.
 * @return Simulation time at which process id will broadcast its first message.
 */
simtime_t SimulationParameters::determineFirstSendTimeInMs(unsigned int id) {
    float nodesPerPeak = nbNodes / PEAKSPERDELAY;
    int NodeWillbroadcastInPeak = id / nodesPerPeak;
    float durationOfOnePeak = delaySend / PEAKSPERDELAY;
    int nodeWillBroadcastAt =
        durationOfOnePeak * NodeWillbroadcastInPeak * 1000000;  // necessary because SimTime only takes int as argument
    cerr << "Node " << id << " will send first message at " << SimTime(nodeWillBroadcastAt, SIMTIME_US) << endl;
    return SimTime(nodeWillBroadcastAt, SIMTIME_US);
}

/** Getter of the entries incremented by nodes.
 *  @param nodeId Identificator of the node of which requests the entries incremented when it broadcasts a message.
 *  @return set of entries incremented by nodeId when it broadcasts a message.
 */
const vector<unsigned int>& SimulationParameters::getEntriesIncrementedByProcess(unsigned int nodeId) {
    return entriesIncrementedByProcess[nodeId % entriesIncrementedByProcess.size()];
}

/** Computes the dependency combinations for dependecies sets going from 1 to 1000 dependencies.
 */
void SimulationParameters::computeDependecyCombinationsArray() {
    dependencyCombinations.resize(1000);
    for (unsigned int i = 1; i < 1000; i++) {
        dependencyCombinations[i] = DependencyCombinations(i);
        //        cerr<<"size of dependencycombination["<<i<<"]: " << dependencyCombinations[i].size()<<endl;
    }
}

/** Computes the set of dependencies combinations for a given number of possible dependencies.
 * For example, with 4 possible dependencies the returned vector (with 1=true and 0=false) will be:
 * { {1111}, {0111}, {1011}, {0011}, {1101}, {0101}, {1001}, {0001}, {1110}, {0110}, {1010}, {0010}, {1100}, {0100},
 * {1000}, {0000}.
 *  @param nbPossibleDep Number of possible dependencies.
 *  @return Vector containing vectors $v$ of booleans, with $v[i][j]=false$ meaning that the dependency $j$ should not
 * be considered in the $ith$ hash computed for a message with nbPossibleDep possible dependencies.
 */
vector<vector<bool>> SimulationParameters::DependencyCombinations(unsigned int nbPossibleDep) {
    vector<vector<bool>> res;
    vector<vector<bool>> tmplist;

    // Initialization
    vector<bool> ltmp;
    ltmp.resize(nbPossibleDep, true);
    res.push_back(ltmp);
    ltmp[0] = false;
    res.push_back(ltmp);

    for (unsigned int i = 1; i < nbPossibleDep && res.size() < LIMIT_HASHS; i++) {
        for (vector<bool> tmp : res) {
            tmp[i] = false;
            tmplist.push_back(tmp);
            if (res.size() + tmplist.size() == LIMIT_HASHS)
                break;
        }
        res.insert(res.end(), tmplist.begin(), tmplist.end());
        tmplist.clear();
    }
    return res;
}

/** Computes the set of dependencies combinations for a given number of possible dependencies.
 * For example, with 4 possible dependencies the returned vector (with 1=true and 0=false) will be:
 * { {1111}, {0111}, {1011}, {1101}, {1110}, {0011}, {1001}, {1100}, {1010}, {0110}, {0101}, {0001}, {1000}, {0100},
 * {0010}, {0000}.
 *  @param nbPossibleDep Number of possible dependencies.
 *  @return Vector containing vectors $v$ of booleans, with $v[i][j]=false$ meaning that the dependency $j$ should not
 * be considered in the $ith$ hash computed for a message with nbPossibleDep possible dependencies.
 */
vector<vector<bool>> SimulationParameters::DependencyCombinationsTwo(unsigned int nbPossibleDep) {
    vector<vector<bool>> res;

    for (unsigned int k = nbPossibleDep; k > 0; k--) {
        vector<vector<bool>> tmp = binominalCoefficient(nbPossibleDep, k);
        res.insert(res.end(), tmp.begin(), tmp.end());
        if (res.size() >= LIMIT_HASHS)
            break;
    }
    return res;
}

/* Builds vectors of booleans used to test combinations of dependencies sets.
 * @param N number of possible dependencies.
 * @param K number of dependencies to set to false.
 *  @return Vector containing vectors $v$ of booleans, with $v[i][j]=false$ meaning that the dependency $j$ should not
 * be considered in the $ith$ hash computed for a message with nbPossibleDep possible dependencies.
 */
vector<vector<bool>> SimulationParameters::binominalCoefficient(unsigned int N, unsigned int K) {
    std::string bitmask(K, 1);  // K leading 1's
    bitmask.resize(N, 0);       // N-K trailing 0's
    vector<vector<bool>> res;
    do {
        vector<bool> v(N, true);
        for (unsigned int i = 0; i < N; ++i)  // [0..N-1] integers
        {
            if (!bitmask[N - i - 1])
                v[i] = false;
        }
        res.push_back(v);
        if (res.size() == LIMIT_HASHS)
            break;  // will not test them anyway because does only up to LIMIT_HASHS tests
    } while (std::prev_permutation(bitmask.begin(), bitmask.end()));  // permute bitmask
    return res;
}

/** Returns the sets of dependency combinations for a given number of possible dependencies.
 * @param nbPossibleDependencies Number of possible dependencies.
 *  @return Vector containing vectors $v$ of booleans, with $v[i][j]=false$ meaning that the dependency $j$ should not
 * be considered in the $ith$ hash computed for a message with nbPossibleDep possible dependencies.
 */
const vector<vector<bool>>& SimulationParameters::getDependencyCombinations(unsigned int nbPossibleDependencies) {
    if (nbPossibleDependencies >= dependencyCombinations.size())
        throw "dependencyCombinations too small";
    else
        return dependencyCombinations[nbPossibleDependencies];
}

/** @brief Determines if nodes use a hash-based error detector.
 */
bool SimulationParameters::usesHashDetector() {
    return DeliveryController == SimulationParameters::Delivery::HashClockDifference ||
           DeliveryController == SimulationParameters::Delivery::HashIntervalDifference ||
           DeliveryController == SimulationParameters::Delivery::Recovery ||
           DeliveryController == SimulationParameters::Delivery::RecoveryTest;
}

/** @brief Determines if nodes are recovering the causal dependencies of messages.
 */
bool SimulationParameters::usesRecoveries() {
    return DeliveryController == SimulationParameters::Delivery::Recovery ||
           DeliveryController == SimulationParameters::Delivery::RecoveryTest;
}
