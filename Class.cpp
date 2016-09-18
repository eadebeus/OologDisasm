#include "stdafx.h"
#include "Basic.h"
#include "Class.h"


Class::Class(void)
{
    constantPoolCount_ = 0;
    constantPoolCount_ = 0;
    constantPool_ = NULL;
    package_ = NULL;
    interfaceCount_ = 0;
    interfaces_ = NULL;
    thisClass_ = NULL;
    superClass_ = NULL;
    fieldCount_ = 0;
    fields_ = NULL;
    methodCount_ = 0;
    methods_ = NULL;
    sourceFileAttribute_ = NULL;
}

Class::~Class(void)
{
    for(int i = 1; i < constantPoolCount_; i++)
        delete constantPool_[i];
    delete [] constantPool_;
    delete [] interfaces_;
    delete package_;
    delete [] fields_;
    delete [] methods_;
}

int Class::Read(istream &source)
{
    int i;

    /*
     * Class file structure:
     *
     *  dword magic;
     *  word minorVersion;
     *  word majorVersion;
     *  word constantPoolCount;
     *  ConstantInfo constantPool[constantPoolCount - 1];
     *  word accessFlags;
     *  word thisClass;
     *  word superClass;
     *  word interfaceCount;
     *  word interfaces[interfaceCount];
     *  word fieldCount;
     *  FieldInfo fields[fieldCount];
     *  word methodCount;
     *  MethodInfo methods[methodCount];
     *  word attributeCount;
     *  AttributeInfo attributes[attributeCount];           
     */
       
    Dword magic;
    if(!magic.Read(source))
        return 0;
    if(magic != (dword) 0xCAFEBABE)
        return SetError();
    
    if(!minorVersion_.Read(source) || !majorVersion_.Read(source))
        return 0;

    // Read constantPool
    Word count;
    if(!count.Read(source))
        return 0;
    constantPoolCount_ = count;
    if(!(constantPool_ = new ConstantInfo*[constantPoolCount_]))
        return 0;
    memset(constantPool_, 0, constantPoolCount_ * sizeof(ConstantInfo *));
    for(i = 1; i < constantPoolCount_; i++) {
        constantPool_[i] = ConstantInfo::Read(source, this);
        if(ErrorSet()) {
            DumpError();
            return 0;
        }
    }

    // Read accessFlags
    if(!access_.Read(source))
        return 0;
    
    // Read thisClass
    Word index;
    if(!index.Read(source))
        return 0;
    thisClass_ = Constant((int) index);
        
    // Read superClass
    if(!index.Read(source))
        return 0;
    superClass_ = constantPool_[(int) index];       
    
    // Read interfaces
    if(!count.Read(source))
        return 0;
    interfaceCount_ = count;
    if(interfaceCount_ > 0) {
        if(!(interfaces_ = new ConstantInfo*[interfaceCount_]))
            return 0;
        for(i = 0; i < interfaceCount_; i++) {
            if(!index.Read(source))
                return 0;
            interfaces_[i] = constantPool_[(int) index];
        }
    }
    
    // Read fields
    if(!count.Read(source))
        return 0;
    fieldCount_ = count;
    if(fieldCount_ > 0) {
        if(!(fields_ = new FieldInfo[fieldCount_]))
            return 0;
        for(i = 0; i < fieldCount_; i++) {
            if(!fields_[i].Read(source, this))
                return 0;
        }
    }

    // Read methods
    if(!count.Read(source))
        return 0;
    methodCount_ = count;
    if(methodCount_ > 0) {
        if(!(methods_ = new MethodInfo[methodCount_]))
            return 0;
        for(i = 0; i < methodCount_; i++) {
            if(!methods_[i].Read(source, this))
                return 0;
        }
    }

    // Read attributes
    if(!count.Read(source))
        return 0;
    attributeCount_ = count;
    /*
     * AttributeInfo structure:
     *  word attributeName;
     *  dword attributeLength;
     *  byte info[attributeLength];
     * The only attribute supported for classes is the SourceFile attribute.
     */
    for(i = 0; i < attributeCount_; i++) {
        if(!index.Read(source))
            return 0;
        Dword attributeLength;
        if(!attributeLength.Read(source))
            return 0;
        char *attrName = (char *) *(Constant(index));
        if(attrName && !strcmp(attrName, "SourceFile")) {
            if(!index.Read(source))
                return 0;
            sourceFileAttribute_ = Constant((int) index);
        }
        else
            // Ignore any other attributes.
            source.ignore((int) attributeLength);
        delete attrName;
    }

    return 1;
}

ConstantInfo *Class::Constant(int index)
{
    if(index < 1 || index >= constantPoolCount_)
        return (ConstantInfo *) 0;
    return constantPool_[index];
}

char *Class::ExportInternalClassName(char *internalName)
{
    string str(internalName);
    std::replace(str.begin(), str.end(), '/', '.');
    return strnew((char *) str.c_str());
}

void Class::Dump(ostream &sink)
{
    int i;

    sink << "ConstantPool:\r\n";
    sink << IncreaseIndent;
    for(i = 1; i < constantPoolCount_; i++) {
        sink << Indent << i << ": ";
        constantPool_[i]->Dump(sink);
        sink << "\r\n";
    }    
    sink << DecreaseIndent;

    sink << "thisClass:\r\n";
    sink << IncreaseIndent;
    if(thisClass_) {
        thisClass_->Dump(sink);
        sink << "\r\n";
    }
    sink << DecreaseIndent;

    sink << "superClass:\r\n";
    sink << IncreaseIndent;
    if(superClass_) {
        superClass_->Dump(sink);
        sink << "\r\n";
    }
    sink << DecreaseIndent;

    sink << "Interfaces:\r\n";
    sink << IncreaseIndent;
    for(i = 0; i < interfaceCount_; i++) {
        interfaces_[i]->Dump(sink);
        sink << "\r\n";
    } 
    sink << DecreaseIndent;

    sink << "Fields:\r\n";
    sink << IncreaseIndent;
    for(i = 0; i < fieldCount_; i++) {
        fields_[i].Dump(sink);
        sink << "\r\n";
    }
    sink << DecreaseIndent;

    sink << "Methods:\r\n";
    sink << IncreaseIndent;
    for(i = 0; i < methodCount_; i++) {
        methods_[i].Dump(sink);
        sink << "\r\n";
    }
    sink << DecreaseIndent;

    sink << "SourceFile:\r\n";
    sink << IncreaseIndent;
    if(sourceFileAttribute_) {
        sourceFileAttribute_->Dump(sink);
        sink << "\r\n";
    }
    sink << DecreaseIndent;
}

