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


// Message to reply to the request of an application message's causal dependencies (depReq message)

cplusplus{{  
    #include "../structures.h"    
    #include <vector> 
    #include "../Dependencies/TotalDependencies.h"
    typedef idMsg idMsgDepRsp;
    typedef TotalDependencies idDepDepRsp;
    typedef omnetpp::simtime_t depRspTime;
}}

class idMsgDepRsp {
    @existingClass;
}

class idDepDepRsp {
    @existingClass;
}

class depRspTime{
   @existingClass;
}

packet DepRsp {
    idMsgDepRsp idMsgToRecover; // The id of the message of which requests the causal dependencies
    idDepDepRsp dependencies; // Dependencies 
    depRspTime delay; // ms 
    unsigned int idRequster; // Id of the node that requested the message's causal dependencies 
}
