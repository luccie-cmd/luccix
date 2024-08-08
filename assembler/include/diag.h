#pragma once
#include <stdarg.h>
#include "util.h"

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
    luccix_assembler_list(const char*) stackTraceList;
    void (*vprint)(struct luccix_diagnostic* this, const char* fmt, va_list args);
    void (*printVerbose)(struct luccix_diagnostic* this, const char* fmt, ...);
    void (*print)(struct luccix_diagnostic* this, luccix_diagnostic_level level, const char* fmt, ...);
    void (*addTrace)(struct luccix_diagnostic* this, const char* functionName);
    void (*popTrace)(struct luccix_diagnostic* this);
    void (*printTrace)(struct luccix_diagnostic* this);
} luccix_diagnostic;

luccix_diagnostic* diagnostic_from_args(int useColor, int verbose);