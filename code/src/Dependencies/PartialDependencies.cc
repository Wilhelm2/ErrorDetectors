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

#include "PartialDependencies.h"

PartialDependencies::PartialDependencies() {}

PartialDependencies::~PartialDependencies() {}

PartialDependencies::PartialDependencies(const map<unsigned int, idMsg>& dependencies) {
    this->dependencies = dependencies;
}

unsigned int& PartialDependencies::operator[](unsigned int i) {
    assert(dependencies.find(i) != dependencies.end());
    return dependencies[i].seq;
}

unsigned int PartialDependencies::at(unsigned int i) const {
    return dependencies.at(i).seq;
}

bool PartialDependencies::operator==(const PartialDependencies& compareDep) const {
    for (const tuple<unsigned int, idMsg> m : compareDep.getDependencies()) {
        if (dependencies.find(get<0>(m)) == dependencies.end())
            return false;
        if (get<1>(m).seq != dependencies.at(get<0>(m)).seq)
            return false;
    }
    return true;
}

bool PartialDependencies::operator<(const PartialDependencies& compareDep) const {
    for (tuple<unsigned int, idMsg> m : compareDep.getDependencies()) {
        if (dependencies.find(get<0>(m)) == dependencies.end())
            return false;
        if (get<1>(m).seq >= dependencies.at(get<0>(m)).seq)
            return false;
    }
    return true;
}

map<unsigned int, idMsg> PartialDependencies::getDependencies() const {
    return dependencies;
}

/** Verifies that the dependencies also include the dependencies to check,
 * ie a node that delivered the messages described by this dependencies also
 * delivered the dependencies to check.
 * @param dependenciesToCheck The dependencies to check.
 * @return true if dependencies include dependenciesToCheck and false
 * otherwise.*/
bool PartialDependencies::includesDependencies(const PartialDependencies& dependenciesToCheck) {
    return dependenciesToCheck < *this;
}

/** Prints the dependencies on the standard output.*/
void PartialDependencies::print() {
    for (auto elt : dependencies)
        cout << get<1>(elt).id << "," << get<1>(elt).seq << "\t";
    cout << endl;
}

/** Prints the dependencies on the error output.*/
void PartialDependencies::printErr() {
    for (auto elt : dependencies)
        cerr << get<1>(elt).id << "," << get<1>(elt).seq << "\t";
    cerr << endl;
}

/** Verifies that the dependencies include the message's dependencies.
 * It is usually called by nodes which want to verify that they delivered all of
 * a message dependencies. Note that the entry corresponding to idMessageSender
 * must only be equal to MessageDependencies[i]-1 because MessageDependencies[i]
 * describes the dependency
 * @param MessageDependencies dependencies to verify.
 * @param idMessageSender Sender of the message.
 * @return true if the dependencies include the message's dependencies and false
 * otherwise.*/
bool PartialDependencies::SatisfiesDeliveryConditions(const PartialDependencies& MessageDependencies,
                                                      unsigned int idMessageSender) {
    for (unsigned int i = 0; i < dependencies.size(); i++) {
        // Necessary when appending no causal dependency because
        // MessageDependencies[i] -1 would provoke a buffer underflow
        if (MessageDependencies.at(i) == 0)
            continue;
        if (i == idMessageSender) {
            if (dependencies.at(i).seq < (MessageDependencies.at(i) - 1))
                return false;
        } else {
            if (dependencies.at(i).seq < MessageDependencies.at(i))
                return false;
        }
    }
    return true;
}

void PartialDependencies::clear() {
    dependencies.clear();
}

void PartialDependencies::set(idMsg id) {
    dependencies[id.id] = id;
}
