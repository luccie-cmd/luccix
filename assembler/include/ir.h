#pragma once
#include "ast.h"
#include <variant>
#include <array>

namespace luccix::assembler{
    enum struct IrInstType : int {
        None,
        Mov,
        Syscall,
        Ret
    };
    enum struct IrInstOpType : int {
        None,
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
        M32M32,
        M64M64,
        RRM32,
        RRM64,
        IMPLICIT,
    };
    enum struct IrRegister : int {
        RAX, RBX, RCX, RDX,
        R8, R9, R10, R11,
        R12, R13, R14, R15,
        RSI, RDI, RBP, RSP,
        RIP,
        CS, DS, ES, FS, GS, SS,
        RFLAGS,
        CR0, CR2, CR3, CR4, CR8,
        DR0, DR1, DR2, DR3, DR6, DR7,
        ST0, ST1, ST2, ST3, ST4, ST5, ST6, ST7,
        MM0, MM1, MM2, MM3, MM4, MM5, MM6, MM7,
        XMM0, XMM1, XMM2, XMM3, XMM4, XMM5, XMM6, XMM7,
        YMM0, YMM1, YMM2, YMM3, YMM4, YMM5, YMM6, YMM7,
        ZMM0, ZMM1, ZMM2, ZMM3, ZMM4, ZMM5, ZMM6, ZMM7,
        GDTR, LDTR, IDTR, TR, 
        FSBASE, GSBASE,
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
    struct IrOperandRegister{
        IrRegister registerN;
        RegisterType size;
    };
    struct IrOperandImmediate{
        uint64_t value;
        uint8_t size;
    };
    struct IrOperandMemory{
        bool isBaseRegister;
        union{
            uint64_t base;
            IrRegister baseRegister;
        };
        bool isOffsetRegister;
        union{
            uint64_t offset;
            IrRegister offsetRegister;
        };
    };
    using IrInstOperand = std::variant<IrOperandRegister*, IrOperandImmediate*, IrOperandMemory*>;
    class IrNodeInst{
        private:
            IrInstType instType;
            IrInstOpType operationType;
            std::array<IrInstOperand, 3> operands;
        public:
            IrNodeInst(IrInstType instType, IrInstOpType operationType, std::array<IrInstOperand, 3> operands);
            ~IrNodeInst();
            inline IrInstType getInstType(){ return this->instType; }
            inline IrInstOpType getOperationType(){ return this->operationType; }
            inline std::array<IrInstOperand, 3> getOperands(){ return this->operands; }
    };
    class IrTree{
        private:
            std::vector<IrNodeString*> strings;
            std::vector<IrNodeSymbol*> symbols;
            std::vector<IrNodeInst*> insts;
            void printSymbols(Diag* diag);
            void printStrings(Diag* diag);
            void printInsts(Diag* diag);
        public:
            IrTree();
            ~IrTree();
            void addString(IrNodeString* str);
            void addSymbol(IrNodeSymbol* sym);
            void addInst(IrNodeInst* inst);
            bool nameInStr(std::string name);
            std::size_t findStrIdx(std::string name);
            void print(Diag* diag);
            inline std::vector<IrNodeString*> getStrings() { return this->strings; }
            inline std::vector<IrNodeSymbol*> getSymbols() { return this->symbols; }
            inline std::vector<IrNodeInst*> getInsts() { return this->insts; }
    };
}