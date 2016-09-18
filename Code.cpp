#include "Code.h"


Code::Code(int codeLength)
{
    codeLength_ = codeLength;
    if(!(instructions_ = new Instruction*[codeLength_]))
        RetError();
    memset(instructions_, 0, codeLength_ * sizeof(Instruction *));
    iterator_ = 0;
}

Code::~Code(void)
{
    for(int i = 0; i < (int) codeLength_; i++)
        delete instructions_[i];
}

void Code::SetInstruction(Instruction *ins)
{
    if(ins)
        instructions_[ins->Offset()] = ins;
}
