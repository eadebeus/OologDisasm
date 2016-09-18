#ifndef _ERROR_H
#define _ERROR_H

#include <stdio.h>

int SetErrorInfo(int line, char *file);
bool ErrorSet(void);
void ClearError(void);
void DumpError(void);
int DisplayError(char *fmt, ...);

#define SetError() SetErrorInfo(__LINE__, __FILE__)
#define RetError() { SetError(); return; }

#endif
