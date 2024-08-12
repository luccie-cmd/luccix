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
    };
    enum struct SyntaxNodeLabelDeclType : int {
        Invalid,
        Global,
    };
    class SyntaxNode {
        private:
            Location* loc;
            SyntaxNodeType type;
        public:
            SyntaxNode(Location* loc, SyntaxNodeType type);
            virtual ~SyntaxNode() = 0;
            SyntaxNodeType getType();
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
            std::size_t offset;
        public:
            SyntaxNodeLabel(Token* name);
            ~SyntaxNodeLabel() override;
            Token* getName();
    };
    class SyntaxSymbol {
        private:
            std::size_t name;
            std::size_t value;
            std::size_t symbolSize;
            std::uint8_t symbolType : 4;
            std::uint8_t symbolBind : 4;
            std::uint8_t unused;
        public:
            SyntaxSymbol(std::size_t name, std::size_t value, std::size_t symbolSize, std::uint8_t symbolType, std::uint8_t symbolBind, std::uint8_t unused);
            ~SyntaxSymbol();
    };
    class SyntaxTree{
        private:
            std::vector<SyntaxNode*> nodes;
            std::vector<SyntaxSymbol*> symbols;
            std::vector<std::string> strings;
        public:
    };
}