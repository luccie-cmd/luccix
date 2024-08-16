#pragma once
#include "ast.h"
#include "diag.h"
#include "parser.h"
#include "ir.h"

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
            SyntaxTree* inTree;
            IrTree* outTree;
            Diag* diag;
            SemaGenStatus status;
            std::size_t instLenInBytes;
            void parseTree();
            void translateNode(SyntaxNode* node);
            void translateLabelDecl(SyntaxNodeLabelDecl* decl);
            void translateLabelToSymbol(SyntaxNodeLabel* label);
        public:
            SemaGen(SyntaxTree* tree, Diag* diag);
            ~SemaGen();
            IrTree* getTree();
    };
};