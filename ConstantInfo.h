#define C_UTF8                  1
#define C_UNICODE               2
#define C_INTEGER               3
#define C_FLOAT                 4
#define C_LONG                  5
#define C_DOUBLE                6
#define C_CLASS                 7
#define C_STRING                8
#define C_FIELDREF              9
#define C_METHODREF            10
#define C_INTERFACEMETHODREF   11
#define C_NAMEANDTYPE          12

class Class;

class ConstantInfo {
protected:
    static Class *cls_;
public:
    ConstantInfo(void) {};
    virtual ~ConstantInfo(void) {};
    ConstantInfo& operator =(ConstantInfo *another) { return *this = *another; };
    static ConstantInfo *Read(istream &source, Class *);
    virtual void Dump(ostream &sink) {};
    virtual operator char *() const { return (char *) 0; };
    virtual char *Descriptor(void) { return (char *) 0; };
};

class ConstantUTF8Info : public ConstantInfo {
private:
    word length_;
    byte *bytes_;
public:
    ConstantUTF8Info(void) {};
    ~ConstantUTF8Info(void);
    ConstantUTF8Info(istream &source);
    void Dump(ostream &sink);
    operator char *() const;
};

class ConstantUnicodeInfo : public ConstantInfo {
private:
    word length_;
    word *words_;
public:
    ConstantUnicodeInfo(void) {};
    ~ConstantUnicodeInfo(void);
    ConstantUnicodeInfo(istream &source);
    void Dump(ostream &sink);
    operator char *() const;
};

class ConstantIntegerInfo : public ConstantInfo {
private:
    dword bytes_;
    char *descriptor_;
public:
    ConstantIntegerInfo(void) { descriptor_ = (char *) 0; };
    ~ConstantIntegerInfo(void) { delete [] descriptor_; };
    ConstantIntegerInfo(istream &source);
    void Dump(ostream &sink);
    operator char *() const;
    char *Descriptor(void);
};

class ConstantFloatInfo : public ConstantInfo {
private:
    dword bytes_;
public:
    ConstantFloatInfo(void) {};
    ~ConstantFloatInfo(void) {};
    ConstantFloatInfo(istream &source);
    void Dump(ostream &sink);
    operator char *() const;
    char *Descriptor(void);
};

class ConstantLongInfo : public ConstantInfo {
private:
    dword high_;
    dword low_;
public:
    ConstantLongInfo(void) {};
    ~ConstantLongInfo(void) {};
    ConstantLongInfo(istream &source);
    void Dump(ostream &sink);
    operator char *() const;
    char *Descriptor(void);
};

class ConstantDoubleInfo : public ConstantInfo {
private:
    dword high_;
    dword low_;
public:
    ConstantDoubleInfo(void) {};
    ~ConstantDoubleInfo(void) {};
    ConstantDoubleInfo(istream &source);
    void Dump(ostream &sink);
    operator char *() const;
    char *Descriptor(void);
};

class ConstantClassInfo : public ConstantInfo {
private:
    word nameIndex_;
public:
    ConstantClassInfo(void) {};
    ~ConstantClassInfo(void) {};
    ConstantClassInfo(istream &source);
    void Dump(ostream &sink);
    operator char *() const;
    char *Descriptor(void);
};

class ConstantStringInfo : public ConstantInfo {
private:
    word stringIndex_;
public:
    ConstantStringInfo(void) {};
    ~ConstantStringInfo(void) {};
    ConstantStringInfo(istream &source);
    void Dump(ostream &sink);
    operator char *() const;
    char *Descriptor(void);
};

class ConstantFieldRefInfo : public ConstantInfo {
private:
    word classIndex_;
    word nameAndTypeIndex_;
public:
    ConstantFieldRefInfo(void) {};
    ~ConstantFieldRefInfo(void) {};
    ConstantFieldRefInfo(istream &source);
    void Dump(ostream &sink);
    operator char *() const;
    char *Descriptor(void);
};

class ConstantMethodRefInfo : public ConstantInfo {
private:
    word classIndex_;
    word nameAndTypeIndex_;
public:
    ConstantMethodRefInfo(void) {};
    ~ConstantMethodRefInfo(void) {};
    ConstantMethodRefInfo(istream &source);
    void Dump(ostream &sink);
    operator char *() const;
    char *Descriptor(void);
    ConstantInfo *Class(void);
};

class ConstantInterfaceMethodRefInfo : public ConstantInfo {
private:
    word classIndex_;
    word nameAndTypeIndex_;
public:
    ConstantInterfaceMethodRefInfo(void) {};
    ~ConstantInterfaceMethodRefInfo(void) {};
    ConstantInterfaceMethodRefInfo(istream &source);
    void Dump(ostream &sink);
    operator char *() const;
};

class ConstantNameAndTypeInfo : public ConstantInfo {
private:
    word classIndex_;
    word descriptorIndex_;
public:
    ConstantNameAndTypeInfo(void) {};
    ~ConstantNameAndTypeInfo(void) {};
    ConstantNameAndTypeInfo(istream &source);
    void Dump(ostream &sink);
    operator char *() const;
    char *Descriptor(void);
};

#endif
