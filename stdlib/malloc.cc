#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <sys/mman.h>
#include <unistd.h>
#include <cstring>
#include <mutex>
#include <vector>
#include <utility>

#define MEGABYTE 1024*1024
#define GIGABYTE 1024*1024*1024
#define PAGE_SIZE 4096
#define GROW_RATE PAGE_SIZE
#define ALIGNMENT 16
static bool __initialized = false;
static uint64_t __pmmSize = 0, __vmmMax = 0, __allocMemory = 0;
struct alignas(16) Node{
    size_t size;
    size_t allocSize;
    Node* prev;
    Node* next;
    bool free;
};
static_assert(alignof(Node) >= ALIGNMENT, "Node does not meet the required alignment!");
static Node* __head = nullptr;
static std::mutex mtx;
template<typename T>
T align(T size, uint64_t alignment){
    return (size + alignment - 1) & ~(alignment - 1);
}

static void __newHead(){
    if(__head){
        int64_t remaining = align(__pmmSize-__head->size, PAGE_SIZE);
        if(remaining < 0){
            write(STDERR_FILENO, "remaining was negative\n", 23);
            std::abort();
        }
        Node* newHead = (Node*)mmap(nullptr, remaining, PROT_READ | PROT_WRITE, 0x22, -1, 0);
        newHead->allocSize = remaining;
        newHead->size = remaining;
        newHead->free = true;
        newHead->prev = nullptr;
        newHead->next = __head;
        __head = newHead;
    } else{
        __pmmSize = align(__pmmSize, PAGE_SIZE);
        __head = (Node*)mmap(nullptr, __pmmSize, PROT_READ | PROT_WRITE, 0x22, -1, 0);
        __head->size = __pmmSize;
        __head->allocSize = __pmmSize;
        __head->free = true;
        __head->prev = nullptr;
        __head->next = nullptr;
    }
}
static void __CoalesceBlocks(){
    Node* current = __head;
    while (current && current->next){
        if (current->free && current->next->free){
            current->size += sizeof(Node) + current->next->size;
            current->next = current->next->next;
            if (current->next){
                current->next->prev = current;
            }
        }
        else{
            current = current->next;
        }
    }
}
static void __initializeMallocFree(){
    __pmmSize = __allocMemory = GROW_RATE;
    __vmmMax = MEGABYTE;
    __newHead();
    __initialized = true;
}

void* malloc(size_t size){
    mtx.lock();
    if(!__initialized){
        __initializeMallocFree();
    }
    size_t alignedLength = align(size, ALIGNMENT);
    __allocMemory += alignedLength;
    bool pmmSizeChanged = false;
    while(__allocMemory >= __pmmSize){
        __pmmSize += GROW_RATE;
        pmmSizeChanged = true;
    }
    if(pmmSizeChanged){
        __newHead();
    }
    Node* current = __head;
    while(current){
        if(current->free && current->size >= alignedLength){
            if(current->size > alignedLength + sizeof(Node)){
                Node* newNode = reinterpret_cast<Node*>(reinterpret_cast<uint8_t*>(current)+sizeof(Node)+alignedLength);
                newNode->size = current->size - alignedLength - sizeof(Node);
                newNode->free = true;
                newNode->next = current->next;
                newNode->prev = current;
                newNode->allocSize = current->size - alignedLength - sizeof(Node);
                if (current->next){
                    current->next->prev = newNode;
                }
                current->next = newNode;
                current->size = alignedLength;
            }
            current->allocSize = alignedLength;
            current->free = false;
            mtx.unlock();
            return reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(current)+sizeof(Node));
        }
        current = current->next;
    }
    write(STDERR_FILENO, "Exhausted memory\n", 17);
    std::abort();
}

void free(void* ptr){
    mtx.lock();
    bool found = false;
    Node* current = __head;
    while(current){
        if(current == (Node*)((uint64_t)ptr-sizeof(Node))){
            found = true;
            break;
        }
        current = current->next;
    }
    if(!found){
        write(STDERR_FILENO, "Node not found\n", 15);
        std::abort();
    }
    Node* freeNode = reinterpret_cast<Node*>((uintptr_t)ptr-sizeof(Node));
    if(freeNode->free){
        write(STDERR_FILENO, "Double free\n", 12);
        std::abort();
    }
    freeNode->free = true;
    __allocMemory -= freeNode->size;
    __CoalesceBlocks();
    mtx.unlock();
}