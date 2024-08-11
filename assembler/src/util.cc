// #include <util.h>
// #    include <errno.h>
// #    include <stdio.h>

// #    ifdef _WIN32
// #        define WIN32_LEAN_AND_MEAN
// #        define _WINUSER_
// #        define _WINGDI_
// #        define _IMM_
// #        define _WINCON_
// #        include <direct.h>
// #        include <io.h>
// #        include <shellapi.h>
// #        include <windows.h>
// #    else
// #        include <execinfo.h>
// #        include <fcntl.h>
// #        include <sys/stat.h>
// #        include <sys/types.h>
// #        include <sys/wait.h>
// #        include <unistd.h>
// #    endif

// void util_da_maybe_expand(void** da_ref, int64_t element_size, int64_t required_count) {
//     if (required_count <= 0) return;

//     struct util_da_header* header = util_da_get_header(*da_ref);
//     if (!*da_ref) {
//         int64_t initial_capacity = 32;
//         void* new_data = UTIL_MALLOC((sizeof *header) + (int64_t)(initial_capacity * element_size));
//         header = new_data;

//         header->capacity = initial_capacity;
//         header->count = 0;
//     } else if (required_count > header->capacity) {
//         while (required_count > header->capacity)
//             header->capacity *= 2;
//         header = UTIL_REALLOC(header, (sizeof *header) + (int64_t)(header->capacity * element_size));
//     }

//     *da_ref = (void*)(header + 1);
// }

// util_string util_string_create(util_allocator allocator) {
//     int64_t capacity = 32;
//     char* data = util_allocate(allocator, (int64_t)capacity * sizeof *data);
//     assert(data);
//     return (util_string){
//         .allocator = allocator,
//         .data = data,
//         .capacity = capacity,
//         .count = 0,
//     };
// }

// util_string util_string_from_data(util_allocator allocator, char* data, int64_t count, int64_t capacity) {
//     assert(data);
//     assert(capacity > 0);
//     assert(count < capacity);
//     assert(data[count] == 0);
//     return (util_string){
//         .allocator = allocator,
//         .data = data,
//         .capacity = capacity,
//         .count = count,
//     };
// }

// void util_string_destroy(util_string* s) {
//     if (s == NULL || s->data == NULL) return;
//     util_deallocate(s->allocator, s->data);
//     *s = (util_string){0};
// }

// char* util_string_as_cstring(util_string s) {
//     assert(s.count < s.capacity);
//     assert(s.data[s.count] == 0);
//     return s.data;
// }

// bool util_string_equals(util_string a, util_string b) {
//     if (a.count != b.count) return false;
//     for (int64_t i = 0; i < a.count; i++) {
//         if (a.data[i] != b.data[i])
//             return false;
//     }
//     return true;
// }

// util_string_view util_string_slice(util_string s, int64_t offset, int64_t length) {
//     assert(offset >= 0);
//     assert(length >= 0);
//     assert(offset + length <= s.count);

//     return (util_string_view){
//         .data = s.data + offset,
//         .count = length,
//     };
// }

// util_string util_string_format(const char* format, ...) {
//     va_list v;
//     va_start(v, format);
//     util_string result = util_string_vformat(format, v);
//     va_end(v);
//     return result;
// }

// util_string util_string_vformat(const char* format, va_list v) {
//     char* buffer = util_temp_vsprintf(format, v);
//     int64_t count = strlen(buffer);
//     return util_string_from_data(temp_allocator, buffer, count, count + 1);
// }

// static void util_string_ensure_capacity(util_string* s, int64_t min_capacity) {
//     if (s == NULL) return;
//     if (s->capacity >= min_capacity) return;

//     int64_t new_capacity = s->capacity;
//     if (new_capacity == 0) {
//         new_capacity = min_capacity;
//     } else {
//         while (new_capacity < min_capacity) {
//             new_capacity <<= 1;
//         }
//     }

//     s->data = util_reallocate(s->allocator, s->data, new_capacity);
//     assert(s->data != NULL);
//     s->capacity = new_capacity;
// }

// void util_string_append_format(util_string* s, const char* format, ...) {
//     assert(s != NULL);
//     va_list v;
//     va_start(v, format);
//     util_string_append_vformat(s, format, v);
//     va_end(v);
// }

