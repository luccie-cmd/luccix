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
};