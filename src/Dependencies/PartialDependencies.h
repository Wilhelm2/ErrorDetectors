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

#ifndef DEPENDENCIES_PARTIALDEPENDENCIES_H_
#define DEPENDENCIES_PARTIALDEPENDENCIES_H_

#include "Dependencies.h"
#include "TotalDependencies.h"

class PartialDependencies : public Dependencies {
public:
    PartialDependencies();
    PartialDependencies(map<unsigned int,idMsg> dependencies);
    virtual ~PartialDependencies();

    unsigned int& operator[](unsigned int i);
    bool operator==(PartialDependencies compareDep);
    bool operator<(PartialDependencies compareDep);

    map<unsigned int,idMsg> getDependencies() const;
    bool includesDependencies(PartialDependencies dependenciesToCheck);
    void print();
    void printErr();
    bool SatisfiesDeliveryConditions(PartialDependencies MessageDependencies, unsigned int idMessageSender);
    void printComparisionWith(PartialDependencies dep);
    void clear();
    void set(idMsg id);

    map<unsigned int,idMsg> dependencies;
};

#endif /* DEPENDENCIES_PARTIALDEPENDENCIES_H_ */
