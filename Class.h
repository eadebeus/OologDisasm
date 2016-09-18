#ifndef _CLASS_H
#define _CLASS_H


#include "Basic.h"
#include "HashTable.h"
#include "ConstantInfo.h"
#include "FieldInfo.h"
#include "MethodInfo.h"
#include "AccessFlags.h"
#include "Error.h"


class ConstantInfo;
class FieldInfo;
class MethodInfo;

class Class {
private:
    Word minorVersion_;
    Word majorVersion_;
    int constantPoolCount_;
    ConstantInfo **constantPool_;
    AccessFlags access_;
    ConstantInfo *thisClass_;
    ConstantInfo *superClass_;
    char *package_;
    int interfaceCount_;
    ConstantInfo **interfaces_;
    int fieldCount_;
    FieldInfo *fields_;
    int methodCount_;
    MethodInfo *methods_;
    int attributeCount_;
    ConstantInfo *sourceFileAttribute_;
public:
    Class(void);
    ~Class(void);
    int Read(istream &source);
    void Dump(ostream &sink);
    ConstantInfo *Constant(int);
    const ConstantInfo *ThisClass(void) { return (const ConstantInfo *) thisClass_; };
    // Note: this function allocates a string which must eventually be freed
    char *ExportInternalClassName(char *);
};

#endif
