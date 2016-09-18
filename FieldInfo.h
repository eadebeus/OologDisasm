#ifndef _FIELD_H
#define _FIELD_H

#include "Basic.h"
#include "Class.h"
#include "AccessFlags.h"
#include "ConstantInfo.h"

class Class;
class ConstantInfo;

class FieldInfo {
private:
    AccessFlags access_;
    ConstantInfo *name_;
    ConstantInfo *descriptor_;
    int attributeCount_;
    ConstantInfo *constantValueAttribute_;
public:
    FieldInfo(void);
    ~FieldInfo(void);
    int Read(istream &source, Class *);
    void Dump(ostream &sink);
};

#endif
