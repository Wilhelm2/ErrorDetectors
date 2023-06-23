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

#ifndef NODES_NODERECOVERY_H_
#define NODES_NODERECOVERY_H_

#include "Node.h"

typedef struct s_stats_recovery{
    unsigned int nbRecoveries = 0;
    unsigned int nbAvoidedRecoveries = 0;
} stats_recovery;

typedef struct s_sentMsgDependency{
    unsigned int seqMsg;
    TotalDependencies dependencies;
} msgDependency;

typedef struct s_recoveredMessage{
    dep msg;
    TotalDependencies dependencies;
} recoveredMessage;

class RecoveryTest;

class NodeRecovery : public Node {
public:
    NodeRecovery();
    virtual ~NodeRecovery();
    virtual void initialize() override;
    virtual void processMessage(cMessage* msg);
    void processDepReq(DepReq* m);
    virtual void iterativeDelivery();
    bool tryDeliverMessageInRecovery();
    bool tryDeliverPendingMessages();
    virtual bool testDeliverMessage(dep m);
    void requestDependencies(dep m);

    void processRcvRspDep(DepRsp* m);
    void pushbackMessagesInRecovery();
    void sendAppMsg();

    stats_recovery statsRecovery;
    bool inRecovery = false;
    bool delayedAppMgsBroadcast = false;
    dep currRecovery ;

    vector<msgDependency> localSentMsgDependencies;
    vector<dep> messagesToRecover;
    vector<recoveredMessage> recoveredMessages;
};

#endif /* NODES_NODERECOVERY_H_ */
