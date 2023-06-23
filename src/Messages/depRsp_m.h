//
// Generated file, do not edit! Created by nedtool 5.5 from Messages/depRsp.msg.
//

#ifndef __DEPRSP_M_H
#define __DEPRSP_M_H

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
    #include <vector> 
    #include "../Dependencies/TotalDependencies.h"
    typedef idMsg idMsgDepRsp;
    typedef TotalDependencies idDepDepRsp;
    typedef omnetpp::simtime_t depRspTime;
// }}

/**
 * Class generated from <tt>Messages/depRsp.msg:42</tt> by nedtool.
 * <pre>
 * packet DepRsp
 * {
 *     idMsgDepRsp idMsgToRecover;
 *     idDepDepRsp dependencies;
 *     depRspTime delay; // ms 
 *     unsigned int idRequster;
 * }
 * </pre>
 */
class DepRsp : public ::omnetpp::cPacket
{
  protected:
    idMsgDepRsp idMsgToRecover;
    idDepDepRsp dependencies;
    depRspTime delay;
    unsigned int idRequster;

  private:
    void copy(const DepRsp& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const DepRsp&);

  public:
    DepRsp(const char *name=nullptr, short kind=0);
    DepRsp(const DepRsp& other);
    virtual ~DepRsp();
    DepRsp& operator=(const DepRsp& other);
    virtual DepRsp *dup() const override {return new DepRsp(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual idMsgDepRsp& getIdMsgToRecover();
    virtual const idMsgDepRsp& getIdMsgToRecover() const {return const_cast<DepRsp*>(this)->getIdMsgToRecover();}
    virtual void setIdMsgToRecover(const idMsgDepRsp& idMsgToRecover);
    virtual idDepDepRsp& getDependencies();
    virtual const idDepDepRsp& getDependencies() const {return const_cast<DepRsp*>(this)->getDependencies();}
    virtual void setDependencies(const idDepDepRsp& dependencies);
    virtual depRspTime& getDelay();
    virtual const depRspTime& getDelay() const {return const_cast<DepRsp*>(this)->getDelay();}
    virtual void setDelay(const depRspTime& delay);
    virtual unsigned int getIdRequster() const;
    virtual void setIdRequster(unsigned int idRequster);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const DepRsp& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, DepRsp& obj) {obj.parsimUnpack(b);}


#endif // ifndef __DEPRSP_M_H

