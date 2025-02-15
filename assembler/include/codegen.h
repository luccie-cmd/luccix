#pragma once
#include "ir.h"

namespace luccix::assembler{
    struct __attribute__((packed)) OrionHeader{
        uint8_t identifier[4];
        uint8_t type;
        uint8_t endianness;
        uint8_t arch;
        uint8_t reserved;
        uint64_t entry;
        uint64_t sectionOffset;
        uint32_t flags;
        uint8_t sectionSize;
        uint16_t sectionNumber;
        uint16_t stringSection;
        uint8_t padding[7];
    };
    struct __attribute__((packed)) OrionSection{
        uint8_t type;
        uint8_t reserved;
        uint16_t alignment;
        uint64_t virtualAddr;
        uint64_t memorySize;
        uint64_t fileSize;
        uint64_t stringIdx;
        uint64_t fileOffset;
        uint8_t flags;
        uint8_t padding[3];
    };
    struct __attribute__((packed)) OrionSymbolEntry{
        uint64_t stringIdx;
        uint64_t size;
        uint16_t info;
        uint8_t value[];
    };
    struct __attribute__((packed)) OrionStringEntry{
        uint64_t stringIdx;
        uint64_t size;
        char value[];
    };
    static_assert(sizeof(OrionHeader) == 40, "Orion header improperly aligned");
    static_assert(sizeof(OrionSection) == 48, "Orion section improperly aligned");
    static_assert(sizeof(OrionSymbolEntry) == 18, "Orion symbol entry improperly aligned");
    static_assert(sizeof(OrionStringEntry) == 16, "Orion string entry improperly aligned");
    class CodeGen{
        private:
            IrTree* inTree;
            std::string outFile;
            OrionHeader *header;
            std::vector<OrionSection*> sections;
            
            void doSymbols();
            void doStrings();
            void doInsts();
            void addHeader();
        public:
            CodeGen(IrTree* tree, std::string outFile);
            ~CodeGen();
            void doCodegen();
    };
};