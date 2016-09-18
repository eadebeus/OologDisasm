#include "stdafx.h"
#include "ConstantInfo.h"


Class *ConstantInfo::cls_ = (Class *) 0;

ConstantInfo *ConstantInfo::Read(istream &source, Class *cls)
{
    Byte tag;

    cls_ = cls;
    if(!tag.Read(source))
        return (ConstantInfo *) SetError();
    switch((int) tag) {
        case C_UTF8:
            return new ConstantUTF8Info(source);
        case C_UNICODE:
            return new ConstantUnicodeInfo(source);
        case C_INTEGER:
            return new ConstantIntegerInfo(source);
        case C_FLOAT:
            return new ConstantFloatInfo(source);
        case C_LONG:
            return new ConstantLongInfo(source);
        case C_DOUBLE:
            return new ConstantDoubleInfo(source);
        case C_CLASS:
            return new ConstantClassInfo(source);
        case C_STRING:
            return new ConstantStringInfo(source);
        case C_FIELDREF:
            return new ConstantFieldRefInfo(source);
        case C_METHODREF:
            return new ConstantMethodRefInfo(source);
        case C_INTERFACEMETHODREF:
            return new ConstantInterfaceMethodRefInfo(source);
        case C_NAMEANDTYPE:
            return new ConstantNameAndTypeInfo(source);
        default:
            return (ConstantInfo *) SetError();
    }
}

ConstantUTF8Info::ConstantUTF8Info(istream &source)
{
    Word length;
    if(!length.Read(source))
        RetError();
    length_ = length;
    if(!(bytes_ = new byte[length_ + 1]))
        RetError();
    if(source.read((char *) bytes_, length_) && source.gcount() == length_) {
        bytes_[length_] = '\0';
    }
    else {
        delete bytes_;
        bytes_ = NULL;
        length_ = 0;
        if(source.eof())
            RetError();
        RetError();
    }
}

ConstantUTF8Info::~ConstantUTF8Info(void)
{
    delete bytes_;
}

void ConstantUTF8Info::Dump(ostream &sink)
{
    sink << "UTF8 '";
    for(int i = 0; i < length_; i++)
        sink << bytes_[i];      
    sink << "'";
}

ConstantUTF8Info::operator char *() const
{
    char *s;
    if(!(s = new char[length_ + 1]))
        return (char *) 0;
    memcpy(s,  bytes_, length_);
    s[length_] = '\0';
    return s;
}

ConstantUnicodeInfo::ConstantUnicodeInfo(istream &source)
{
    Word length;
    if(!length.Read(source))
        RetError();
    length_ = length;
    if(!(words_ = new word[length_ + 1]))
        RetError();
    if(source.read((char *) words_, length_*2) && source.gcount() == length_*2) {
        words_[length_] = (word) '\0';
    }
    else {
        delete words_;
        words_ = NULL;
        length_ = 0;
        if(source.eof())
            RetError();
        RetError();
    }
}

ConstantUnicodeInfo::~ConstantUnicodeInfo(void)
{
    delete words_;
}

void ConstantUnicodeInfo::Dump(ostream &sink)
{
    sink << "Unicode '";
    for(int i = 0; i < length_; i++)
        sink << words_[i];      
    sink << "'";
}

ConstantUnicodeInfo::operator char *() const
{
    char *s = new char[length_*2 + 1];
    if(!s)
        return (char *) 0;
    memcpy(s, (char *) words_, length_*2);
    s[length_*2] = '\0';
    return s;
}

ConstantIntegerInfo::ConstantIntegerInfo(istream &source)
{
    Dword value;
    if(!value.Read(source))
        RetError();
    bytes_ = value;
    if(!(descriptor_ = Descriptor()))
        RetError();
}

void ConstantIntegerInfo::Dump(ostream &sink)
{
    sink << "Integer " << bytes_;
}

