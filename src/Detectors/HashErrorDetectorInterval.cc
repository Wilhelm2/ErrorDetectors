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

vector<idMsg> ErrorDetectorInterval::determineAndGetAppendedDependencies(vector<messageInfo>& delivered, ProbabilisticClock messageClock)
{
    vector<idMsg> dependencies;
    for(const messageInfo t: delivered)
    {
        if(t.recvtime > simTime() - 2*deltaTS ) // appends messages delivered in the last 2\deltaT seconds
            dependencies.push_back(t.id);
    }
    return dependencies;
}

bool ErrorDetectorInterval::isConsideredAsDependency(messageInfo message, messageInfo possibleDep)
{
    return(possibleDep.recvtime < message.recvtime - 2*deltaTR - cdelay);
}

bool ErrorDetectorInterval::isConsideredAsPossibleDependency(messageInfo message, messageInfo possibleDep)
{
    return(possibleDep.recvtime > message.recvtime - 2*deltaTR - cdelay);
}


vector<idMsg> ErrorDetectorInterval::sortPossibleDependenciesSet(messageInfo message, vector<messageInfo> baseCombineSet, Controller* controller)
{
    vector<messageInfo> BaseCombineSetRes;
    vector<idMsg> res;
    vector<messageInfo>::iterator resit;

    for(messageInfo d: baseCombineSet)
    {
        for(resit = BaseCombineSetRes.begin(); resit != BaseCombineSetRes.end(); resit++)
        {
            if(d.recvtime < resit->recvtime)
                break;
        }
        BaseCombineSetRes.insert(resit, d);
    }
    for(messageInfo msg : BaseCombineSetRes)
        res.push_back(msg.id);
    return res;
}
