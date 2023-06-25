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

#ifndef ERRORDETECTOR_H_
#define ERRORDETECTOR_H_
#include <string>
#include <regex>
#include <omnetpp.h>

#include "../Messages/AppMsg_m.h"

using namespace std;
using namespace omnetpp;

class ErrorDetector {
public:
    ErrorDetector();
    virtual ~ErrorDetector() = 0;
    virtual AppMsg* prepareMessage(AppMsg* m) = 0;
};

#endif /* ERRORDETECTOR_H_ */
