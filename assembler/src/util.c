#include <util.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

char* readFile(const char* path){
    FILE* f = fopen(path, "rb");
    if(!f){
        printf("ERROR: %s\n", strerror(errno));
        exit(1);
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    if(size <= 0){
        printf("ERROR: Cannot read less or equal than 0 bytes\n");
        fclose(f);
        exit(1);
    }

    char* buffer = malloc((size_t)size);
    size_t read = fread(buffer, 1, size, f);
    if(read != (size_t)size){
        printf("ERROR: %s\n", strerror(errno));
        exit(1);
    }
    buffer[size] = '\0';
    fclose(f);
    return buffer;
}