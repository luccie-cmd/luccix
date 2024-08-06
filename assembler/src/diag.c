#include <diag.h>
#include <stdlib.h>
#include <stdio.h>
#include <ansi.h>

static const char* diagnostic_level_to_cstr(luccix_diagnostic_level level){
    switch(level){
        case DIAG_LEVEL_NOTE: {
            return ANSI_COLOR_RESET;
        } break;
        case DIAG_LEVEL_WARNING: {
            return ANSI_COLOR_BRIGHT_MAGENTA;
        } break;
        case DIAG_LEVEL_ERROR:
        case DIAG_LEVEL_ICE: {
            return ANSI_COLOR_BRIGHT_RED;
        } break;
        case DIAG_LEVEL_INVALID:
        default: {
            return "";
        } break;
    }
    return "";
}

static void vprint(struct luccix_diagnostic* this, const char* fmt, va_list args){
    (void)this;
    vprintf(fmt, args);
}
static void print(struct luccix_diagnostic* this, luccix_diagnostic_level level, const char* fmt, ...){
    if(this->useColor){
        printf("%s", diagnostic_level_to_cstr(level));
    }
    va_list args;
    va_start(args, fmt);
    this->vprint(this, fmt, args);
    va_end(args);
    if(this->useColor){
        printf("%s", ANSI_COLOR_RESET);
    }
}
static void printVerbose(struct luccix_diagnostic* this, const char* fmt, ...){
    if(!this->verbose){
        return;
    }
    va_list args;
    va_start(args, fmt);
    this->vprint(this, fmt, args);
    va_end(args);
}

luccix_diagnostic* diagnostic_from_args(int useColor, int verbose){
    luccix_diagnostic* diag = malloc(sizeof(diag[0]));
    diag->useColor = useColor;
    diag->verbose = verbose;
    diag->print = print;
    diag->vprint = vprint;
    diag->printVerbose = printVerbose;
    return diag;
}