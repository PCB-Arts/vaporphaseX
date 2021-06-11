
#include <unity.h>
#include <stdio.h>
#include <stdarg.h>

static char buffer[1024];

void DEBUG_TEST_OUTPUT(const char* format, ...) {
    va_list format_arguments;

    va_start(format_arguments, format);

    vsnprintf(buffer, sizeof(buffer), format, format_arguments);

    va_end(format_arguments);

    TEST_MESSAGE(buffer);
}