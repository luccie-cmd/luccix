#pragma once
#include "util.h"

typedef enum luccix_assembler_args_out_format{
    INVALID,
    BINARY,
    ORION,
    ELF,
} luccix_assembler_args_out_format;

typedef struct luccix_assembler_args{
    const char* outFile;
    const char* inputFile;
    util_da(const char*) inputFiles;
    luccix_assembler_args_out_format outputFormat;
    int verbose : 1;
    int useColor : 1;
} luccix_assembler_args;

luccix_assembler_args* parse_args(int argc, char** argv);
void print_args(luccix_assembler_args* args);