#ifndef _INSTRUCT_H
#define _INSTRUCT_H

#include "Error.h"
#include "OpCode.h"
#include "Basic.h"

class Instruction {
protected:
    int offset_;
    int size_;
    int offsetNext_;
public:
    Instruction(int, int);
    virtual ~Instruction(void);
    static Instruction *Read(istream &source, int);
    virtual int Size(void);
    int Offset(void) { return offset_; };
};

class TableSwitchInstruction : public Instruction {
private:
    int default_;
    int tableSize_;
    int low_;
    int high_;    
    int *table_;
public:
    TableSwitchInstruction(int, int, istream &);
    ~TableSwitchInstruction(void);    
    int Size(void);
};

class LookupSwitchInstruction : public Instruction {
private:
    int default_;
    int tableSize_;
    int *keyTable_;
    int *addrTable_;
public:
    LookupSwitchInstruction(int, int, istream &);
    ~LookupSwitchInstruction(void);    
    int Size(void);
};

#endif
