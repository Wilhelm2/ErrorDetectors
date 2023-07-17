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

// Application message

cplusplus{{  
    #include "../structures.h"    
    #include <vector> 
    #include "../Clocks/ProbabilisticClock.h"
    typedef idMsg idMsgAppMsg;
    typedef std::vector<idMsg> AppMsgFalseDep;
    typedef std::vector<idMsg> AppMsgDep;
    typedef ProbabilisticClock AppMsgPC;
    typedef omnetpp::simtime_t AppMsgTime;
}}

class idMsgAppMsg {
    @existingClass;
}

class AppMsgFalseDep {
    @existingClass;
}

class AppMsgDep {
    @existingClass;
}

class AppMsgPC {
    @existingClass;
}

class AppMsgTime{
   @existingClass;
}
    

packet AppMsg {
    idMsgAppMsg id; // id of sender node
    AppMsgPC PC; // The message's probabilistic clock
    unsigned long hash ; // Hash appended on messages 
    AppMsgTime delay; // ms 
    AppMsgFalseDep falseDependencies; // False dependencies used when recovering
    AppMsgDep dependencies; // Dependencies attached to messages used for high message loads
}
