#include "Log.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstdarg>

void error(const char* format, ...)
{
    va_list argptr;
    va_start(argptr, format);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), format, argptr);
	printf("\u001b[31m[Error]\u001b[0m %s\n", buffer);
    va_end(argptr);
    exit(-1);
}

void log(const char* format, ...)
{
    va_list argptr;
    va_start(argptr, format);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), format, argptr);
	printf("\u001b[34m[Log]\u001b[0m %s\n", buffer);
    va_end(argptr);
}
