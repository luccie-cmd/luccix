#pragma once
#include "ast.h"
#include "diag.h"
#include "parser.h"
#include "ir.h"
#include <utility>

namespace luccix::assembler{
    enum struct SemaGenStatus : int {
        Invalid,
        Progress,
        Error,
        Done,
    };
    class SemaGen{
        private:
            std::vector<SyntaxNodeLabelDecl*> globalExternNodesToHandle;
            std::vector<std::pair<bool, SyntaxSymbol*>> tempSymbols;
            SyntaxTree* inTree;
            IrTree* outTree;
            Diag* diag;
            SemaGenStatus status;
            void parseTree();
            void translateNode(SyntaxNode* node);
            void translateLabelDecl(SyntaxNodeLabelDecl* decl);
            void translateLabelToSymbol(SyntaxNodeLabel* label);
            void translateInst(SyntaxNodeInst* inst);
        public:
            SemaGen(SyntaxTree* tree, Diag* diag);
            ~SemaGen();
            IrTree* getTree();
    };
};