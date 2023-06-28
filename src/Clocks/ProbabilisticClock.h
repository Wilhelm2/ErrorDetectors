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

#ifndef PROBABILISTICCLOCK_H_
#define PROBABILISTICCLOCK_H_

#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

class ProbabilisticClock {
    public:
        ProbabilisticClock();
        ProbabilisticClock(unsigned int size);
        virtual ~ProbabilisticClock();

        bool operator<=(const ProbabilisticClock& PC) const;
        unsigned int& operator[](unsigned int i);
        unsigned int operator[](unsigned int i) const;
        unsigned int size() const;


        void incrementEntries(const vector<unsigned int>& entries);
        bool satisfiesDeliveryCondition(const ProbabilisticClock& PC, const vector<unsigned int>& sendIncrementedEntries) const;

        void printClock()const;
        void decreaseEntry(unsigned int entry);
        unsigned int clockEntryDifference(const ProbabilisticClock& PC) const;
        const unsigned int at(unsigned int entry) const;
    private:
        vector<unsigned int> clock;
};

#endif /* PROBABILISTICCLOCK_H_ */
