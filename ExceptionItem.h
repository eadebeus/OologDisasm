#ifndef _EXCPTITM_H
#define _EXCPTITM_H

#include "Basic.h"
#include "ConstantInfo.h"
#include "Class.h"

class ExceptionItem {
private:
    int startPC_;
    int endPC_;
    int handlerPC_;
    ConstantInfo *catch_;
public:
    ExceptionItem(void);
    ~ExceptionItem(void);
    int Read(istream &source, Class *cls);
    int HandlerPC(void) { return handlerPC_; };
    int StartPC(void) { return startPC_; };
    int EndPC(void) { return endPC_; };
    ConstantInfo *CatchType(void) { return catch_; };
    void Dump(ostream &sink);
};

#endif
