#ifndef __DEBUG_H__
#define __DEBUG_H__
#include <stdarg.h>

void LogPrint(char *, unsigned int);
void LogPrintF(char* format, unsigned int formatLength, ...);


#define LOGF(x, ...) (LogPrintF(x, sizeof(x) - 1, __VA_ARGS__))

#endif