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

#include "ProbabilisticClock.h"

ProbabilisticClock::ProbabilisticClock() {

}

ProbabilisticClock::ProbabilisticClock(unsigned int size) {
    clock.resize(size,0);
}

ProbabilisticClock::~ProbabilisticClock() {
    // TODO Auto-generated destructor stub
}

bool ProbabilisticClock::operator<=(const ProbabilisticClock& PC) const
{
    for(unsigned int i = 0; i<clock.size();i++)
        if(clock.at(i)>PC.at(i))
            return false;
    return true;
}

unsigned int& ProbabilisticClock::operator[](unsigned int i)
{
    return clock[i];
}

unsigned int ProbabilisticClock::operator[](unsigned int i) const
{
    return clock[i];
}


unsigned int ProbabilisticClock::size() const
{
    return clock.size();
}

void ProbabilisticClock::incrementEntries(const std::vector<unsigned int>& entries)
{
    for(unsigned int i:entries)
        clock[i]++;
}

bool ProbabilisticClock::satisfiesDeliveryCondition(const ProbabilisticClock& PC, const std::vector<unsigned int>& sendIncrementedEntries) const
{
//    printClock();
//    cout<<"Compare to ";
//    PC.printClock();
    for(unsigned int i=0; i<clock.size(); i++)
    {
        bool entryIsIncremented = find(sendIncrementedEntries.begin(), sendIncrementedEntries.end(), i) != sendIncrementedEntries.end();
        if( (!entryIsIncremented && clock[i] < PC[i]) || (entryIsIncremented && clock[i] < PC[i]-1) )
            return false;
    }
    return true;
}

void ProbabilisticClock::printClock() const
{
    cerr<< "Clock: ";
    for(unsigned int i : clock)
        cerr << i << ",";
    cerr<<endl;
}

void ProbabilisticClock::decreaseEntry(unsigned int entry)
{
    clock[entry]--;
}

unsigned int ProbabilisticClock::clockEntryDifference(const ProbabilisticClock& PC) const
{
    unsigned int difference = 0;
    for(unsigned int i=0; i < clock.size(); i++)
        difference += clock[i] - PC[i];
    return difference;
}


const unsigned int ProbabilisticClock::at(unsigned int i) const
{
    return clock[i];
}
