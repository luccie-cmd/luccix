#pragma once
#include <stddef.h>

// Define the list struct as just a pointer to T
#define luccix_assembler_list(T) T*
#define luccix_assembler_list_len(L) (*(size_t*)((char*)(L) - sizeof(size_t) * 2))
#define luccix_assembler_list_capacity(L) (*(size_t*)((char*)(L) - sizeof(size_t)))
#define luccix_assembler_list_idx(L, I) (L)[I]

// Function to initialize the list
#define luccix_assembler_list_init(L) \
    do { \
        size_t* temp = malloc(sizeof(size_t) * 2 + sizeof(*(L)) * 2); \
        (L) = (void*)(temp + 2); \
        luccix_assembler_list_len(L) = 0; \
        luccix_assembler_list_capacity(L) = 2; \
    } while (0)

// Function to push an element to the list
#define luccix_assembler_list_push(L, E) \
    do { \
        if (luccix_assembler_list_len(L) >= luccix_assembler_list_capacity(L)) { \
            size_t new_capacity = luccix_assembler_list_capacity(L) * 2; \
            size_t* temp = realloc((size_t*)((char*)(L) - sizeof(size_t) * 2), sizeof(size_t) * 2 + sizeof(*(L)) * new_capacity); \
            (L) = (void*)(temp + 2); \
            luccix_assembler_list_capacity(L) = new_capacity; \
        } \
        (L)[luccix_assembler_list_len(L)++] = (E); \
    } while (0)

// Function to free the list
#define luccix_assembler_list_free(L) \
    do { \
        free((size_t*)((char*)(L) - sizeof(size_t) * 2)); \
        (L) = NULL; \
    } while (0)

char* readFile(const char* file_path);