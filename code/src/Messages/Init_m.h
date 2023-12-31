//
// Generated file, do not edit! Created by nedtool 5.5 from Messages/Init.msg.
//

#ifndef __INIT_M_H
#define __INIT_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0505
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



/**
 * Class generated from <tt>Messages/Init.msg:19</tt> by nedtool.
 * <pre>
 * //
 * // TODO generated message class
 * //
 * packet Init
 * {
 *     unsigned int nodeId;
 * }
 * </pre>
 */
class Init : public ::omnetpp::cPacket
{
  protected:
    unsigned int nodeId;

  private:
    void copy(const Init& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const Init&);

  public:
    Init(const char *name=nullptr, short kind=0);
    Init(const Init& other);
    virtual ~Init();
    Init& operator=(const Init& other);
    virtual Init *dup() const override {return new Init(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual unsigned int getNodeId() const;
    virtual void setNodeId(unsigned int nodeId);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const Init& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, Init& obj) {obj.parsimUnpack(b);}


#endif // ifndef __INIT_M_H

