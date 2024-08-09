/*
This software is available under 2 licenses -- choose whichever you prefer.
------------------------------------------------------------------------------
ALTERNATIVE A - MIT License
Copyright (c) 2023 Local Atticus
Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
------------------------------------------------------------------------------
ALTERNATIVE B - Public Domain (www.unlicense.org)
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
software, either in source code form or as a compiled binary, for any purpose,
commercial or non-commercial, and by any means.
In jurisdictions that recognize copyright laws, the author or authors of this
software dedicate any and all copyright interest in the software to the public
domain. We make this dedication for the benefit of the public at large and to
the detriment of our heirs and successors. We intend this dedication to be an
overt act of relinquishment in perpetuity of all present and future rights to
this software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef UTIL_H
#define UTIL_H

#define UTIL_INCLUDED

#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(UTIL_MALLOC) || defined(UTIL_REALLOC) || defined(UTIL_DEALLOC)
#    if !defined(UTIL_MALLOC) || !defined(UTIL_REALLOC) || !defined(UTIL_DEALLOC)
#        error "The LCA library requires all three of UTIL_MALLOC, UTIL_REALLOC and UTIL_DEALLOC to be defined if at least one is."
#    endif
#else
#    define UTIL_MALLOC(N)     (util_allocate(util_default_allocator, N))
#    define UTIL_REALLOC(P, N) (util_reallocate(util_default_allocator, P, N))
#    define UTIL_FREE(P)       (util_deallocate(util_default_allocator, P))
#endif

#define ANSI_COLOR_RESET             "\x1b[0m"
#define ANSI_COLOR_BLACK             "\x1b[30m"
#define ANSI_COLOR_RED               "\x1b[31m"
#define ANSI_COLOR_GREEN             "\x1b[32m"
#define ANSI_COLOR_YELLOW            "\x1b[33m"
#define ANSI_COLOR_BLUE              "\x1b[34m"
#define ANSI_COLOR_MAGENTA           "\x1b[35m"
#define ANSI_COLOR_CYAN              "\x1b[36m"
#define ANSI_COLOR_WHITE             "\x1b[37m"
#define ANSI_COLOR_BRIGHT_BLACK      "\x1b[30;1m"
#define ANSI_COLOR_BRIGHT_RED        "\x1b[31;1m"
#define ANSI_COLOR_BRIGHT_GREEN      "\x1b[32;1m"
#define ANSI_COLOR_BRIGHT_YELLOW     "\x1b[33;1m"
#define ANSI_COLOR_BRIGHT_BLUE       "\x1b[34;1m"
#define ANSI_COLOR_BRIGHT_MAGENTA    "\x1b[35;1m"
#define ANSI_COLOR_BRIGHT_CYAN       "\x1b[36;1m"
#define ANSI_COLOR_BRIGHT_WHITE      "\x1b[37;1m"
#define ANSI_BG_COLOR_BLACK          "\x1b[40m"
#define ANSI_BG_COLOR_RED            "\x1b[41m"
#define ANSI_BG_COLOR_GREEN          "\x1b[42m"
#define ANSI_BG_COLOR_YELLOW         "\x1b[43m"
#define ANSI_BG_COLOR_BLUE           "\x1b[44m"
#define ANSI_BG_COLOR_MAGENTA        "\x1b[45m"
#define ANSI_BG_COLOR_CYAN           "\x1b[46m"
#define ANSI_BG_COLOR_WHITE          "\x1b[47m"
#define ANSI_BG_COLOR_BRIGHT_BLACK   "\x1b[40;1m"
#define ANSI_BG_COLOR_BRIGHT_RED     "\x1b[41;1m"
#define ANSI_BG_COLOR_BRIGHT_GREEN   "\x1b[42;1m"
#define ANSI_BG_COLOR_BRIGHT_YELLOW  "\x1b[43;1m"
#define ANSI_BG_COLOR_BRIGHT_BLUE    "\x1b[44;1m"
#define ANSI_BG_COLOR_BRIGHT_MAGENTA "\x1b[45;1m"
#define ANSI_BG_COLOR_BRIGHT_CYAN    "\x1b[46;1m"
#define ANSI_BG_COLOR_BRIGHT_WHITE   "\x1b[47;1m"
#define ANSI_STYLE_BOLD              "\x1b[1m"
#define ANSI_STYLE_UNDERLINE         "\x1b[4m"
#define ANSI_STYLE_REVERSED          "\x1b[7m"

#define UTIL_SV_EMPTY       ((util_string_view){0})
#define UTIL_SV_CONSTANT(C) ((util_string_view){.data = (C), .count = (sizeof C) - 1})
#define UTIL_STR_EXPAND(s)  ((int)s.count), (s.data)

void util_assertion_failure_handler(const char* message, const char* expression, int line, const char* file);
#define UTIL_ASSERT(Condition, Message)                                                                  \
    do {                                                                                                \
        if (!(Condition)) { util_assertion_failure_handler((Message), #Condition, __LINE__, __FILE__); } \
    } while (0)

typedef void* (*util_allocator_function)(void* user_data, int64_t count, void* ptr);

typedef struct util_allocator {
    void* user_data;
    util_allocator_function allocator_function;
} util_allocator;

typedef struct util_arena util_arena;

extern util_allocator util_default_allocator;
extern util_allocator temp_allocator;

// TODO(local): dynamic arrays should have optional allocators
/// Header data for a light-weight implelentation of typed dynamic arrays.
typedef struct util_da_header {
    int64_t capacity;
    int64_t count;
} util_da_header;

// invariant: should always be nul terminated
typedef struct util_string {
    util_allocator allocator;
    char* data;
    int64_t count;
    int64_t capacity;
} util_string;

typedef struct util_string_view {
    const char* data;
    int64_t count;
} util_string_view;

typedef struct util_command_result {
    bool exited;
    int exit_code;
} util_command_result;

typedef struct util_command {
    const char** arguments;
    int64_t count;
    int64_t capacity;
} util_command;

typedef enum util_clopt_parser_result {
    UTIL_CLOPT_PARSE_OK,
    UTIL_CLOPT_PARSE_STOP,
} util_clopt_parser_result;

typedef enum util_clopt_flags {
    UTIL_CLOPT_NONE = 0,
    UTIL_CLOPT_ALIAS = 1 << 0,
    UTIL_CLOPT_ARG_OPTIONAL = 1 << 1,
    UTIL_CLOPT_HIDDEN = 1 << 2,
    UTIL_CLOPT_DOC = 1 << 3,
    UTIL_CLOPT_NO_USAGE = 1 << 4,
} util_clopt_flags;

typedef enum util_clopt_type {
    UTIL_CLOPT_CSTRING,
    UTIL_CLOPT_STRING_VIEW,
    UTIL_CLOPT_BOOL,
    UTIL_CLOPT_PROGRAMATIC,
} util_clopt_type;

enum {
    UTIL_CLOPT_KEY_ARG = -1,
    UTIL_CLOPT_KEY_END = -2,
    UTIL_CLOPT_KEY_PROGRAM_NAME = -3,
};

enum {
    UTIL_CLOPT_RESULT_OK = 0,
};

typedef util_clopt_parser_result (*util_clopt_parser_callback)(int key, util_string_view name, util_string_view arg, void* args_data);

typedef struct util_clopt {
    /// A unique identifier for this option.
    /// If this is a printable ASCII character, it also specifies the short option name `-char`,
    /// where `char` is the ASCII character with the code key.
    int key;

    /// The long name of this option.
    /// Can be null if this option only has a short name.
    const char* name;

    /// If nonnull, the name of the argument which should be provided with this option,
    /// for example in `--name=value` or `-c value`.
    /// Optional if the `UTIL_CLOPT_ARG_OPTIONAL` flag is set.
    const char* arg;

    /// The location within the argument storage struct to place the option value.
    /// If this option has the `UTIL_CLOPT_PROGRAMATIC` flag, this field is ignored and
    /// the option value is instead passed to the argument parser callback you provide.
    int64_t struct_offset;

    /// Flags associated with this option.
    util_clopt_flags flags;

    /// The type of the option when storing into the arguments structure at `struct_offset`, or
    /// `UTIL_CLOPT_PROGRAMATIC` to have the parser callback handle it manually.
    util_clopt_type type;

    /// A documentation string, for printing in help and usage messages.
    /// 
    /// If both key and name are null, this will be printed tabbed left from
    /// the normal option column, making it useful as a group header.
    /// This will be the first thing printed in its group.
    /// When used as a group header, it's convention to end the string with ':'.
    const char* doc;

    /// Group identity for this option.
    ///
    /// In a long help message, options are sorted alphabetically within each group.
    /// Groups are presented in the order 0, 1, 2, ..., n, -m, ..., -2, -1.
    ///
    /// Every option item with a `0` will inherit the group number of the previous entry,
    /// or zero if it's the first one. If it's a group header, with the `name` and `key` fields
    /// both zero, then the previous entry + 1 is the default instead.
    /// "Automagic" options such as `--help` are put into group -1.
    int group;
} util_clopt;

int util_clopt_parse(int argc, char** argv, util_clopt* opts, void* args_data, util_clopt_parser_callback parser_callback);
void util_clopt_write_error(util_string_view program_name, const char* format, ...);
void util_clopt_usage(util_string_view program_name, util_clopt* opts);

void util_da_maybe_expand(void** da_ref, int64_t element_size, int64_t required_count);

void* util_allocate(util_allocator allocator, int64_t n);
void util_deallocate(util_allocator allocator, void* ptr);
void* util_reallocate(util_allocator allocator, void* ptr, int64_t n);

void util_temp_allocator_init(util_allocator allocator, int64_t block_size);
void util_temp_allocator_clear(void);
void util_temp_allocator_dump(void);

char* util_temp_sprintf(const char* format, ...);
char* util_temp_vsprintf(const char* format, va_list v);

util_arena* util_arena_create(util_allocator allocator, int64_t block_size);
void util_arena_destroy(util_arena* arena);
void* util_arena_push(util_arena* arena, int64_t size);
void util_arena_clear(util_arena* arena);
void util_arena_dump(util_arena* arena);

util_string util_string_create(util_allocator allocator);
util_string util_string_from_data(util_allocator allocator, char* data, int64_t count, int64_t capacity);
void util_string_destroy(util_string* s);
char* util_string_as_cstring(util_string s);
bool util_string_equals(util_string a, util_string b);
util_string_view util_string_slice(util_string s, int64_t offset, int64_t length);
util_string util_string_format(const char* format, ...);
util_string util_string_vformat(const char* format, va_list v);
void util_string_append_format(util_string* s, const char* format, ...);
void util_string_append_vformat(util_string* s, const char* format, va_list v);
void util_string_append_rune(util_string* s, int rune);

void util_string_path_parent(util_string* string);
void util_string_path_append(util_string* path, util_string s);
void util_string_path_append_cstring(util_string* path, const char* s);
void util_string_path_append_view(util_string* path, util_string_view s);

util_string_view util_string_view_from_cstring(const char* s);
util_string_view util_string_as_view(util_string s);
util_string_view util_string_view_slice(util_string_view sv, int64_t offset, int64_t length);
bool util_string_view_equals(util_string_view a, util_string_view b);
bool util_string_view_equals_cstring(util_string_view a, const char* b);
bool util_string_view_starts_with(util_string_view a, util_string_view b);
util_string util_string_view_to_string(util_allocator allocator, util_string_view s);
char* util_string_view_to_cstring(util_allocator allocator, util_string_view s);
int64_t util_string_view_index_of(util_string_view s, char c);
int64_t util_string_view_last_index_of(util_string_view s, char c);
bool util_string_view_ends_with_cstring(util_string_view s, const char* cstr);
util_string util_string_view_change_extension(util_allocator allocator, util_string_view s, const char* new_ext);

util_string_view util_string_view_path_file_name(util_string_view s);

#define util_da(T)            T*
#define util_da_get_header(V) (((struct util_da_header*)(V)) - 1)
#define util_da_count(V)      ((V) ? util_da_get_header(V)->count : 0)
#define util_da_capacity(V)   ((V) ? util_da_get_header(V)->capacity : 0)
#define util_da_reserve(V, N) \
    do { util_da_maybe_expand((void**)&(V), (int64_t)sizeof *(V), N); } while (0)
#define util_da_count_set(V, N)                    \
    do {                                          \
        util_da_reserve(V, N);                     \
        if (V) util_da_get_header(V)->count = (N); \
    } while (0)
#define util_da_push(V, E)                                                             \
    do {                                                                              \
        util_da_maybe_expand((void**)&(V), (int64_t)sizeof *(V), util_da_count(V) + 1); \
        (V)[util_da_count(V)] = E;                                                     \
        util_da_get_header(V)->count++;                                                \
    } while (0)
#define util_da_pop(V)                                                   \
    do {                                                                \
        if (util_da_get_header(V)->count) util_da_get_header(V)->count--; \
    } while (0)
#define util_da_insert(V, I, E)                                                                 \
    assert((I) > 0);                                                                           \
    do {                                                                                       \
        util_da_count_set((V), util_da_count((V)) + 1);                                          \
        memmove(&(V)[(I) + 1], &(V)[(I)], sizeof(*(V)) * (int64_t)(util_da_count((V)) - (I)-1)); \
        (V)[(I)] = (E);                                                                        \
    } while (0)
#define util_da_back(V) (&(V)[util_da_count(V) - 1])
#define util_da_free(V)                                             \
    do {                                                           \
        if (V) {                                                   \
            memset(V, 0, (int64_t)util_da_count(V) * (sizeof *(V))); \
            UTIL_FREE(util_da_get_header(V));                        \
            (V) = NULL;                                            \
        }                                                          \
    } while (0)
#define util_da_free_all(V, F)                                                                                    \
    do {                                                                                                         \
        if (V) {                                                                                                 \
            for (int64_t util_da_index = 0; util_da_index < util_da_count(V); util_da_index++) F((V)[util_da_index]); \
            memset(V, 0, (int64_t)util_da_count(V) * (sizeof *(V)));                                               \
            UTIL_FREE(util_da_get_header(V));                                                                      \
            (V) = NULL;                                                                                          \
        }                                                                                                        \
    } while (0)

char* util_shift_args(int* argc, char*** argv);

bool util_stdout_isatty(void);
bool util_stderr_isatty(void);

bool util_file_exists(const char* file_path);
util_string util_file_read(util_allocator allocator, const char* file_path);

char* util_plat_self_exe(void);

void util_command_append1(util_command* command, const char* argument);
void util_command_append_many(util_command* command, const char** arguments, int argument_count);
#define util_command_append(C, ...) util_command_append_many(C, ((const char*[]){__VA_ARGS__}), (int)(sizeof((const char*[]){__VA_ARGS__}) / sizeof(const char*)))
util_command_result util_command_run(util_command command);

#endif // !UTIL_H
