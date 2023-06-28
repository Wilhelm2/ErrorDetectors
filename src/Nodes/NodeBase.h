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

static unsigned int idCountNodeDep=0;

typedef struct s_statsNode {
    unsigned int nbFalseDeliveredMessages = 0;
    unsigned int nbSentDependencies = 0;
    unsigned int nbDeliveredMessages = 0;
    unsigned int controlDataSize = 0;
}statsNode;

class NodeBase : public cSimpleModule
{
    protected:
        NodeBase();
        virtual ~NodeBase();

        virtual void initialize() override;
        void initializeCommunications();
        virtual void handleMessage(cMessage *msg) override;

        virtual AppMsg* prepareBroadcast();
        virtual AppMsg* createAppMsg();

        virtual void processMessage(cMessage* msg) = 0;
        virtual bool deliverMsg(const messageInfo& message);

        const unsigned int id = idCountNodeDep++;
        unsigned int seq = 0;

        cMessage* broadcastTimer = new cMessage();
        simtime_t baseTimeBroadcast ;
        std::default_random_engine generatorSendDistribution;
        static std::normal_distribution<double> sendDistribution;
        cGate* outGate;

        SimulationParameters* params;
        statsNode stats;
        Controller* control;

        friend class Stats;
};

#endif /* NODES_NODEBASE_H_ */
