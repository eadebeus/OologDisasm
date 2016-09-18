#include "stdafx.h"
#include "MethodInfo.h"


MethodInfo::MethodInfo(void)
{
    codeAttribute_ = NULL;
}

MethodInfo::~MethodInfo(void)
{
    delete codeAttribute_;
}

int MethodInfo::Read(istream &source, Class *cls)
{
    /*
     * MethodInfo structure:
     *
     *  word accessFlags;
     *  word nameIndex;
     *  word descriptorIndex;
     *  word attributeCount;
     *  AttributeInfo attributes[attributeCount];        
     */
    std::streamoff pos = source.tellg();

    if(!access_.Read(source))
        return 0;

    // Read nameIndex
    Word index;
    if(!index.Read(source))
        return 0;
    name_ = cls->Constant((int) index);

    // Read descriptorIndex
    if(!index.Read(source))
        return 0;
    descriptor_ = cls->Constant((int) index);

    // Read attributes
    Word count;
    if(!count.Read(source))
        return 0;
    attributeCount_ = count;

    /*
     * AttributeInfo structure:
     * 
     *  word attributeName;
     *  dword attributeLength;
     *  byte info[attributeLength];
     * 
     * The only attributes supported for methods are Code and Exceptions.
     */
    for(int i = 0; i < attributeCount_; i++) {
        if(!index.Read(source))
            return 0;
        Dword attributeLength;
        if(!attributeLength.Read(source))
            return 0;
        char *tmpStr = (char *) *(cls->Constant(index));
        if(tmpStr && !strcmp(tmpStr, "Code")) {
            codeAttribute_ = new CodeAttribute(source, cls);
            if(ErrorSet()) {
                DumpError();
                return 0;
            }
        }
        else if(tmpStr && !strcmp(tmpStr, "Exceptions")) {
            /*
             * ExceptionAttribute structure:
             *
             *  word attributeName;
             *  dword attributeLength;
             *  word exceptionsCount;
             *  word exceptions[numExceptions];
             */
            if(!count.Read(source))
                return 0;
            exceptionCount_ = count;
            if(!(exceptions_ = new ConstantInfo*[exceptionCount_]))
                return 0;
            for(i = 0; i < exceptionCount_; i++) {
                if(!index.Read(source))
                    return 0;
                exceptions_[i] = cls->Constant((int) index);
            }
        }
        else
            // Ignore any other attributes.
            source.ignore((int) attributeLength);
        delete tmpStr;
    }
    return 1;
}

void MethodInfo::Dump(ostream &sink)
{
    sink << Indent << "Method: Name ";
    name_->Dump(sink);
    sink << "  ";
    sink << "Descriptor ";
    descriptor_->Dump(sink);
    sink << "\r\n";
    if(exceptionCount_ > 0) {
        sink << IncreaseIndent;
        sink << Indent << "Exceptions:\r\n";
        sink << IncreaseIndent;
        for(int i = 0; i < exceptionCount_; i++) {
            exceptions_[i]->Dump(sink);
            sink << "\r\n";
        }
        sink << DecreaseIndent;
        sink << DecreaseIndent;
    }
    if(codeAttribute_) {
        sink << IncreaseIndent;
        codeAttribute_->Dump(sink);
        sink << DecreaseIndent;
    }
}