// void util_string_append_vformat(util_string* s, const char* format, va_list v) {
//     assert(s != NULL);

//     va_list v1;
//     va_copy(v1, v);
//     int n = vsnprintf(NULL, 0, format, v1);
//     va_end(v1);

//     util_string_ensure_capacity(s, s->count + n + 1);
//     vsnprintf(s->data + s->count, n + 1, format, v);

//     s->count += n;
// }

// void util_string_append_rune(util_string* s, int rune) {
//     assert(s != NULL);
//     util_string_ensure_capacity(s, s->count + 1);
//     s->data[s->count] = (char)rune;
//     s->count += 1;
// }

// void util_string_path_parent(util_string* string) {
//     if (string->count > 0 && (string->data[string->count - 1] == '/' || string->data[string->count - 1] == '\\')) {
//         string->data[string->count - 1] = 0;
//         string->count--;
//     }

//     for (int64_t i = string->count - 1; i >= 0 && string->data[i] != '/' && string->data[i] != '\\'; i--) {
//         string->data[i] = 0;
//         string->count--;
//     }
// }

// void util_string_path_append_view(util_string* path, util_string_view s) {
//     assert(path != NULL);

//     bool path_ends_with_slash = path->count > 0 && (path->data[path->count - 1] == '/' || path->data[path->count - 1] == '\\');
//     bool view_starts_with_slash = s.count > 0 && (s.data[0] == '/' || s.data[0] == '\\');

//     if (path_ends_with_slash && view_starts_with_slash) {
//         s.data++;
//         s.count--;
//         view_starts_with_slash = false;
//     }

//     if (path_ends_with_slash) {
//         util_string_append_format(path, "%.*s", UTIL_STR_EXPAND(s));
//     } else {
//         util_string_append_format(path, "/%.*s", UTIL_STR_EXPAND(s));
//     }
// }

// util_string_view util_string_view_from_cstring(const char* s) {
//     return (util_string_view){
//         .data = s,
//         .count = strlen(s),
//     };
// }

// util_string_view util_string_as_view(util_string s) {
//     return (util_string_view){
//         .data = s.data,
//         .count = s.count,
//     };
// }

// util_string_view util_string_view_slice(util_string_view sv, int64_t offset, int64_t length) {
//     assert(offset >= 0 && offset <= sv.count);
//     if (length == -1) {
//         length = sv.count - offset;
//     }
//     assert(length >= 0 && length <= sv.count - offset);

//     return (util_string_view){
//         .data = sv.data + offset,
//         .count = length,
//     };
// }

// bool util_string_view_equals(util_string_view a, util_string_view b) {
//     if (a.count != b.count) return false;
//     for (int64_t i = 0; i < a.count; i++) {
//         if (a.data[i] != b.data[i])
//             return false;
//     }
//     return true;
// }

// bool util_string_view_equals_cstring(util_string_view a, const char* b) {
//     for (int64_t i = 0; i < a.count; i++) {
//         if (a.data[i] != b[i])
//             return false;
//         if (b[i] == 0)
//             return false;
//     }
//     return b[a.count] == 0;
// }

// bool util_string_view_starts_with(util_string_view a, util_string_view b) {
//     if (a.count < b.count) return false;
//     for (int64_t i = 0; i < b.count; i++) {
//         if (a.data[i] != b.data[i])
//             return false;
//     }
//     return true;
// }

// util_string util_string_view_to_string(util_allocator allocator, util_string_view s) {
//     char* data = util_allocate(allocator, s.count + 1);
//     assert(data);
//     memcpy(data, s.data, (int64_t)s.count);
//     return util_string_from_data(allocator, data, s.count, s.count + 1);
// }

// char* util_string_view_to_cstring(util_allocator allocator, util_string_view s) {
//     char* result = util_allocate(allocator, s.count + 1);
//     memcpy(result, s.data, s.count);
//     result[s.count] = 0;
//     return result;
// }

// int64_t util_string_view_index_of(util_string_view s, char c) {
//     for (int64_t i = 0; i < s.count; i++) {
//         if (s.data[i] == c) {
//             return i;
//         }
//     }

//     return -1;
// }

