#pragma once
#include "ast.h"

namespace luccix::assembler{
    enum struct IrInstType : int {
        // we are only supporting 64 bit systems, but 64 bit nasm outputs, when a value is less than UINT32_T_MAX, a 32 bit mov inst (0x8B)
        Mov32,
        // These instructions will always output the same instructions no mather where they are or in which architecture
        Syscall,
        Ret, // Near far will be diffirent
    };
    enum struct IrInstOpType : int {
        R32R32,
        R64R64,
        R32I32,
        R64I64,
        R32M32,
        R64M64,
        M32I32,
        M64I64,
        M32R32,
        M64R64,
    };
    enum struct IrRegister : int {
        RAX,
        RBX,
        RCX,
        RDX,
        RSI,
        RDI,
        RBP,
        RSP,
    };
    class IrNodeString{
        private:
            std::string str;
        public:
            IrNodeString(std::string str);
            ~IrNodeString();
            std::string getStr();

    };
    class IrNodeSymbol{
        private:
            SyntaxSymbol* synSym;
        public:
            IrNodeSymbol(SyntaxSymbol* sym);
            ~IrNodeSymbol();
            SyntaxSymbol* getSynSym();

    };
    class IrTree{
        private:
            std::vector<IrNodeString*> strings;
            std::vector<IrNodeSymbol*> symbols;
        public:
            IrTree();
            ~IrTree();
            void addString(IrNodeString* str);
            void addSymbol(IrNodeSymbol* sym);
            bool nameInStr(std::string name);
            std::size_t findStrIdx(std::string name);
    };
}