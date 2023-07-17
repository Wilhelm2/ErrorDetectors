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

#include <omnetpp.h>
#include <string.h>
#include <stdexcept>
#include <vector>

#include "Dependencies/Dependencies.h"
#include "Dependencies/TotalDependencies.h"
#include "simulationParameters.h"
#include "structures.h"

using namespace std;
using namespace omnetpp;

/** Contains the information required to verify that the message is delivered exactly once by every node.
 */
typedef struct s_msg {
    // id not required since the structure belongs to a vector associated with the message's sending node
    /** Sequence number associated to the message to identify it uniquely.*/
    unsigned int seq;
    /** The message's dependencies.*/
    TotalDependencies dependencies;
    /** Number of nodes that delivered the message. Used to delete messages once delivered by all nodes.*/
    unsigned int deliveredAtNbPs = 0;
    /** Array to control multiple deliveries.*/
    vector<bool> psHasDelivered;
    /** The message's sending time.*/
    simtime_t sendTime;
    /** Time at which each node delivers the message. Used for statistics to compute the average delivery time*/
    vector<simtime_t> deliveredAtTime;
} msg;

/** Module to control that nodes deliver application messages exactly once and in causal order.
 *
 */
class Controller : public cSimpleModule {
   public:
    Controller();
    virtual ~Controller();
    bool isDependency(idMsg idM, idMsg idDep);
    void notifySendMessage(unsigned int idSender, unsigned int seq);
    bool notifyDeliverMessage(idMsg idM, unsigned int idDest);
    bool canCausallyDeliverMessage(idMsg idM, unsigned int idDest);

   private:
    virtual void initialize();
    vector<msg>::iterator searchMessage(idMsg idM);
    void printDeliveryError(string errorReason,
                            idMsg idM,
                            unsigned int destnode,
                            const TotalDependencies& messageDependencies,
                            const TotalDependencies& nodeDependencies);
    void deleteMessage(idMsg idM);

    /** Contains for each node $p_i$ a structure per message that is currently broadcasted (not yet delivered by all
     * nodes). nodeBroadcastedMessages[0] contains a structure per message that $p_0$ currently broadcasts.*/
    vector<vector<msg>> nodeBroadcastedMessages;
    /** Contains the dependencies for each node. nodeDependencies[0] contains the total dependencies tracking the
     * messages delivered by $p_0$.*/
    vector<TotalDependencies> nodeDependencies;
    /** Reference to the simulation parameters to avoid looking them up in each function.*/
    SimulationParameters* params;
};

Define_Module(Controller);

#endif /* CONTROLLER_H_ */
