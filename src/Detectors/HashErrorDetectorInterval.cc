/*
 * ErrorDetectorInterval.cpp
 *
 *  Created on: Mar 10, 2023
 *      Author: wilhelm
 */

#include "../Detectors/HashErrorDetectorInterval.h"

ErrorDetectorInterval::ErrorDetectorInterval() {
    // TODO Auto-generated constructor stub

}

ErrorDetectorInterval::~ErrorDetectorInterval() {
    // TODO Auto-generated destructor stub
}

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

bool ErrorDetectorInterval::isConsideredAsDependency(const messageInfo& message, const messageInfo& possibleDep)
{
    return(possibleDep.recvtime < message.recvtime - 2*deltaTR - cdelay);
}

bool ErrorDetectorInterval::isConsideredAsPossibleDependency(const messageInfo& message, const messageInfo& possibleDep)
{
    return(possibleDep.recvtime > message.recvtime - 2*deltaTR - cdelay);
}

vector<idMsg> ErrorDetectorInterval::sortPossibleDependenciesSet(const messageInfo& message, const vector<messageInfo>& baseCombineSet, Controller* controller)
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
