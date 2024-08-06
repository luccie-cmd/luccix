#pragma once
#include <stddef.h>

#define luccix_assembler_list(T) struct { T* elements; size_t count, capacity; }
#define luccix_assembler_list_len(L) L.count
#define luccix_assembler_list_idx(L, I) L.elements[I]
#define luccix_assembler_list_init(L) L.elements = malloc(sizeof(L.elements[0])); L.capacity = 1; L.count = 0
#define luccix_assembler_list_push(L, E) if(luccix_assembler_list_len(L) >= L.capacity) \
                                { \
                                   L.capacity *= 2; \
                                   L.elements = realloc(L.elements, L.capacity);  \
                              } \
                              L.elements[L.count++] = E

char* readFile(const char* file_path);