#include <cstddef>
#include <cstdlib>
#include <cstdio>

void* operator new(size_t size){
    void* ptr = malloc(size);
    return ptr;
}
void* operator new[](size_t size){
    void* ptr = malloc(size);
    return ptr;
}
void operator delete[](void* ptr, size_t size){
    (void)size;
    free(ptr);
}
void operator delete[](void* ptr){
    free(ptr);
}
void operator delete(void* ptr, size_t size){
    (void)size;
    free(ptr);
}
void operator delete(void* ptr){
    free(ptr);
}