ConstantIntegerInfo::operator char *() const
{
    string s;

    if(!strcmp(descriptor_, "Z")) {
        if(bytes_ != 0)
            return strnew("true");
        else
            return strnew("false");
    }
    else if(strcmp(descriptor_, "C")) {
        s = std::to_string(bytes_);
        return strnew((char *) s.c_str());
    }
    s = "'";
    switch(bytes_) {
        case 0:
            s = s + "\\0"; break;
        case 8:
            s = s + "\\b"; break;
        case 9:
            s = s + "\\t"; break;
        case 10:
            s = s + "\\n"; break;
        case 12:
            s = s + "\\f"; break;
        case 13:
            s = s + "\\r"; break;
        case 39:
            s = s + "\\'"; break;
        case 92:
            s = s + "\\\\"; break;
        default:
            if(bytes_ > 32 && bytes_ < 127)
                s = s + std::to_string((char)bytes_);
            else {
                stringstream ss;
                ss << std::hex << bytes_;
                s = s + ss.str();
            }
            break;
    }
    s = s + "'";
    return strnew((char *) s.c_str());
}

char *ConstantIntegerInfo::Descriptor(void)
{
    return strnew("I");
}

ConstantFloatInfo::ConstantFloatInfo(istream &source)
{
    Dword value;
    if(!value.Read(source))
        RetError();
    bytes_ = value;
}

void ConstantFloatInfo::Dump(ostream &sink)
{
    sink << "Float " << (float) bytes_;
}

ConstantFloatInfo::operator char *() const
{
    string s = std::to_string((float)bytes_);
    return strnew((char *) s.c_str());
}

char *ConstantFloatInfo::Descriptor(void)
{
    return strnew("F");
}

ConstantLongInfo::ConstantLongInfo(istream &source)
{
    Dword high, low;
    if(!high.Read(source) || !low.Read(source))
        RetError();
    high_ = high;
    low_ = low;
}

void ConstantLongInfo::Dump(ostream &sink)
{
    sink << "Long " << "hi: " << high_ << " lo: " << low_;
}

ConstantLongInfo::operator char *() const
{
    stringstream ss;
    ss << "(" << std::hex << high_ << " << 32) + " << std::hex << low_;
    string s = ss.str();
    return strnew((char *)s.c_str());
}

char *ConstantLongInfo::Descriptor(void)
{
    return strnew("J");
}

ConstantDoubleInfo::ConstantDoubleInfo(istream &source)
{
    Dword high, low;
    if(!high.Read(source) || !low.Read(source))
        RetError();
    high_ = high;
    low_ = low;
}

void ConstantDoubleInfo::Dump(ostream &sink)
{
    sink << "Double " << (((double) high_) * 0xffffffffL) + (double) low_;
}

ConstantDoubleInfo::operator char *() const
{
    string s = std::to_string((((double)high_) * 0xffffffffL) + (double)low_);
    return strnew((char *) s.c_str());
}

char *ConstantDoubleInfo::Descriptor(void)
{
    return strnew("D");
}

ConstantClassInfo::ConstantClassInfo(istream &source)
{
    Word value;
    if(!value.Read(source))
        RetError();
    nameIndex_ = value;
}

void ConstantClassInfo::Dump(ostream &sink)
{
    sink << Indent << "Class(" << nameIndex_ << ":";
    ConstantInfo *info = cls_->Constant(nameIndex_);
    if(info)
        info->Dump(sink);
    else
        SetError();
    sink << ")";
}

ConstantClassInfo::operator char *() const
{
    char *className = (char *) *(cls_->Constant(nameIndex_));
    char *s = cls_->ExportInternalClassName(className);
    delete className;
    return s;
}

char *ConstantClassInfo::Descriptor(void)
{
    return (char *) *(cls_->Constant(nameIndex_));
}

ConstantStringInfo::ConstantStringInfo(istream &source)
{
    Word value;
    if(!value.Read(source))
        RetError();
    stringIndex_ = value;
}

void ConstantStringInfo::Dump(ostream &sink)
{
    sink << "String(" << stringIndex_ << ":";
    cls_->Constant(stringIndex_)->Dump(sink);
    sink << ")";
}

ConstantStringInfo::operator char *() const
{
    char *str = (char *) *(cls_->Constant(stringIndex_)); 

    string s = "\"";
    int len = strlen(str);
    for(int i = 0; i < len; ) {
        int c = str[i++];
        if(c >= 0x7f) {
            if(c < 0x00e0) {
                c &= 0x1f;
                c <<= 6;
                c |= str[i++] & 0x3f;
            }
            else {
                c &= 0x0f;
                c <<= 6;
                c |= str[i++] & 0x3f;
                c <<= 6;
                c |= str[i++] & 0x3f;
            }
        }
        switch(c) {
            case 0:
                s = s + "\\0"; break;
            case 8:
                s = s + "\\b"; break;
            case 9:
                s = s + "\\t"; break;
            case 10:
                s = s + "\\n"; break;
            case 12:
                s = s + "\\f"; break;
            case 13:
                s = s + "\\r"; break;
            case 39:
                s = s + "\\'"; break;
            case 92:
                s = s + "\\\\"; break;
            default:
                if (c > 32 && c < 127)
                    s = s + std::to_string(c);
                else {
                    stringstream ss;
                    ss << std::hex << c;
                    s = s + ss.str();
                }
                break;
        }
    }

    delete[] str;
    return strnew((char *) s.c_str());
}

