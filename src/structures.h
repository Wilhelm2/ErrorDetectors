/*
 * structures.h
 *
 *  Created on: Dec 16, 2020
 *      Author: wilhelm
 */

#ifndef STRUCTURES_H_
#define STRUCTURES_H_

#include <vector>
#include <omnetpp.h>

#include "Clocks/ProbabilisticClock.h"

typedef struct s_id
{
    unsigned int id;
    unsigned int seq;
} idMsg;
#define IDMSG_EQ(idM1,idM2) (idM1.id == idM2.id && idM1.seq == idM2.seq)

typedef struct s_messageInfo
{
    idMsg id;
    omnetpp::simtime_t recvtime;
    size_t hash;
    std::vector<idMsg> dependencies;
    ProbabilisticClock clock;

    s_messageInfo(idMsg id, omnetpp::simtime_t recvtime, size_t hash, vector<idMsg> dependencies, ProbabilisticClock clock)
    {
        this->id = id;
        this->recvtime = recvtime;
        this->hash = hash;
        this->dependencies = dependencies;
        this->clock = clock;
    }
    s_messageInfo()
    {

    }
}messageInfo;

#define CHANNELDELAY 100000.
#define PACKAGENAME "ErrorDetectorWithRecovery"

#endif /* STRUCTURES_H_ */
