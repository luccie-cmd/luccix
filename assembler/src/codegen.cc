#include <codegen.h>
#include <cstring>

#include <iostream>

namespace luccix::assembler{
    template<typename T>
    static std::vector<uint8_t> getUint8List(T n){
        std::vector<uint8_t> result(sizeof(n));
        std::memcpy(result.data(), &n, sizeof(n));
        return result;
    }
    CodeGen::CodeGen(IrTree* tree, std::string outFile){
        this->inTree = tree;
        this->outFile = outFile;
    }
    void CodeGen::doCodegen(){
        this->addHeader();
        this->doSymbols();
        this->doStrings();
        this->doInsts();
        // for(uint64_t i = 0; i < this->outBytes.size(); ++i){
        //     std::printf("0x%02x ", this->outBytes.at(i));
        //     if((i+1)%16==0){
        //         std::printf("\n");
        //     }
        // }
        std::printf("\n");
    }
    void CodeGen::addHeader(){
//         // Identifier bytes
//         this->outBytes.push_back(0x04);
//         this->outBytes.push_back('O');
//         this->outBytes.push_back('R');
//         this->outBytes.push_back('I');
//         // Type byte
//         this->outBytes.push_back(0x01);
//         // Endianness
//         this->outBytes.push_back((uint8_t)(std::endian::native == std::endian::big));
//         // Arch
// #if defined(__x86_64__) || defined(_M_X64)
//         this->outBytes.push_back(0x02);
// #elif defined(__i386__) || defined(_M_IX86)
//         #error "TODO: X86 codegen"
// #elif defined(__aarch64__)
//         #error "TODO: ARM64 codegen"
// #elif defined(__arm__) || defined(_M_ARM)
//         #error "TODO: ARM codegen"
// #elif defined(__riscv)
//         #error "TODO: RISC-V codegen"
// #else
//         #error "Unknown architecture"
// #endif
//         // Reserved
//         this->outBytes.push_back(0x00);
//         // Entry
//         for(uint8_t sym : getUint8List((size_t)0)){
//             this->outBytes.push_back(sym);
//         }
//         // Section offset
//         for(uint8_t sym : getUint8List((size_t)40)){
//             this->outBytes.push_back(sym);
//         }
//         // Flags
//         for(uint8_t sym : getUint8List((size_t)0)){
//             this->outBytes.push_back(sym);
//         }
//         // Section size
//         this->outBytes.push_back(48);
//         // Section numbers
//         // String section
//         // Padding
    }
    void CodeGen::doSymbols(){
        // for(IrNodeSymbol* symbol : this->inTree->getSymbols()){
        //     for(uint8_t sym : getUint8List(symbol->getSynSym()->getName())){
        //         this->tmpSections.push_back(sym);
        //     }
        // }
    }
    void CodeGen::doStrings(){
        // for(IrNodeString* symbol : this->inTree->getStrings()){
        //     for(char c : symbol->getStr()){
        //         this->tmpSections.push_back((uint8_t)(c));
        //     }
        //     this->tmpSections.push_back((uint8_t)('\0'));
        // }
    }
    void CodeGen::doInsts(){

    }
    CodeGen::~CodeGen(){}
};