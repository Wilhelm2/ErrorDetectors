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

#include "../Messages/depReq_m.h"
#include "../Messages/depRsp_m.h"
#include "NodeDetector.h"

/** Contains the statistics relevant to recoveries.*/
typedef struct s_stats_recovery {
    /** Number of local recoveries.*/
    unsigned int nbRecoveries = 0;
    /** Number of recoveries avoided through the recoveryTest.*/
    unsigned int nbAvoidedRecoveries = 0;
    /** Number of jumped broadcasts (because the node was in recovery). */
    unsigned int jumpedBroadcasts = 0;
} stats_recovery;

/** Contains the dependencies of broadcasted messages*/
typedef struct s_sentMsgDependency {
    /** Sequence number of the message.*/
    unsigned int seqMsg;
    /** Causal dependencies of the message.*/
    TotalDependencies dependencies;
} msgDependency;

/** Contains the information about a recovered message as well as its causal dependencies.*/
typedef struct s_recoveredMessage {
    /** Information about the recovered message.*/
    messageInfo msg;
    /** The recovered message's causal dependencies. */
    TotalDependencies dependencies;
} recoveredMessage;

/** Nodes that recover the causal dependencies of messages which the error detector tags as not causally ordered.*/
class NodeWithRecovery : public NodeDetector {
   public:
    NodeWithRecovery();
    virtual ~NodeWithRecovery();

   protected:
    virtual void handleMessage(cMessage* msg) override;
    virtual void processMessage(cMessage* msg);
    void processDepReq(DepReq* m);
    void processRcvRspDep(DepRsp* m);
    void pushbackMessagesInRecovery();

    virtual void iterativeDelivery();
    bool tryDeliverMessageInRecovery();
    bool tryDeliverPendingMessages();

    virtual bool testDeliverMessage(const messageInfo& m);
    void requestDependencies(const messageInfo& m);
    virtual AppMsg* prepareBroadcast();

    /** Stats about the recoveries the node did*/
    stats_recovery statsRecovery;
    /** True during recoveries and false otherwise*/
    bool inRecovery = false;
    /** Set to true if the node tries to broadcast a message during recovery. Is set to false at the end of the
     * recovery*/
    bool delayedAppMgsBroadcast = false;
    /** Message which is currently in recovery. Invalid outside recoveries.*/
    messageInfo currRecovery;

    /** A process keeps the dependencies of messages it broadcasts to reply to recovery request.*/
    vector<msgDependency> localSentMsgDependencies;
    /** List containing the messages to recover when a recovery is already in progress. The messages are pushed back to
     * the pending message list at the end of the recovery.*/
    vector<messageInfo> messagesToRecover;
    /** List containing messages for which the node received the dependencies list but which have some dependencies that
     * the node has not delivered yet.*/
    vector<recoveredMessage> recoveredMessages;

    friend class Stats;
};

#endif /* NODES_NODEWITHRECOVERY_H_ */
