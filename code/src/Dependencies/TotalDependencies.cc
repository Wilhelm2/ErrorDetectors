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

#include "TotalDependencies.h"

TotalDependencies::TotalDependencies() {}

TotalDependencies::TotalDependencies(const vector<unsigned int>& dependencies) {
    this->dependencies = dependencies;
}

TotalDependencies::TotalDependencies(unsigned int size) {
    dependencies.resize(size, 0);
}

TotalDependencies::~TotalDependencies() {}

unsigned int& TotalDependencies::operator[](unsigned int i) {
    return dependencies[i];
}

unsigned int TotalDependencies::operator[](unsigned int i) const {
    return dependencies[i];
}

bool TotalDependencies::operator==(const TotalDependencies& compareDep) {
    for (unsigned int i = 0; i < dependencies.size(); i++)
        if (dependencies[i] != compareDep.getDependencies()[i])
            return false;
    return true;
}

bool TotalDependencies::operator<(const TotalDependencies& compareDep) {
    for (unsigned int i = 0; i < dependencies.size(); i++)
        if (dependencies[i] < compareDep[i])
            return true;
    return false;
}

vector<unsigned int> TotalDependencies::getDependencies() const {
    return dependencies;
}

/** Verifies that the dependencies also include the dependencies to check,
 * ie a node that delivered the messages described by this dependencies also delivered the dependencies to check.
 * @param dependenciesToCheck The dependencies to check.
 * @return true if dependencies include dependenciesToCheck and false otherwise.*/
bool TotalDependencies::includesDependencies(const TotalDependencies& dependenciesToCheck) {
    for (unsigned int i = 0; i < dependencies.size(); i++)
        if (dependencies[i] < dependenciesToCheck[i])
            return false;
    return true;
}

/** Verifies that the dependencies also include the dependencies to check,
 * ie a node that delivered the messages described by this dependencies also delivered the dependencies to check.
 * @param dependenciesToCheck The dependencies to check.
 * @return true if dependencies include dependenciesToCheck and false otherwise.*/
bool TotalDependencies::includesDependencies(const vector<idMsg>& dependenciesToCheck) {
    for (const idMsg& id : dependenciesToCheck)
        if (dependencies[id.id] < id.seq)
            return false;
    return true;
}

/** Prints the dependencies on the standard output.*/
void TotalDependencies::print() {
    for (unsigned int elt : dependencies)
        cout << elt << "\t";
    cout << endl;
}

/** Prints the dependencies on the error output.*/
void TotalDependencies::printErr() const {
    for (unsigned int elt : dependencies)
        cerr << elt << "\t";
    cerr << endl;
}

/** Verifies that the dependencies include the message's dependencies.
 * It is usually called by nodes which want to verify that they delivered all of a message dependencies.
 * Note that the entry corresponding to idMessageSender must only be equal to MessageDependencies[i]-1 because
 * MessageDependencies[i] describes the dependency
 * @param MessageDependencies dependencies to verify.
 * @param idMessageSender Sender of the message.
 * @return true if the dependencies include the message's dependencies and false otherwise.*/
bool TotalDependencies::SatisfiesDeliveryConditions(const TotalDependencies& MessageDependencies,
                                                    unsigned int idMessageSender) {
    for (unsigned int i = 0; i < dependencies.size(); i++) {
        if (MessageDependencies[i] ==
            0)  // No causal dependency + necessary because MessageDependencies[i] -1 provokes buffer underflow
            continue;
        if (i == idMessageSender) {
            if (dependencies[i] < (MessageDependencies[i] - 1))
                return false;
        } else {
            if (dependencies[i] < MessageDependencies[i])
                return false;
        }
    }
    return true;
}

/** Returns the size of the dependencies.
 * @return Size of dependencies.*/
unsigned int TotalDependencies::size() const {
    return dependencies.size();
}
