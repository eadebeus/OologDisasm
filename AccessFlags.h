#ifndef _ACCESS_H
#define _ACCESS_H

#define ACC_PUBLIC          0x0001
#define ACC_PRIVATE         0x0002
#define ACC_PROTECTED       0x0004
#define ACC_STATIC          0x0008
#define ACC_FINAL           0x0010
#define ACC_SYNCHRONIZED    0x0020
#define ACC_VOLATILE        0x0040
#define ACC_TRANSIENT       0x0080
#define ACC_NATIVE          0x0100
#define ACC_INTERFACE       0x0200
#define ACC_ABSTRACT        0x0400

#include "Basic.h"


class AccessFlags {
private:
    word flags_;
public:
    AccessFlags() {};
    ~AccessFlags() {};    
    int Read(istream &source);
    int IsPublic(void) { return flags_ & ACC_PUBLIC; };
    int IsPrivate(void) { return flags_ & ACC_PRIVATE; };
    int IsProtected(void) { return flags_ & ACC_PROTECTED; };
    int IsStatic(void) { return flags_ & ACC_STATIC; };
    int IsFinal(void) { return flags_ & ACC_FINAL; };
    int IsSynchronized(void) { return flags_ & ACC_SYNCHRONIZED; };
    int IsVolatile(void) { return flags_ & ACC_VOLATILE; };
    int IsTransient(void) { return flags_ & ACC_TRANSIENT; };
    int IsNative(void) { return flags_ & ACC_NATIVE; };
    int IsAbstract(void) { return flags_ & ACC_ABSTRACT; };
};

#endif
