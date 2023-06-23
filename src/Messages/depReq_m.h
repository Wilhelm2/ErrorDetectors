//
// Generated file, do not edit! Created by nedtool 5.5 from Messages/depReq.msg.
//

#ifndef __DEPREQ_M_H
#define __DEPREQ_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0505
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



// cplusplus {{
  
    #include "../structures.h"
    typedef idMsg idMsgDepReq;
    typedef omnetpp::simtime_t depReqTime;
// }}

/**
 * Class generated from <tt>Messages/depReq.msg:32</tt> by nedtool.
 * <pre>
 * packet DepReq
 * {
 *     idMsgDepReq idMsgToRecover;
 *     unsigned int idRequester; // debug
 *     depReqTime delay; // ms 
 * }
 * </pre>
 */
class DepReq : public ::omnetpp::cPacket
{
  protected:
    idMsgDepReq idMsgToRecover;
    unsigned int idRequester;
    depReqTime delay;

  private:
    void copy(const DepReq& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const DepReq&);

  public:
    DepReq(const char *name=nullptr, short kind=0);
    DepReq(const DepReq& other);
    virtual ~DepReq();
    DepReq& operator=(const DepReq& other);
    virtual DepReq *dup() const override {return new DepReq(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual idMsgDepReq& getIdMsgToRecover();
    virtual const idMsgDepReq& getIdMsgToRecover() const {return const_cast<DepReq*>(this)->getIdMsgToRecover();}
    virtual void setIdMsgToRecover(const idMsgDepReq& idMsgToRecover);
    virtual unsigned int getIdRequester() const;
    virtual void setIdRequester(unsigned int idRequester);
    virtual depReqTime& getDelay();
    virtual const depReqTime& getDelay() const {return const_cast<DepReq*>(this)->getDelay();}
    virtual void setDelay(const depReqTime& delay);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const DepReq& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, DepReq& obj) {obj.parsimUnpack(b);}


#endif // ifndef __DEPREQ_M_H

