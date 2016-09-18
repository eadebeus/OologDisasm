#ifndef _METHOD_H
#define _METHOD_H

#include "Class.h"
#include "AccessFlags.h"
#include "ConstantInfo.h"
#include "Basic.h"
#include "CodeAttribute.h"


class Class;
class ConstantInfo;
class CodeAttribute;

class MethodInfo {
private:
    AccessFlags access_;
    ConstantInfo *name_;
    ConstantInfo *descriptor_;
    int attributeCount_;
    CodeAttribute *codeAttribute_;
    int exceptionCount_;
    ConstantInfo **exceptions_;
public:
    MethodInfo(void);
    ~MethodInfo(void);
    int Read(istream &source, Class *);
    void Dump(ostream &sink);
};

#endif
