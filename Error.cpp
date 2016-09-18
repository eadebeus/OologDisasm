#include "stdafx.h"
#include "Basic.h"
#include "Error.h"

bool errorSet = false;
static vector<string> errors;

int DisplayError(char *fmt, ...)
{
    va_list marker;
    va_start(marker, fmt);
    printf(fmt, marker);
    printf("\r\n");
    va_end(marker);
    return 0;
}

int SetErrorInfo(int line, char *file)
{
    errorSet = true;

    string error = string("Error: ") + std::to_string(line) + string(", ") + string(file);
    errors.push_back(error);

    return 0;
}

bool ErrorSet(void)
{
    return errorSet;
}

void ClearError(void)
{
    errorSet = false;
    errors.clear();
}

void DumpError(void)
{
    for(int i = 0; i < (int) errors.size(); i++)
        cout << errors.at(i);
}
