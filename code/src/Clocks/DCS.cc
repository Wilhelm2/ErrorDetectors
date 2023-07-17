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

#include "DCS.h"

DCS::DCS() {}

DCS::DCS(unsigned int componentSize) {
    this->componentSize = componentSize;
}

DCS::~DCS() {}

void DCS::incrementEntries(const vector<unsigned int>& S_incr, const vector<unsigned int>& entries) {
    for (unsigned int ck : S_incr) {
        for (unsigned int i : entries) {
            if (ck >= clock.size())
                throw "clock smaller than ck";
            clock[ck].clock[i]++;
        }
    }
}

bool DCS::satisfiesDeliveryConditions(const DCS& compareClock,
                                      const vector<unsigned int>& S_incr,
                                      const vector<unsigned int>& entries) const {
    for (unsigned int i = 0; i < this->size(); i++) {
        if (std::find(S_incr.begin(), S_incr.end(), i) != S_incr.end())  // i\in S_incr
        {
            if (!clock[i].clock.satisfiesDeliveryCondition(compareClock.getComponent(i).clock, entries))
                return false;
        } else  // i \notin S_incr
        {
            vector<unsigned int> emptyvector;
            if (!clock[i].clock.satisfiesDeliveryCondition(compareClock.getComponent(i).clock, emptyvector))
                return false;
        }
    }
    return true;
}

unsigned int DCS::size() const {
    return clock.size();
}

component DCS::getComponent(unsigned int k) const {
    return clock[k];
}

bool DCS::prepareComparison(DCS clockCompare) {
    bool activateComponents = false;
    // Step 1
    while (clock.size() < clockCompare.size()) {
        activateComponents = true;
        Add();
        activeComponents = clock.size() - 1;
    }

    // Step 2
    for (unsigned int i = 0; i < clock.size(); i++) {
        if (!(clockCompare.clock[i].clock <= clock[i].clock)) {
            activateComponents = true;
            ActivateComponent(i);
            activeComponents = max(activeComponents, i);
        }
    }

    // ensures that activated components have lower ids than inactive ones
    for (unsigned int i = 0; i <= activateComponents; i++)
        ActivateComponent(i);
    return activateComponents;
}

void DCS::Add() {
    component c = {ProbabilisticClock(componentSize), false};
    clock.push_back(c);
}

void DCS::ActivateComponent(unsigned int i) {
    clock[i].active = true;
}
