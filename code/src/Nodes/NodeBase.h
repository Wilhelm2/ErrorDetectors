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

#ifndef NODES_NODEBASE_H_
#define NODES_NODEBASE_H_

#include <omnetpp.h>
#include "../Messages/Init_m.h"
#include "../Messages/AppMsg_m.h"
#include "../simulationParameters.h"
#include "../Controller.h"

using namespace omnetpp;
using namespace std;

/** Takes statistics relevant to all types of Nodes
 */
typedef struct s_statsNode {
    /** Number of messages the node delivered out of causal order*/
    unsigned int nbFalseDeliveredMessages = 0;
    /** Number of dependencies the node attached to messages*/
    unsigned int nbSentDependencies = 0;
    /** Number of messages the node delivered */
    unsigned int nbDeliveredMessages = 0;
    /** Size of control information attached to messages*/
    unsigned int controlDataSize = 0;
}statsNode;

/** Base class of Nodes.
 */
class NodeBase : public cSimpleModule
{
    protected:
        NodeBase();
        virtual ~NodeBase();

        virtual void initialize() override;
        void initializeCommunications();
        virtual void handleMessage(cMessage *msg) override;
        void handleBroadcastTimer(cMessage* msg);

        virtual AppMsg* prepareBroadcast();
        virtual AppMsg* createAppMsg();

        /** Called at reception of messages from other nodes in order to process them.*/
        virtual void processMessage(cMessage* msg) = 0;
        virtual bool deliverMsg(const messageInfo& message);

        /** Unique identificator of the process.*/
        const unsigned int id = idCounter++;
        /** Sequence number counter the process uses to give a unique identificator to messages.*/
        unsigned int seq = 0;

        /** Timer to scheduel the broadcast of application messages.*/
        cMessage* broadcastTimer = new cMessage();
        /** Time of first message broadcast.*/
        simtime_t baseTimeBroadcast ;
        /** Used to have a random variation in the broadcast time of application messages. Each node broadcast a message at baseTimeBroadcast + sendDelay + variableDelay, with variableDelay generated with that random engine.*/
        std::default_random_engine generatorSendDistribution;
        /** Used to have a random variation in the broadcast time of application messages. */
        static std::normal_distribution<double> sendDistribution;
        /** Gate used to communicate with other nodes. */
        cGate* outGate;

        /** Reference to the simulation parameters to avoid looking them up in each function.*/
        SimulationParameters* params;
        /** Stats relevant to all types of nodes.*/
        statsNode stats;
        /** Reference to the controller module to avoid looking it up every time. */
        Controller* control;

        friend class Stats;
    private:
        /** Counter used to give a unique identificator to every node.*/
        static unsigned int idCounter;
};

#endif /* NODES_NODEBASE_H_ */
