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

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <string.h>
#include <omnetpp.h>
#include <vector>
#include <stdexcept>

#include "structures.h"
#include "simulationParameters.h"
#include "Dependencies/Dependencies.h"
#include "Dependencies/TotalDependencies.h"

using namespace std;
using namespace omnetpp;


typedef struct s_msg
{
    // id not required since struct belongs to a vector associated with the message's sending process
    unsigned int seq;
    TotalDependencies dependencies;
    unsigned int deliveredAtNbPs = 0; // number of nodes that delivered the message
    vector<bool> psHasDelivered; // array to control multiple deliveries
    simtime_t sendTime;
    vector<simtime_t> deliveredAtTime;
}msg;


class Controller : public cSimpleModule{
    public:
        Controller();
        virtual ~Controller();
        bool isDependency(idMsg idM, idMsg idDep);
        void notifySendMessage(unsigned int idSender, unsigned int seq);
        bool notifyDeliverMessage(idMsg idM, unsigned int idDest);
        bool canCausallyDeliverMessage(idMsg idM, unsigned int idDest);
    private:
        virtual void initialize(int stage);
        vector<msg>::iterator searchMessage(idMsg idM);
        void printDeliveryError(string errorReason, idMsg idM, unsigned int destProcess, const TotalDependencies& messageDependencies, const TotalDependencies& processDependencies);
        void deleteMessage(idMsg idM);

        vector<vector<msg>> processBroadcastedMessages; // a vector of msg for each process
        vector<TotalDependencies> processDependencies;
        SimulationParameters* params;
};

Define_Module(Controller);

#endif /* CONTROLLER_H_ */
