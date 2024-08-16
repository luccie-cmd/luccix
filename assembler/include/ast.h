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
        Nameref,
        NumberLiteral,
    };
    enum struct SyntaxNodeLabelDeclType : int {
        Invalid,
        Global,
    };
    enum struct SyntaxNodeInstType : int {
        Invalid,
        Mov,
        Syscall,
        Ret,
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
    class SyntaxNodeLabelDecl : public SyntaxNode {
        private:
            SyntaxNodeLabelDeclType labelType;
            Token* StartKeyword;
            Token* name;
        public:
            SyntaxNodeLabelDecl(Token* startKeyword, Token* name);
            ~SyntaxNodeLabelDecl() override;
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
            std::size_t getName();
            std::size_t getValue();
            std::size_t getSymbolSize();
            SymbolType getSymbolType();
            SymbolBind getSymbolBind();
            std::uint8_t getUnused();
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