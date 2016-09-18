#ifndef _BASIC_H
#define _BASIC_H

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <strstream>
#include <algorithm>

using namespace std;

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned long dword;


// Utilities
extern ostream& IncreaseIndent(ostream& stream);
extern ostream& DecreaseIndent(ostream& stream);
extern ostream& Indent(ostream& stream);

char *strnew(char *);

class Byte {
private:
    byte value_;
public:
    Byte(void) {};
    ~Byte(void) {};
    operator byte() const;
    Byte& operator =(byte);
    int Read(istream &);
};


class Word {
private:
    word value_;
public:
    Word(void) {};
    ~Word(void) {};
    operator word() const;
    Word& operator =(word);
    int Read(istream &);
};


class Dword {
private:
    dword value_;
public:
    Dword(void) {};
    ~Dword(void) {};
    operator dword() const;
    Dword& operator =(dword);
    int Read(istream &);
};


class String {
private:
    int size_;
    char *value_;

    int Size_(void) const;
    int Size_(int);
    int Read_(const char *);
    int Read_(istream &);
    int Append_(const String &);

public:
    String(void);
    ~String(void);
    operator const char *() const;
    String& operator =(const char *);
    int operator ==(const String &) const;
    int operator !=(const String &) const;
    String& operator +(const String &another);
    int Length(void) const;
    // Reads from istream, including length word; returns number of bytes read if successful, 0 otherwise.
    int Read(istream &);
};

#endif

