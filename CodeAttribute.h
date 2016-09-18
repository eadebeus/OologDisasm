#ifndef _CODEATT_H
#define _CODEATT_H

#include "Class.h"
#include "Basic.h"
#include "ExceptionItem.h"
#include "Code.h"


class Class;
class ExceptionItem;
class Code;

class CodeAttribute {
private:
    int maxStack_;
    int maxLocals_;
    dword codeLength_;
    byte *codeBytes_;
    int exceptionCount_;
    ExceptionItem *exceptions_;
    int attributeCount_;
    Code *code_;
    int ReadIntFromByteArray(byte*, int);
public:
    CodeAttribute(istream &source, Class *);
    ~CodeAttribute(void);
    void Dump(ostream &sink);
};

#endif
