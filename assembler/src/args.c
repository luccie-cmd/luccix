#include <args.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// TODO(luccie): Handle output format (elf, binary, orion)

static void check_args(luccix_args *args){
    if(args->outFile == NULL){
        args->outFile = "./a.out";
    }
    if(luccix_list_len(args->inputFiles) == 0){
        printf("ERROR: No input file was provided\n");
        free(args);
        exit(1);
    } else if(luccix_list_len(args->inputFiles) >= 2){
        printf("ERROR: Can only handle 1 input file at a time\n");
        free(args);
        exit(1);
    }
    args->inputFile = args->inputFiles.elements[0];
}

luccix_args* parse_args(int argc, char** argv){
    luccix_args *args = malloc(sizeof(args[0]));
    luccix_list_init(args->inputFiles);
    int i = 1;
    for(; i < argc;){
        if(strcmp(argv[i], "-v") == 0){
            args->verbose = 1;
        } else if(strcmp(argv[i], "-o") == 0){
            args->outFile = argv[++i];
        } else{
            luccix_list_push(args->inputFiles, argv[i]);
        }
        i++;
    }
    check_args(args);
    return args;
}

void print_args(luccix_args* args){
    printf("Output file %s\n", args->outFile);
    printf("Input file %s\n", args->inputFile);
    // TODO(luccie): Make this report a string
    // TODO(luccie): Make this report a string
    // TODO(luccie): Make this report a string
    // TODO(luccie): Make this report a string
    // TODO(luccie): Make this report a string
    printf("Output file format %d\n", args->outputFormat);
    printf("Verbose on %s\n", args->verbose ? "Yes" : "No");
}