// int64_t util_string_view_last_index_of(util_string_view s, char c) {
//     for (int64_t i = s.count - 1; i >= 0; i--) {
//         if (s.data[i] == c) {
//             return i;
//         }
//     }

//     return -1;
// }

// bool util_string_view_ends_with_cstring(util_string_view s, const char* cstr) {
//     int64_t cstr_len = (int64_t)strlen(cstr);
//     if (cstr_len > s.count) {
//         return false;
//     }

//     return 0 == strncmp(s.data + s.count - cstr_len, cstr, (int64_t)cstr_len);
// }

// util_string util_string_view_change_extension(util_allocator allocator, util_string_view s, const char* new_ext) {
//     int64_t last_slash_index = 0;
//     for (int64_t i = last_slash_index - 1; i >= 0; i--) {
//         if (s.data[i] == '/' || s.data[i] == '\\') {
//             last_slash_index = i;
//             break;
//         }
//     }

//     int64_t last_dot_index = s.count;
//     for (int64_t i = last_dot_index - 1; i > last_slash_index; i--) {
//         if (s.data[i] == '.') {
//             last_dot_index = i;
//             break;
//         }
//     }

//     int64_t new_ext_length = strlen(new_ext);
//     util_string result = util_string_create(allocator);
//     util_string_ensure_capacity(&result, last_dot_index + new_ext_length + 1);

//     util_string_append_format(&result, "%.*s%s", (int)last_dot_index, s.data, new_ext);

//     return result;
// }

// util_string_view util_string_view_path_file_name(util_string_view s) {
//     int64_t start_index = s.count - 1;
//     while (start_index > 0) {
//         if (s.data[start_index - 1] == '/' || s.data[start_index - 1] == '\\') {
//             break;
//         }
//         start_index--;
//     }

//     s.data += start_index;
//     s.count -= start_index;

//     return s;
// }

// typedef struct util_arena_block {
//     void* memory;
//     int64_t allocated;
//     int64_t capacity;
// } util_arena_block;

// struct util_arena {
//     util_allocator allocator;
//     util_da(util_arena_block) blocks;
//     int64_t block_size;
// };

// void* util_util_default_allocator_function(void* user_data, int64_t count, void* ptr);
// void* util_temp_allocator_function(void* user_data, int64_t count, void* ptr);

// util_allocator util_default_allocator = {
//     .allocator_function = util_util_default_allocator_function
// };

// util_allocator temp_allocator = {0};

// void* util_allocate(util_allocator allocator, int64_t n) {
//     return allocator.allocator_function(allocator.user_data, n, NULL);
// }

// void util_deallocate(util_allocator allocator, void* n) {
//     allocator.allocator_function(allocator.user_data, 0, n);
// }

// void* util_reallocate(util_allocator allocator, void* ptr, int64_t n) {
//     return allocator.allocator_function(allocator.user_data, n, ptr);
// }

// void* util_util_default_allocator_function(void* user_data, int64_t count, void* ptr) {
//     (void)user_data;
//     if (count == 0) {
//         free(ptr);
//         return NULL;
//     } else if (ptr == NULL) {
//         void* data = calloc(1, count);
//         assert(data != NULL);
//         return data;
//     } else {
//         void* data = realloc(ptr, count);
//         assert(data != NULL);
//         return data;
//     }
// }

// void* util_temp_allocator_function(void* user_data, int64_t count, void* ptr) {
//     (void)user_data;
//     util_arena* temp_arena = temp_allocator.user_data;
//     assert(temp_arena != NULL && "Where did the arena go? did you init it?");
//     assert(ptr == NULL && "Cannot reallocate temp arena memory");
//     return util_arena_push(temp_arena, count);
// }

// void util_temp_allocator_init(util_allocator allocator, int64_t block_size) {
//     assert(temp_allocator.user_data == NULL);
//     temp_allocator = (util_allocator){
//         .user_data = util_arena_create(allocator, block_size),
//         .allocator_function = util_temp_allocator_function,
//     };

//     assert(temp_allocator.user_data != NULL);
// }

// void util_temp_allocator_clear(void) {
//     util_arena* temp_arena = temp_allocator.user_data;
//     util_arena_clear(temp_arena);
// }

// void util_temp_allocator_dump(void) {
//     util_arena* temp_arena = temp_allocator.user_data;
//     util_arena_dump(temp_arena);
// }

