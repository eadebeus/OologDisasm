#include "stdafx.h"
#include "ExceptionItem.h"


ExceptionItem::ExceptionItem(void)
{
}

ExceptionItem::~ExceptionItem(void)
{
}

int ExceptionItem::Read(istream &source, Class *cls)
{
    /*
     * ExceptionItem structure:
     *
     *  word startPC;
     *  word endPC;
     *  word handlerPC;
     *  word catchType;
     */

    Word tmp;
    if(!tmp.Read(source))
        return 0;
    startPC_ = (int) tmp;

    if(!tmp.Read(source))
        return 0;
    endPC_ = (int) tmp;

    if(!tmp.Read(source))
        return 0;
    handlerPC_ = (int) tmp;

    if(!tmp.Read(source))
        return 0;
    if(((int) tmp) == 0)
        catch_ = (ConstantInfo *) 0;
    else
        catch_ = cls->Constant((int) tmp);
    
    return 1;
}

void ExceptionItem::Dump(ostream &sink)
{
    sink << "Start " << startPC_ << " End " << endPC_ << " Handler " << handlerPC_ << "\r\n";
    catch_->Dump(sink);
    sink << "\r\n";
}
