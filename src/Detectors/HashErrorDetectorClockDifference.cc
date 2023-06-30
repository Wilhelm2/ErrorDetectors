/*
 * ErrorDetectorClockDifference.cpp
 *
 *  Created on: Mar 10, 2023
 *      Author: wilhelm
 */

#include "../Detectors/HashErrorDetectorClockDifference.h"

ErrorDetectorClockDifference::ErrorDetectorClockDifference(){
}

ErrorDetectorClockDifference::~ErrorDetectorClockDifference() {
}

/** Determines the causal dependencies to append on the message to broadcast.
 * @param delivered Messages the node has delivered, ie the causal dependencies of the message to broadcast.
 * @param clock The clock appended to the message to broadcast.
 * @return The set of dependencies that will be appended on the message to broadcast. */
vector<idMsg> ErrorDetectorClockDifference::determineAndGetAppendedDependencies(const vector<messageInfo>& delivered, const ProbabilisticClock& clock)
{
    vector<idMsg> dependencies;
    for(const messageInfo& t: delivered)
    {
        if(clock.clockEntryDifference(t.clock) < clockDifferenceConsideredDependency)
            dependencies.push_back(t.id);
    }
    return dependencies;
}

/** Determines if a message is considered as being a dependency.
 * @param message The analyzed message.
 * @param possibleDep The message that is checked to be a dependency.
 * @return true if possibleDep is considered as a dependency of message and false otherwise.*/
bool ErrorDetectorClockDifference::isConsideredAsDependency(const messageInfo& message, const messageInfo& possibleDep)
{
    return message.clock.clockEntryDifference(possibleDep.clock) >= clockDifferenceConsideredDependency;
}

/** Determines if a message is considered as being a possible dependency.
 * @param message The analyzed message.
 * @param possibleDep The message that is checked to be a possible dependency.
 * @return true if possibleDep is considered as a possible dependency of message and false otherwise.*/
bool ErrorDetectorClockDifference::isConsideredAsPossibleDependency(const messageInfo& message, const messageInfo& possibleDep)
{
    return message.clock.clockEntryDifference(possibleDep.clock) < clockDifferenceConsideredDependency;
}

/** Sorts the set of possible dependencies depending on the clock difference of messages with the message to broadcast.
 * @param message Message to deliver.
 * @param baseCombineSet Set of possible dependencies to order.
 * @return Sorted set of possible dependencies in increasing order of probability of being a dependency of message.
 * */
vector<idMsg> ErrorDetectorClockDifference::sortPossibleDependenciesSet(const messageInfo& message, const vector<messageInfo>& baseCombineSet)
{
    vector<tuple<unsigned int,messageInfo>> BaseCombineSetRes;
    vector<idMsg> res;
    vector<tuple<unsigned int, messageInfo>>::iterator resit;

    for(const messageInfo& d: baseCombineSet)
    {
        unsigned int difference = message.clock.clockEntryDifference(d.clock);
        for(resit = BaseCombineSetRes.begin(); resit != BaseCombineSetRes.end(); resit++)
        {
            if(difference < get<0>(*resit))
                break;
        }
        BaseCombineSetRes.insert(resit, make_tuple(difference,d));
    }

    for(const tuple<unsigned int,messageInfo>& msg : BaseCombineSetRes)
        res.push_back(get<1>(msg).id);
    return res;
}

/** Sets the clock difference over which a message is considered as having been delivered by all nodes and is therefore considered as being a causal dependency of the message to deliver.
 * See paper to get precision about the used formula.
 * @param messageLoad Message load inside the system (average number of messages broadcasted by second.)
 * @param nbIncrementedEntries Number of entries incremented by nodes when broadcasting a message.*/
void ErrorDetectorClockDifference::setClockDifferenceConsideredDependency(unsigned int messageLoad, unsigned nbIncrementedEntries)
{
    clockDifferenceConsideredDependency = (messageLoad*nbIncrementedEntries*0.15 + ASSUMED_CONCURRENT_MESSAGES*nbIncrementedEntries)*1;
    cerr<<"clockDifferenceConsideredDependency = " << clockDifferenceConsideredDependency<<endl;
}

/** Get partial dependencies of the message to broadcast.
 * @param delivered Messages the node has delivered.
 * @param clock The clock of the message to broadcast.
 * @return Partial dependencies of the message to broadcast.
 * */
PartialDependencies ErrorDetectorClockDifference::getPartialDependencies(const vector<messageInfo>& delivered, const ProbabilisticClock& clock)
{
    messageInfo message; message.clock = clock;
    PartialDependencies depVector;
    for(const messageInfo& d : delivered)
    {
        if(isConsideredAsPossibleDependency(message, d))
            depVector.set(d.id);
    }
    return depVector;
}