// char* util_temp_sprintf(const char* format, ...) {
//     va_list v;
//     va_start(v, format);
//     char* result = util_temp_vsprintf(format, v);
//     va_end(v);
//     return result;
// }

// char* util_temp_vsprintf(const char* format, va_list v) {
//     va_list v1;
//     va_copy(v1, v);
//     int n = vsnprintf(NULL, 0, format, v1);
//     va_end(v1);

//     char* result = util_allocate(temp_allocator, (int64_t)n + 1);
//     vsnprintf(result, n + 1, format, v);

//     return result;
// }

// util_arena_block util_arena_block_create(util_arena* arena) {
//     return (util_arena_block){
//         .memory = util_allocate(arena->allocator, arena->block_size),
//         .capacity = arena->block_size,
//     };
// }

// util_arena* util_arena_create(util_allocator allocator, int64_t block_size) {
//     util_arena* arena = util_allocate(allocator, block_size);
//     *arena = (util_arena){
//         .allocator = allocator,
//         .block_size = block_size
//     };

//     util_arena_block first_block = util_arena_block_create(arena);
//     util_da_push(arena->blocks, first_block);

//     return arena;
// }

// void util_arena_destroy(util_arena* arena) {
//     if (arena == NULL) return;

//     util_allocator allocator = arena->allocator;

//     for (int64_t i = 0, count = util_da_count(arena->blocks); i < count; i++) {
//         util_arena_block* block = &arena->blocks[i];
//         util_deallocate(allocator, block->memory);
//         *block = (util_arena_block){0};
//     }

//     util_da_free(arena->blocks);

//     *arena = (util_arena){0};
//     util_deallocate(allocator, arena);
// }

// void* util_arena_push(util_arena* arena, int64_t count) {
//     assert(count <= (int64_t)arena->block_size && "Requested more memory than a temp arena block can hold :(");

//     util_arena_block* block = &arena->blocks[util_da_count(arena->blocks) - 1];
//     if (block->capacity - block->allocated < (int64_t)count) {
//         util_arena_block new_block = util_arena_block_create(arena);
//         util_da_push(arena->blocks, new_block);
//         block = &arena->blocks[util_da_count(arena->blocks) - 1];
//     }

//     void* result = (char*)block->memory + block->allocated;
//     block->allocated += count;
//     memset(result, 0, count);

//     return result;
// }

// void util_arena_clear(util_arena* arena) {
//     for (int64_t i = 0, count = util_da_count(arena->blocks); i < count; i++) {
//         util_arena_block* block = &arena->blocks[i];
//         memset(block->memory, 0, block->capacity);
//         util_deallocate(arena->allocator, block->memory);
//     }

//     util_da_free(arena->blocks);
//     arena->blocks = NULL;

//     util_arena_block first_block = util_arena_block_create(arena);
//     util_da_push(arena->blocks, first_block);
// }

// void util_arena_dump(util_arena* arena) {
//     fprintf(stderr, "<Memory Arena %p>\n", (void*)arena);
//     fprintf(stderr, "  Block Count: %ld\n", util_da_count(arena->blocks));
//     fprintf(stderr, "  Block Size: %ld\n", arena->block_size);
//     fprintf(stderr, "  Block Storage: %p\n", (void*)arena->blocks);
//     fprintf(stderr, "  Allocator:\n");
//     fprintf(stderr, "    User Data: %p\n", (void*)arena->allocator.user_data);
//     // fprintf(stderr, "    Function: %p\n", arena->allocator.allocator_function);
//     fprintf(stderr, "  Blocks:\n");

//     for (int64_t i = 0, count = util_da_count(arena->blocks); i < count; i++) {
//         fprintf(stderr, "    %ld:\n", i);
//         util_arena_block block = arena->blocks[i];

//         fprintf(stderr, "      Memory: %p\n", (void*)block.memory);
//         fprintf(stderr, "      Allocated: %ld\n", block.allocated);
//         fprintf(stderr, "      Capacity: %ld\n", block.capacity);
//     }
// }

