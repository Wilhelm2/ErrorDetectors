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

/** Partial dependencies means that it does not desribe all the dependencies of a message.
 * Hence, instead of a vector with one entry per node of the system, partial dependencies only have an entry for a subset of a message's dependencies. */
class PartialDependencies : public Dependencies {
    public:
        PartialDependencies();
        PartialDependencies(const map<unsigned int,idMsg>& dependencies);
        virtual ~PartialDependencies();

        unsigned int& operator[](unsigned int i);
        unsigned int at(unsigned int i) const;
        bool operator==(const PartialDependencies& compareDep) const;
        bool operator<(const PartialDependencies& compareDep) const;

        map<unsigned int,idMsg> getDependencies() const;
        bool includesDependencies(const PartialDependencies& dependenciesToCheck);
        void print();
        void printErr();
        bool SatisfiesDeliveryConditions(const PartialDependencies& MessageDependencies, unsigned int idMessageSender);
        void clear();
        void set(idMsg id);
    private:
        /** Contains the partial dependencies.*/
        map<unsigned int,idMsg> dependencies;
};

#endif /* DEPENDENCIES_PARTIALDEPENDENCIES_H_ */
