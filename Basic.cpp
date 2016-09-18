#include "Basic.h"
#include "Error.h"


char *strnew(char *src)
{
    int n;
    char *s;
    
    n = strlen(src);
    if(!(s = new char[n + 1]))
        return (char *) 0;
    memcpy(s, src, n);
    s[n] = '\0';
    return s;
}

static int GetIndentIndex() {
    // ios_base::xalloc allocates indices for custom-storage locations.
    static int index = ios_base::xalloc();
    return index;
}

ostream& IncreaseIndent(ostream& stream) {
    // The iword(index) function gives a reference to the index-th custom storage location as a integer.
    stream.iword(GetIndentIndex())++;
    return stream;
}

ostream& DecreaseIndent(ostream& stream) {
    // The iword(index) function gives a reference to the index-th custom storage location as a integer.
    stream.iword(GetIndentIndex())--;
    return stream;
}

ostream& Indent(ostream& stream) {
    int indent = stream.iword(GetIndentIndex());
    while (indent) {
        stream.put(' ');
        stream.put(' ');
        indent--;
    }
    stream.flush();
    return stream;
}


Byte::operator byte() const
{
    return value_;
}

Byte& Byte::operator =(byte value)
{
    value_ = value;
    return *this;
}

int Byte::Read(istream& source)
{
    char value;
    if(!source.get(value)) {
        if(source.eof())
            return DisplayError("EOF reading byte.");
        else
            return DisplayError("Error reading byte.");
    }
    value_ = (byte) value;
    return 1;
}


Word::operator word() const
{
    return value_;
}

Word& Word::operator =(word value)
{
    value_ = value;
    return *this;
}

int Word::Read(istream& source)
{
    char a, b;
    if(!source.get(a) || !source.get(b)) {
        if(source.eof())
            return DisplayError("EOF reading word.");
        else
            return DisplayError("Error reading word.");
    }
    // Java is big-endian
    value_ = (((word) a) << 8) + (byte) b;
    return 1;
}


Dword::operator dword() const
{
    return value_;
}

Dword& Dword::operator =(dword value)
{
    value_ = value;
    return *this;
}

int Dword::Read(istream& source)
{
    Word a, b;
    if(!a.Read(source) || !b.Read(source)) {
        if(source.eof())
            return DisplayError("EOF reading dword.");
        else
            return DisplayError("Error readinf dword.");
    }
    // Java is big-endian
    value_ = (((dword) a) << 16) + b;
    return 1;
}


int String::Size_(void) const
{
    return size_;
}

int String::Size_(int size)
{
    char *value;
    if(!(value = new char[size + 1]))
        return 0;
    value[size] = '\0';
    delete value_;
    value_ = value;
    return size_ = size;
}

int String::Read_(const char *s) 
{
    if(!s)
        return 0;
    Size_(strlen(s));
    memcpy(value_, s, size_);
    return size_;
}

int String::Read_(istream &source)
{
    if((source.read(value_, size_)) && source.gcount() == size_)
        return size_;
    else if(source.eof())
        return DisplayError("EOF reading dword");
    else
        return 0;
}

int String::Append_(const String &another) {
    int add = another.Length();
    if(add < 0)
        return 0;
    else if(add == 0)
        return size_;
    int size;
    size = size_ + add;
    char *value;
    if(!(value = new char[size + 1]))
        return 0;
    memcpy(value, value_, (size_ < size) ? size_ : size);
    memcpy(value + size_, (const char *) another, add);
    value[size] = '\0';
    delete value_;
    value_ = value;
    return size_ = size;
}

String::String(void)
{
    size_ = 0;
    value_ = NULL;
}

String::~String(void)
{
    delete value_;
}

String::operator const char *() const
{
    return value_;
}

int String::operator ==(const String &another) const
{
    return (size_ == another.size_) ? (memcmp(value_, another.value_, size_) ? 0 : 1) : 0;
}

int String::operator !=(const String &another) const
{
    return (*this == another) ? 0 : 1;
}

String& String::operator =(const char *s) {
    Read_(s);
    return *this;
}

String& String::operator +(const String &another) {
    Append_(another);
    return *this;
}

int String::Length(void) const
{
    return Size_();
}

int String::Read(istream &source)
{
    Word size;
    return (size.Read(source) && (Size_(size) == size) && (Read_(source) == size ? 1 : Size_(0))) ? sizeof(word) + size_ : 0;
}
