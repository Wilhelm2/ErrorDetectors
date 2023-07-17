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

#ifndef DEPENDENCIES_H_
#define DEPENDENCIES_H_

#include <algorithm>
#include <iostream>
#include <vector>
#include "../structures.h"

using namespace std;

/** Base class for dependencies*/
class Dependencies {
   public:
    Dependencies();
    virtual ~Dependencies() = 0;
};

#endif /* DEPENDENCIES_H_ */
