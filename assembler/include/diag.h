#pragma once
#include <stdarg.h>

typedef enum luccix_diagnostic_level {
    DIAG_LEVEL_INVALID,
    DIAG_LEVEL_NOTE,
    DIAG_LEVEL_WARNING,
    DIAG_LEVEL_ERROR,
    DIAG_LEVEL_ICE,
} luccix_diagnostic_level;

typedef struct luccix_diagnostic{
    int useColor;
    int verbose;
    void (*vprint)(struct luccix_diagnostic* this, const char* fmt, va_list args);
    void (*printVerbose)(struct luccix_diagnostic* this, const char* fmt, ...);
    void (*print)(struct luccix_diagnostic* this, luccix_diagnostic_level level, const char* fmt, ...);
} luccix_diagnostic;

luccix_diagnostic* diagnostic_from_args(int useColor, int verbose);