// char* util_shift_args(int* argc, char*** argv) {
//     assert(argc != NULL);
//     assert(argv != NULL);
//     assert(*argv != NULL);
//     char* result = **argv;
//     if (result != NULL) {
//         assert(*argc > 0);
//         (*argv) += 1;
//         (*argc) -= 1;
//     }
//     return result;
// }

// // bool util_stdout_isatty(void) {
// //     return isatty(fileno(stdout));
// // }

// // bool util_stderr_isatty(void) {
// //     return isatty(fileno(stderr));
// // }

// bool util_file_exists(const char* file_path) {
// #    if _WIN32
//     // TODO: distinguish between "does not exists" and other errors
//     DWORD dwAttrib = GetFileAttributesA(file_path);
//     return dwAttrib != INVALID_FILE_ATTRIBUTES;
// #    else
//     struct stat statbuf;
//     if (stat(file_path, &statbuf) < 0) {
//         if (errno == ENOENT) return 0;
//         return -1;
//     }
//     return 1;
// #    endif
// }

// util_string util_file_read(util_allocator allocator, const char* file_path) {
//     assert(file_path != NULL);
//     FILE* stream = fopen(file_path, "r");
//     if (stream == NULL) {
//         return util_string_create(allocator);
//     }
//     fseek(stream, 0, SEEK_END);
//     int64_t count = ftell(stream);
//     fseek(stream, 0, SEEK_SET);
//     char* data = util_allocate(allocator, count + 1);
//     size_t readSize = fread(data, (int64_t)count, 1, stream);
//     (void)readSize;
//     data[count] = 0;
//     fclose(stream);
//     return util_string_from_data(allocator, data, count, count + 1);
// }

// // char* util_plat_self_exe(void) {
// // #    if defined(__linux__)
// //     char* buffer = malloc(1024);
// //     memset(buffer, 0, 1024);
// //     uint64_t n = readlink("/proc/self/exe", buffer, 1024);
// //     if (n < 0) {
// //         free(buffer);
// //         return NULL;
// //     }
// //     return buffer;
// // #    elif define(_WIN32)
// //     assert(false && "util_plat_self_exe is not implemented on this platform");
// //     return NULL;
// // #    else
// //     assert(false && "util_plat_self_exe is not implemented on this platform");
// //     return NULL;
// // #    endif
// // }

// void util_assertion_failure_handler(const char* message, const char* expression, int line, const char* file) {
//     fprintf(
//         stderr,
//         "%s:%d: %sAssertion Failed:%s %s\n%s\n",
//         file,
//         line,
//         ANSI_COLOR_RED,
//         ANSI_COLOR_RESET,
//         message,
//         expression
//     );
//     abort();
// }

// // int util_clopt_parse(int argc, char** argv, util_clopt* opts, void* args_data, util_clopt_parser_callback parser_callback) {
// //     UTIL_ASSERT(argc >= 1, "util_clopt_parse can't handle < 1 arguments (needs program name)");

// //     util_string_view program_name = util_string_view_from_cstring(util_shift_args(&argc, &argv));
// //     parser_callback(UTIL_CLOPT_KEY_PROGRAM_NAME, UTIL_SV_EMPTY, program_name, args_data);

// //     if (argc == 0) {
// //         util_clopt_usage(program_name, opts);
// //         exit(0);
// //         return 0;
// //     }

// //     // bool help = false;

// //     bool force_value_args = false;

// //     while (argc > 0) {
// //         util_string_view arg = util_string_view_from_cstring(util_shift_args(&argc, &argv));
// //         if (force_value_args) {
// //             goto arg_is_value;
// //         }

// //         if (util_string_view_equals_cstring(arg, "--")) {
// //             force_value_args = true;
// //         }

// //     arg_is_value:;
// //     }

// //     return 0;
// // }

// // void util_clopt_write_error(util_string_view program_name, const char* format, ...) {
// //     fprintf(stderr, "%.*s: %serror:%s ", UTIL_STR_EXPAND(program_name), ANSI_COLOR_RED, ANSI_COLOR_RESET);
// //     va_list ap;
// //     va_start(ap, format);
// //     vfprintf(stderr, format, ap);
// //     va_end(ap);
// // }

// // void util_clopt_usage(util_string_view program_name, util_clopt* opts) {
// //     fprintf(stderr, "usage: %.*s\n", UTIL_STR_EXPAND(program_name));
// // }