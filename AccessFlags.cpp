#include "stdafx.h"
#include "Basic.h"
#include "AccessFlags.h"


int AccessFlags::Read(istream &source)
{
    Word flags;
    if(!flags.Read(source))
        return 0;
    flags_ = flags;
    return 1;
}