char *ConstantStringInfo::Descriptor(void)
{    
    return strnew("Ljava/lang/String;");
}

ConstantFieldRefInfo::ConstantFieldRefInfo(istream &source)
{
    Word cls, name;
    if(!cls.Read(source) || !name.Read(source))
        RetError();
    classIndex_ = cls;
    nameAndTypeIndex_ = name;
}

void ConstantFieldRefInfo::Dump(ostream &sink)
{
    sink << "FieldRef(" << classIndex_ << ":";
    cls_->Constant(classIndex_)->Dump(sink);
    sink << ",";
    sink << nameAndTypeIndex_ << ":";
    cls_->Constant(nameAndTypeIndex_)->Dump(sink);
    sink << ")";
}

ConstantFieldRefInfo::operator char *() const
{
    return (char *) *(cls_->Constant(nameAndTypeIndex_));
}

char *ConstantFieldRefInfo::Descriptor(void)
{
    return cls_->Constant(nameAndTypeIndex_)->Descriptor();
}

ConstantMethodRefInfo::ConstantMethodRefInfo(istream &source)
{
    Word cls, name;
    if(!cls.Read(source) || !name.Read(source))
        RetError();
    classIndex_ = cls;
    nameAndTypeIndex_ = name;
}

void ConstantMethodRefInfo::Dump(ostream &sink)
{
    sink << "MethodRef(" << classIndex_ << ":";
    cls_->Constant(classIndex_)->Dump(sink);
    sink << ",";
    sink << nameAndTypeIndex_ << ":";
    cls_->Constant(nameAndTypeIndex_)->Dump(sink);
    sink << ")";
}

ConstantMethodRefInfo::operator char *() const
{
    return (char *) *(cls_->Constant(nameAndTypeIndex_));
}

char *ConstantMethodRefInfo::Descriptor(void)
{    
    return cls_->Constant(nameAndTypeIndex_)->Descriptor();
}

ConstantInfo *ConstantMethodRefInfo::Class(void)
{
    return cls_->Constant(classIndex_);
}

ConstantInterfaceMethodRefInfo::ConstantInterfaceMethodRefInfo(istream &source)
{
    Word cls, name;
    if(!cls.Read(source) || !name.Read(source))
        RetError();
    classIndex_ = cls;
    nameAndTypeIndex_ = name;
}

void ConstantInterfaceMethodRefInfo::Dump(ostream &sink)
{
    sink << "InterfaceMethodRef(" << classIndex_ << ":";
    cls_->Constant(classIndex_)->Dump(sink);
    sink << ",";
    sink << nameAndTypeIndex_ << ":";
    cls_->Constant(nameAndTypeIndex_)->Dump(sink);
    sink << ")";
}

ConstantInterfaceMethodRefInfo::operator char *() const
{
    return (char *) *(cls_->Constant(nameAndTypeIndex_)); 
}

ConstantNameAndTypeInfo::ConstantNameAndTypeInfo(istream &source)
{
    Word cls, name;
    if(!cls.Read(source) || !name.Read(source))
        RetError();
    classIndex_ = cls;
    descriptorIndex_ = name;
}

void ConstantNameAndTypeInfo::Dump(ostream &sink)
{
    sink << "NameAndType(" << classIndex_ << ":";
    cls_->Constant(classIndex_)->Dump(sink);
    sink << ",";
    sink << descriptorIndex_ << ":";
    cls_->Constant(descriptorIndex_)->Dump(sink);
    sink << ")";
}

ConstantNameAndTypeInfo::operator char *() const
{
    return (char *) *(cls_->Constant(classIndex_));
}

char *ConstantNameAndTypeInfo::Descriptor(void)
{
    return (char *) *(cls_->Constant(descriptorIndex_));
}