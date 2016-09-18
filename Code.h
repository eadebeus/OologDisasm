#ifndef _CODE_H
#define _CODE_H

#include "Error.h"
#include "Basic.h"
#include "Instruction.h"
#include "ExceptionItem.h"


class Instruction;
class ExceptionItem;

class Code {
protected:
    Instruction **instructions_;
    ExceptionItem *exceptions_;
    dword codeLength_;
    int iterator_;
public:
    Code(int);
    virtual ~Code(void);
    void SetExceptions(ExceptionItem *exceptions) { exceptions_ = exceptions; };
    void SetInstruction(Instruction *);
};

#endif
