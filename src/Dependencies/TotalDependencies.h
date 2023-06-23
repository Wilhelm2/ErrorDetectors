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

#ifndef DEPENDENCIES_TOTALDEPENDENCIES_H_
#define DEPENDENCIES_TOTALDEPENDENCIES_H_

#include "Dependencies.h"


class TotalDependencies : public Dependencies {
public:
    TotalDependencies();
    TotalDependencies(vector<unsigned int> dependencies);
    TotalDependencies(unsigned int size);
    virtual ~TotalDependencies();

    unsigned int& operator[](unsigned int i);
    bool operator==(TotalDependencies compareDep);
    bool operator<(TotalDependencies compareDep);

    vector<unsigned int> getDependencies();
    bool includesDependencies(TotalDependencies dependenciesToCheck);
    bool includesDependencies(vector<idMsg> dependenciesToCheck);
    void print();
    void printErr();
    bool SatisfiesDeliveryConditions(TotalDependencies MessageDependencies, unsigned int idMessageSender);
    void printComparisionWith(TotalDependencies dep);



    vector<unsigned int> dependencies;

};

#endif /* DEPENDENCIES_TOTALDEPENDENCIES_H_ */
