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

/** @struct s_id
 *  @brief Each message has an unique identifier composed of the id of the node $p_i$ that sent it and the sequence number $p_i$ associated to the message
 */
typedef struct s_id
{
    /** Identification number of the node that sent the message */
    unsigned int id;
    /** Sequence number that the message's sending node associated to the message*/
    unsigned int seq;
} idMsg;

/**@def Tests the equality of two message identificators */
#define IDMSG_EQ(idM1,idM2) (idM1.id == idM2.id && idM1.seq == idM2.seq)

/** @struct s_messageInfo
 *  @brief Structure containing information about received messages required to causally order them as well as to execute the error detectors on them
 */
typedef struct s_messageInfo
{
    /** The message's identificator*/
    idMsg id;
    /** The message's reception time */
    omnetpp::simtime_t recvtime;
    /** The message's  hash */
    size_t hash;
    /** The message's causal dependencies appended. This field only contains dependencencies when APPENDDEPENDENCIES (see simulationParameters.h) is defined as true*/
    std::vector<idMsg> dependencies;
    /** The message's probabilistic clock */
    ProbabilisticClock clock;

    /**@brief Constructor for s_messageInfo structs
     * @param id The message's identificator
     * @param recvtime The message's reception time
     * @param hash The message's hash
     * @param dependencies the message's Causal dependencies
     * @param clock The message's Probabilistic clock
     *
     * */
    s_messageInfo(idMsg id, omnetpp::simtime_t recvtime, size_t hash, vector<idMsg> dependencies, ProbabilisticClock clock)
    {
        this->id = id;
        this->recvtime = recvtime;
        this->hash = hash;
        this->dependencies = dependencies;
        this->clock = clock;
    }
    /**@brief Empty constructor for s_messageInfo structs
     */
    s_messageInfo()
    {

    }
}messageInfo;

/** @def Average communication delay between nodes*/
#define CHANNELDELAY 100000.
/** @def Package name of project*/
#define PACKAGENAME "ErrorDetectorWithRecovery"

#endif /* STRUCTURES_H_ */
