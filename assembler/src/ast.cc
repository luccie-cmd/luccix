#include "ast.h"

namespace luccix::assembler{
    SyntaxNode::SyntaxNode(Location* loc, SyntaxNodeType type){
        this->loc = loc;
        this->type = type;
    }
    SyntaxNode::~SyntaxNode(){
        delete this->loc;
    }
    SyntaxNodeType SyntaxNode::getType(){
        return this->type;
    }
    SyntaxNodeLabelDecl::SyntaxNodeLabelDecl(Token* startKeyword, Token* name) :SyntaxNode(startKeyword->getLoc(), SyntaxNodeType::LabelDecl){
        this->StartKeyword = startKeyword;
        this->name = name;
    }
    SyntaxNodeLabelDecl::~SyntaxNodeLabelDecl(){
        delete this->name;
        delete this->StartKeyword;
    }
    SyntaxNodeNameref::SyntaxNodeNameref(Token* namerefToken) :SyntaxNode(namerefToken->getLoc(), SyntaxNodeType::Nameref){
        this->nameref = namerefToken;
    }
    SyntaxNodeNameref::~SyntaxNodeNameref(){
        delete this->nameref;
    }
    Token* SyntaxNodeNameref::getToken(){
        return this->nameref;
    }
    SyntaxNodeLiteralNumber::SyntaxNodeLiteralNumber(Token* numberToken) :SyntaxNode(numberToken->getLoc(), SyntaxNodeType::NumberLiteral){
        this->number = numberToken;
    }
    SyntaxNodeLiteralNumber::~SyntaxNodeLiteralNumber(){
        delete this->number;
    }
    Token* SyntaxNodeLiteralNumber::getToken(){
        return this->number;
    }
    SyntaxNodeLabel::SyntaxNodeLabel(Token* name) :SyntaxNode(name->getLoc(), SyntaxNodeType::Label){
        this->nameToken = name;
        this->offset = -1; // let the semantic analyser fill this in
    }
    SyntaxNodeLabel::~SyntaxNodeLabel(){
        delete this->nameToken;
    }
    static std::vector<std::pair<TokenType, SyntaxNodeInstType>> instTypes = {
        {TokenType::InstMov, SyntaxNodeInstType::Mov},
        {TokenType::InstRet, SyntaxNodeInstType::Ret},
        {TokenType::InstSyscall, SyntaxNodeInstType::Syscall}
    };
    static SyntaxNodeInstType instTypeFromToken(Token* token){
        for(auto pair : instTypes){
            if(pair.first == token->getType()){
                return pair.second;
            }
        }
        return SyntaxNodeInstType::Invalid;
    }
    SyntaxNodeInst::SyntaxNodeInst(Token* instToken, std::vector<SyntaxNode*> args) :SyntaxNode(instToken->getLoc(), SyntaxNodeType::Inst){
        this->instToken = instToken;
        this->arguments = args;
        this->instType = instTypeFromToken(instToken);
    }
    SyntaxNodeInstType SyntaxNodeInst::getInstType(){
        return this->instType;
    }
    SyntaxNodeInst::~SyntaxNodeInst(){
        delete this->instToken;
        for(SyntaxNode* node : this->arguments){
            delete node;
        }
    }
    std::vector<SyntaxNode*> SyntaxNodeInst::getArguments(){
        return this->arguments;
    }
    Token* SyntaxNodeLabelDecl::getStartKeyword(){
        return this->StartKeyword;
    }
    Token* SyntaxNodeLabelDecl::getName(){
        return this->name;
    }
    Token* SyntaxNodeLabel::getName(){
        return this->nameToken;
    }
    SyntaxTree::SyntaxTree(){}
    SyntaxTree::~SyntaxTree(){}
    void SyntaxTree::pushNode(SyntaxNode* node){
        this->nodes.push_back(node);
    }
    std::vector<SyntaxNode*> SyntaxTree::getNodes(){
        return this->nodes;
    }
    static void printNode(SyntaxNode* node, Diag* diag){
        if(node->getType() == SyntaxNodeType::LabelDecl){
            auto decl = static_cast<SyntaxNodeLabelDecl*>(node);
            diag->print(DiagLevel::Note, "Token start = `%s`\nNote: Token name  = `%s`\n", decl->getStartKeyword()->getData().c_str(), decl->getName()->getData().c_str());
        } else if(node->getType() == SyntaxNodeType::Label){
            auto label = static_cast<SyntaxNodeLabel*>(node);
            diag->print(DiagLevel::Note, "Token name  = `%s`\n", label->getName()->getData().c_str());
        } else if(node->getType() == SyntaxNodeType::Inst){
            auto inst = static_cast<SyntaxNodeInst*>(node);
            diag->print(DiagLevel::Note, "Inst type = %d\n", (int)inst->getInstType());
            diag->print(DiagLevel::Note, "Children:\n");
            for(SyntaxNode* childNode : inst->getArguments()){
                printNode(childNode, diag);
            }
            diag->print(DiagLevel::Note, "Children end\n");
        } else if(node->getType() == SyntaxNodeType::Nameref){
            auto nameref = static_cast<SyntaxNodeNameref*>(node);
            diag->print(DiagLevel::Note, "Nameref data = `%s`\n", nameref->getToken()->getData().c_str());
        } else if(node->getType() == SyntaxNodeType::NumberLiteral){
            auto number = static_cast<SyntaxNodeLiteralNumber*>(node);
            diag->print(DiagLevel::Note, "Number data = `%.8lx`\n", std::atoll(number->getToken()->getData().c_str()));
        }
    }
    void SyntaxTree::print(Diag* diag){
        for(SyntaxNode* node : this->getNodes()){
            printNode(node, diag);
        }
    }
};