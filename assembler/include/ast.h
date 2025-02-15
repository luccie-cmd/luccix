#pragma once
#include "diag.h"
#include "token.h"
#include <cstdint>

namespace luccix::assembler{
    enum struct SyntaxNodeType : int {
        Invalid,
        LabelDecl, // extern, global
        Label,
        Inst,
        Register,
        Nameref,
        NumberLiteral,
    };
    enum struct SyntaxNodeLabelDeclType : int {
        Invalid,
        Global,
        Extern,
    };
    enum struct SyntaxNodeInstType : int {
        Invalid,
        Mov,
        Syscall,
        Ret,
    };
    enum struct RegisterType {
        Byte,    // 8-bit registers
        Word,    // 16-bit registers
        Dword,   // 32-bit registers
        Qword,   // 64-bit registers
        XMM,     // 128-bit SSE registers
        YMM,     // 256-bit AVX registers
        ZMM      // 512-bit AVX-512 registers
    };
    struct RegisterInfo {
        const char* name;
        RegisterType type;
    };
    class SyntaxNode {
        private:
            Location* loc;
            SyntaxNodeType type;
        public:
            SyntaxNode(Location* loc, SyntaxNodeType type);
            virtual ~SyntaxNode();
            SyntaxNodeType getType();
            Location* getLoc();
    };
    class SyntaxNodeRegister : public SyntaxNode{
        private:
            RegisterInfo info;
        public:
            SyntaxNodeRegister(Token* registerToken, RegisterInfo info);
            SyntaxNodeRegister(Token* registerToken, const char* name, RegisterType type);
            inline RegisterInfo getInfo(){ return this->info; }
            ~SyntaxNodeRegister();
    };
    class SyntaxNodeLabelDecl : public SyntaxNode {
        private:
            SyntaxNodeLabelDeclType labelType;
            Token* StartKeyword;
            Token* name;
        public:
            SyntaxNodeLabelDecl(Token* startKeyword, Token* name);
            ~SyntaxNodeLabelDecl() override;
            inline SyntaxNodeLabelDeclType getLabelType() { return this->labelType; }
            Token* getStartKeyword();
            Token* getName();
    };
    class SyntaxNodeLabel : public SyntaxNode {
        private:
            Token* nameToken;
            // Relative to begining of the file
            std::size_t offset;
        public:
            SyntaxNodeLabel(Token* name);
            ~SyntaxNodeLabel() override;
            Token* getName();
            std::size_t getOffset();
    };
    class SyntaxNodeNameref : public SyntaxNode {
        private:
            Token *nameref;
        public:
            SyntaxNodeNameref(Token* namerefToken);
            ~SyntaxNodeNameref() override;
            Token* getToken();
    };
    class SyntaxNodeLiteralNumber : public SyntaxNode {
        private:
            Token *number;
        public:
            SyntaxNodeLiteralNumber(Token* numberToken);
            ~SyntaxNodeLiteralNumber() override;
            Token* getToken();
    };
    class SyntaxNodeInst : public SyntaxNode {
        private:
            Token* instToken;
            SyntaxNodeInstType instType; // Should be in 1 big std::vector<std::pair<TokenType, SyntaxNodeInstType>>
            std::vector<SyntaxNode*> arguments;
        public:
            SyntaxNodeInst(Token* instToken, std::vector<SyntaxNode*> args);
            ~SyntaxNodeInst() override;
            SyntaxNodeInstType getInstType();
            std::vector<SyntaxNode*> getArguments();
    };
    enum struct SymbolType : int {
        NoType,
    };
    enum struct SymbolBind : int {
        Unknown,
        Global,
        Local,
    };
    class SyntaxSymbol {
        private:
            std::size_t name;
            std::size_t value;
            std::size_t symbolSize;
            SymbolType symbolType : 4;
            SymbolBind symbolBind : 4;
            std::uint8_t unused;
        public:
            SyntaxSymbol(std::size_t name, std::size_t value, std::size_t symbolSize, SymbolType symbolType, SymbolBind symbolBind, std::uint8_t unused);
            ~SyntaxSymbol();
            std::size_t getName() const;
            std::size_t getValue() const;
            std::size_t getSymbolSize() const;
            SymbolType getSymbolType() const;
            SymbolBind getSymbolBind() const;
            std::uint8_t getUnused() const;
            bool operator==(SyntaxSymbol* other);
            bool operator==(SyntaxSymbol& other);
            bool operator==(SyntaxSymbol*& other);
            bool operator==(SyntaxSymbol&& other);
            bool operator==(const SyntaxSymbol* other) const;
            bool operator==(const SyntaxSymbol& other) const;
            bool operator==(const SyntaxSymbol*& other) const;
            bool operator==(const SyntaxSymbol&& other) const;
    };
    class SyntaxTree{
        private:
            std::vector<SyntaxNode*> nodes;
        public:
            SyntaxTree();
            ~SyntaxTree();
            void print(Diag* diag);
            std::size_t findStrIdx(std::string str);
            bool nameInStrtab(std::string name);
            void addString(std::string str);
            SyntaxSymbol* getSymbolByName(std::string str);
            void addSymbol(SyntaxSymbol* symbol);
            void pushNode(SyntaxNode* node);
            std::vector<SyntaxNode*> getNodes();
    };
}