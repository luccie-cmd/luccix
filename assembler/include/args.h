#pragma once
#include "util.h"

typedef enum luccix_args_out_format{
    INVALID,
    BINARY,
    ORION,
    ELF,
} luccix_args_out_format;

typedef struct luccix_args{
    const char* outFile;
    const char* inputFile;
    luccix_list(const char*) inputFiles;
    luccix_args_out_format outputFormat;
    int verbose : 1;
} luccix_args;

luccix_args* parse_args(int argc, char** argv);
void print_args(luccix_args* args);