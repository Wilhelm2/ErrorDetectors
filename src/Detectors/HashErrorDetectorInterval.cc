/*
 * ErrorDetectorInterval.cpp
 *
 *  Created on: Mar 10, 2023
 *      Author: wilhelm
 */

#include "../Detectors/HashErrorDetectorInterval.h"

ErrorDetectorInterval::ErrorDetectorInterval() {
}

ErrorDetectorInterval::~ErrorDetectorInterval() {
}

/** Determines the causal dependencies to append on the message to broadcast.
 * @param delivered Messages the node has delivered, ie the causal dependencies of the message to broadcast.
 * @param clock The clock appended to the message to broadcast.
 * @return The set of dependencies that will be appended on the message to broadcast. */
vector<idMsg> ErrorDetectorInterval::determineAndGetAppendedDependencies(const vector<messageInfo>& delivered, const ProbabilisticClock& clock)
{
    vector<idMsg> dependencies;
    for(const messageInfo& t: delivered)
    {
        if(t.recvtime > simTime() - 2*deltaTS ) // appends messages delivered in the last 2\deltaT seconds
            dependencies.push_back(t.id);
    }
    return dependencies;
}

/** Determines if a message is considered as being a dependency.
 * @param message The analyzed message.
 * @param possibleDep The message that is checked to be a dependency.
 * @return true if possibleDep is considered as a dependency of message and false otherwise.*/
bool ErrorDetectorInterval::isConsideredAsDependency(const messageInfo& message, const messageInfo& possibleDep)
{
    return(possibleDep.recvtime < message.recvtime - 2*deltaTR - cdelay);
}

/** Determines if a message is considered as being a possible dependency.
 * @param message The analyzed message.
 * @param possibleDep The message that is checked to be a possible dependency.
 * @return true if possibleDep is considered as a possible dependency of message and false otherwise.*/
bool ErrorDetectorInterval::isConsideredAsPossibleDependency(const messageInfo& message, const messageInfo& possibleDep)
{
    return(possibleDep.recvtime > message.recvtime - 2*deltaTR - cdelay);
}

/** Sorts the set of possible dependencies depending on the reception time of messages.
 * @param message Message to deliver.
 * @param baseCombineSet Set of possible dependencies to order.
 * @return Sorted set of possible dependencies in increasing order of probability of being a dependency of message.
 * */
vector<idMsg> ErrorDetectorInterval::sortPossibleDependenciesSet(const messageInfo& message, const vector<messageInfo>& baseCombineSet)
{
    vector<messageInfo> BaseCombineSetRes;
    vector<idMsg> res;
    vector<messageInfo>::iterator resit;

    for(const messageInfo& d: baseCombineSet)
    {
        for(resit = BaseCombineSetRes.begin(); resit != BaseCombineSetRes.end(); resit++)
        {
            if(d.recvtime < resit->recvtime)
                break;
        }
        BaseCombineSetRes.insert(resit, d);
    }
    for(const messageInfo& msg : BaseCombineSetRes)
        res.push_back(msg.id);
    return res;
}

/** Get partial dependencies of the message to broadcast.
 * @param delivered Messages the node has delivered.
 * @param clock The clock of the message to broadcast.
 * @return Partial dependencies of the message to broadcast.
 * */
PartialDependencies ErrorDetectorInterval::getPartialDependencies(const vector<messageInfo>& delivered, const ProbabilisticClock& clock)
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
