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

/** Contains the total dependencies, ie a vector with one entry per node in the system.*/
class TotalDependencies : public Dependencies {
    public:
        TotalDependencies();
        TotalDependencies(const vector<unsigned int>& dependencies);
        TotalDependencies(unsigned int size);
        virtual ~TotalDependencies();

        unsigned int& operator[](unsigned int i);
        unsigned int operator[](unsigned int i) const;
        bool operator==(const TotalDependencies& compareDep);
        bool operator<(const TotalDependencies& compareDep);

        vector<unsigned int> getDependencies() const;
        bool includesDependencies(const TotalDependencies& dependenciesToCheck);
        bool includesDependencies(const vector<idMsg>& dependenciesToCheck);
        void print();
        void printErr() const;
        bool SatisfiesDeliveryConditions(const TotalDependencies& MessageDependencies, unsigned int idMessageSender);
        unsigned int size() const;

    private:
        /** Contains the dependencies.*/
        vector<unsigned int> dependencies;
};

#endif /* DEPENDENCIES_TOTALDEPENDENCIES_H_ */
