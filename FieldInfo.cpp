#include "stdafx.h"
#include "FieldInfo.h"


FieldInfo::FieldInfo(void)
{
    constantValueAttribute_ = NULL;
}

FieldInfo::~FieldInfo(void)
{
}

int FieldInfo::Read(istream &source, Class *cls)
{
    /*
     * FieldInfo structure:
     *
     *  word accessFlags;
     *  word nameIndex;
     *  word descriptorIndex;
     *  word attributeCount;
     *  AttributeInfo attributes[attributeCount];        
     */
    
    if(!access_.Read(source))
        return 0;

    Word index;
    if(!index.Read(source))
        return 0;
    name_ = cls->Constant((int) index);

    if(!index.Read(source))
        return 0;
    descriptor_ = cls->Constant((int) index);

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
     * The only attribute supported for fields is the ConstantValue attribute.
     */
    for(int i = 0; i < attributeCount_; i++) {
        if(!index.Read(source))
            return 0;
        Dword attributeLength;
        if(!attributeLength.Read(source))
            return 0;
        char *attrName = (char *) *(cls->Constant(index));
        if(attrName && !strcmp(attrName, "ConstantValue")) {
            if(!index.Read(source))
                return 0;
            constantValueAttribute_ = cls->Constant((int) index);
        }
        else
            // Ignore any other attributes.
             source.ignore((int) attributeLength);
        delete attrName;
    }
    return 1;
}

void FieldInfo::Dump(ostream &sink)
{
    sink << Indent << "Field: Name ";
    name_->Dump(sink);
    sink << "  ";
    sink << "Descriptor ";
    descriptor_->Dump(sink);
    sink << "  ";
    if(constantValueAttribute_) {
        sink << "Value ";
        constantValueAttribute_->Dump(sink);
    }
}
