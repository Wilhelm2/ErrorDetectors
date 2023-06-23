/*
 * ErrorDetectorClockDifference.cpp
 *
 *  Created on: Mar 10, 2023
 *      Author: wilhelm
 */

#include "../Detectors/HashErrorDetectorClockDifference.h"

ErrorDetectorClockDifference::ErrorDetectorClockDifference() {
    // TODO Auto-generated constructor stub

}

ErrorDetectorClockDifference::~ErrorDetectorClockDifference() {
    // TODO Auto-generated destructor stub
}


vector<idMsg> ErrorDetectorClockDifference::determineAndGetAppendedDependencies(const vector<dep>& delivered, const ProbabilisticClock& messageClock)
{
    vector<idMsg> dependencies;
    for(const dep t: delivered)
    {
        if(messageClock.clockEntryDifference(t.clock) < clockDifferenceConsideredDependency)
            dependencies.push_back(t.id);
    }
    return dependencies;
}

bool ErrorDetectorClockDifference::isConsideredAsDependency(dep message, dep possibleDep)
{
    return message.clock.clockEntryDifference(possibleDep.clock) >= clockDifferenceConsideredDependency;
}

bool ErrorDetectorClockDifference::isConsideredAsPossibleDependency(dep message, dep possibleDep)
{
    return message.clock.clockEntryDifference(possibleDep.clock) < clockDifferenceConsideredDependency;
}


vector<idMsg> ErrorDetectorClockDifference::sortPossibleDependenciesSet(dep message, vector<dep> baseCombineSet, Controller* controller)
{
    vector<tuple<unsigned int,dep>> BaseCombineSetRes;
    vector<idMsg> res;
    vector<tuple<unsigned int, dep>>::iterator resit;

    for(dep d: baseCombineSet)
    {
        unsigned int difference = message.clock.clockEntryDifference(d.clock);
        for(resit = BaseCombineSetRes.begin(); resit != BaseCombineSetRes.end(); resit++)
        {
            if(difference < get<0>(*resit))
                break;
        }
        BaseCombineSetRes.insert(resit, make_tuple(difference,d));
    }

    for(tuple<unsigned int,dep> msg : BaseCombineSetRes)
        res.push_back(get<1>(msg).id);
    return res;
}

void ErrorDetectorClockDifference::setClockDifferenceConsideredDependency(unsigned int messageLoad, unsigned nbIncrementedEntries)
{
    clockDifferenceConsideredDependency = (messageLoad*nbIncrementedEntries*0.15 + ASSUMED_CONCURRENT_MESSAGES*nbIncrementedEntries)*1;
    cerr<<"clockDifferenceConsideredDependency = " << clockDifferenceConsideredDependency<<endl;
}

PartialDependencies ErrorDetectorClockDifference::getPartialDependencies(const vector<dep>& delivered, const ProbabilisticClock& clock)
{
    dep message; message.clock = clock;
    PartialDependencies depVector;
    for(dep d : delivered)
    {
        if(isConsideredAsPossibleDependency(message, d))
            depVector.set(d.id);
    }
    return depVector;
}

