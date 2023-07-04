/** @file CommunicationDispatcher.h
    @brief File that contains the class used to connect nodes. 
 * The system is organized in a star formation with the communicationDispatcher in the midle.  
 * All nodes are connected to the communicationDispatcher and send messages to each other through the communicationDispatcher.
 * Using this star formation graph instead of a complete graph reduces the number of communication channels from N^2 channels to N channels, which particularly reduces the simulation's initialization time. 
 * 
 * The communicationDispatcher routes messages to the destination, handles the broadcast of application messages, and sets the propagation delays of messages. 
*/

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


#ifndef __RECOVERYPC_COMPLETEGRAPHEMODULE_H_
#define __RECOVERYPC_COMPLETEGRAPHEMODULE_H_

#include <omnetpp.h>
#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <regex>
#include <functional>
#include "Messages/AppMsg_m.h"
#include "Messages/depReq_m.h"
#include "Messages/depRsp_m.h"
#include "Messages/Init_m.h"
#include "Controller.h"
#include "simulationParameters.h"

using namespace omnetpp;
using namespace std;

typedef struct s_statsCommunicationModule
{
    unsigned int delayIntervals[200]={0};
}statsCommunicationModule;

/** @class CommunicationDispatcher
    @brief Class used to connect nodes. 
 * The system is organized in a star formation with the communicationDispatcher in the midle.  
 * All nodes are connected to the communicationDispatcher and send messages to each other through the communicationDispatcher.
 * Using this star formation graph instead of a complete graph reduces the number of communication channels from N^2 channels to N channels, which particularly reduces the simulation's initialization time. 
 * 
 * The communicationDispatcher routes messages to the destination, handles the broadcast of application messages, and sets the propagation delays of messages. 
*/
class CommunicationDispatcher : public cSimpleModule
{
    private:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
        void broadcastAppMessage(AppMsg* m);
        simtime_t computeDelay(unsigned int idSourceProcess, unsigned int idTargetProcess);
        void incrementDelayIntervals(unsigned int entry);

        /** vector containing a gate reference to each node.*/
        vector<cGate*> gates; 

        /** Used to generate channel propagation delays*/
        std::default_random_engine generatorChannelDelay;
        std::normal_distribution<double> distributionChannelDelayPair = std::normal_distribution<double>(CHANNELDELAY,30000.);
        std::normal_distribution<double> distributionChannelDelayImpair = std::normal_distribution<double>(CHANNELDELAY,30000.);
        /** Used to attribute a different channel number to each gate*/
        vector<int> channelRandNumber;
        /** Stats module to take statistics about message propagation delays*/
        statsCommunicationModule stats;
        /** Reference to the SimulationParameters module which contains the simulation parameters*/
        SimulationParameters* params;
};

#endif
