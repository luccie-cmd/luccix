#include <semagen.h>

namespace luccix::assembler{
    SemaGen::SemaGen(SyntaxTree* tree, Diag* diag){
        diag->print(DiagLevel::Ice, "TODO: Sema Irgen\n");
        this->inTree = tree;
        this->diag = diag;
        this->outTree = nullptr;
        this->status = SemaGenStatus::Progress;
        this->instLenInBytes = 0;
    }
    SemaGen::~SemaGen(){
        this->status = SemaGenStatus::Invalid;
        delete this->inTree;
    }
    void SemaGen::translateLabelDecl(SyntaxNodeLabelDecl* decl){
        this->diag->addTrace(__PRETTY_FUNCTION__);
        this->globalExternNodesToHandle.push_back(decl);
        this->diag->popTrace();
    }
    void SemaGen::translateLabelToSymbol(SyntaxNodeLabel* label){
        this->diag->addTrace(__PRETTY_FUNCTION__);
        if(this->outTree->nameInStr(label->getName()->getData())){
            this->diag->print(label->getLoc(), DiagLevel::Error, "Redefinition of label `%s`\n", label->getName()->getData().c_str());
            this->status = SemaGenStatus::Error;
        }
        this->outTree->addString(new IrNodeString(label->getName()->getData()));
        SyntaxSymbol* symbol = new SyntaxSymbol(this->outTree->findStrIdx(label->getName()->getData()), this->instLenInBytes, 0, SymbolType::NoType, SymbolBind::Unknown, 0);
        this->outTree->addSymbol(new IrNodeSymbol(symbol));
        this->diag->popTrace();
    }
    void SemaGen::translateNode(SyntaxNode* node){
        this->diag->addTrace(__PRETTY_FUNCTION__);
        switch(node->getType()){
            case SyntaxNodeType::LabelDecl: {
                this->translateLabelDecl(static_cast<SyntaxNodeLabelDecl*>(node));  
            } break;
            case SyntaxNodeType::Label: {
                this->translateLabelToSymbol(static_cast<SyntaxNodeLabel*>(node));
            } break;
            default: {
                this->diag->printTrace();
                this->diag->print(node->getLoc(), DiagLevel::Ice, "Unhandled syntaxnode type %d\n", (int)node->getType());
                this->status = SemaGenStatus::Error;
                // std::exit(1);
            } break;
        }
        this->diag->popTrace();
    }
    void SemaGen::parseTree(){
        this->diag->addTrace(__PRETTY_FUNCTION__);
        this->outTree = new IrTree;
        for(SyntaxNode* node : this->inTree->getNodes()){
            if(this->status == SemaGenStatus::Error){
                return;
            }
            this->translateNode(node);
        }
        this->diag->popTrace();
    }
    IrTree* SemaGen::getTree(){
        this->diag->addTrace(__PRETTY_FUNCTION__);
        if(this->outTree == nullptr){
            this->parseTree();
        }
        if(this->status == SemaGenStatus::Error){
            this->diag->print(DiagLevel::Error, "Semanatic Generator error\n");
        }
        this->diag->popTrace();
        return this->outTree;
    }
}