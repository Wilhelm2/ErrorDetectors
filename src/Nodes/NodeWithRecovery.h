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

#ifndef NODES_NODEWITHRECOVERY_H_
#define NODES_NODEWITHRECOVERY_H_

#include "NodeDetector.h"
#include "../Messages/depReq_m.h"
#include "../Messages/depRsp_m.h"

typedef struct s_stats_recovery{
    unsigned int nbRecoveries = 0;
    unsigned int nbAvoidedRecoveries = 0;
} stats_recovery;

typedef struct s_sentMsgDependency{
    unsigned int seqMsg;
    TotalDependencies dependencies;
} msgDependency;

typedef struct s_recoveredMessage{
    messageInfo msg;
    TotalDependencies dependencies;
} recoveredMessage;

class NodeWithRecovery : public NodeDetector
{
    public:
        NodeWithRecovery();
        virtual ~NodeWithRecovery();
        virtual void handleMessage(cMessage *msg) override;
        virtual void processMessage(cMessage* msg);
        void processDepReq(DepReq* m);
        void processRcvRspDep(DepRsp* m);
        void pushbackMessagesInRecovery();

        virtual void iterativeDelivery();
        bool tryDeliverMessageInRecovery();
        bool tryDeliverPendingMessages();

        virtual bool testDeliverMessage(messageInfo m);
        void requestDependencies(messageInfo m);
        virtual AppMsg* prepareBroadcast();

        stats_recovery statsRecovery;
        bool inRecovery = false;
        bool delayedAppMgsBroadcast = false;
        messageInfo currRecovery;

        vector<msgDependency> localSentMsgDependencies;
        vector<messageInfo> messagesToRecover;
        vector<recoveredMessage> recoveredMessages;
};

#endif /* NODES_NODEWITHRECOVERY_H_ */
