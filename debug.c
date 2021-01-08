/*
 * A wrapper around printf. Not thread-safe due to the global variable
 * DEBUG.
 */

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

bool DEBUG = false;

void debug_enable(bool enable)
{
    DEBUG = enable;
}

int debug_printf(char *format, ...)
{
    if (!DEBUG)
    {
        return 0;
    }

    va_list args;
    va_start(args, format);
    return printf(format, args);
}