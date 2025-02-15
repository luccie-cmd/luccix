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
    Location* SyntaxNode::getLoc(){
        return this->loc;
    }
    SyntaxNodeLabelDecl::SyntaxNodeLabelDecl(Token* startKeyword, Token* name) :SyntaxNode(startKeyword->getLoc(), SyntaxNodeType::LabelDecl){
        this->StartKeyword = startKeyword;
        if(startKeyword->getType() == TokenType::KeywordExtern){
            this->labelType = SyntaxNodeLabelDeclType::Extern;
        } else if(startKeyword->getType() == TokenType::KeywordGlobal){
            this->labelType = SyntaxNodeLabelDeclType::Global;
        } else{
            startKeyword->getLoc()->print();
            std::printf("ERROR: Unable to determine label type\n");
            std::exit(1);
        }
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
        this->offset = -1; // let the semagenntic analyser fill this in
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
    std::size_t SyntaxNodeLabel::getOffset(){
        return this->offset;
    }
    SyntaxTree::SyntaxTree(){}
    SyntaxTree::~SyntaxTree(){}
    void SyntaxTree::pushNode(SyntaxNode* node){
        this->nodes.resize(this->nodes.size()+1);
        this->nodes.push_back(node);
    }
    std::vector<SyntaxNode*> SyntaxTree::getNodes(){
        return this->nodes;
    }
    static void printNode(SyntaxNode* node, Diag* diag){
        if(node->getType() == SyntaxNodeType::LabelDecl){
            auto decl = static_cast<SyntaxNodeLabelDecl*>(node);
            diag->printVerbose("Token start = `%s`\nToken name  = `%s`\n", decl->getStartKeyword()->getData().c_str(), decl->getName()->getData().c_str());
        } else if(node->getType() == SyntaxNodeType::Label){
            auto label = static_cast<SyntaxNodeLabel*>(node);
            diag->printVerbose("Token name  = `%s`\n", label->getName()->getData().c_str());
            diag->printVerbose("Label off   =  %ld\n", label->getOffset());
        } else if(node->getType() == SyntaxNodeType::Inst){
            auto inst = static_cast<SyntaxNodeInst*>(node);
            diag->printVerbose("Inst type = %d\n", (int)inst->getInstType());
            diag->printVerbose("Children:\n");
            for(SyntaxNode* childNode : inst->getArguments()){
                printNode(childNode, diag);
            }
            diag->printVerbose("Children end\n");
        } else if(node->getType() == SyntaxNodeType::Nameref){
            auto nameref = static_cast<SyntaxNodeNameref*>(node);
            diag->printVerbose("Nameref data = `%s`\n", nameref->getToken()->getData().c_str());
        } else if(node->getType() == SyntaxNodeType::NumberLiteral){
            auto number = static_cast<SyntaxNodeLiteralNumber*>(node);
            diag->printVerbose("Number data  = `%lx`\n", std::atoll(number->getToken()->getData().c_str()));
        }
    }
    void SyntaxTree::print(Diag* diag){
        for(SyntaxNode* node : this->nodes){
            printNode(node, diag);
        }
    }
    SyntaxSymbol::SyntaxSymbol(std::size_t name, std::size_t value, std::size_t symbolSize, SymbolType symbolType, SymbolBind symbolBind, std::uint8_t unused){
        this->name = name;
        this->value = value;
        this->symbolSize = symbolSize;
        this->symbolType = symbolType;
        this->symbolBind = symbolBind;
        this->unused = unused;
    }
    SyntaxSymbol::~SyntaxSymbol(){}

    std::size_t SyntaxSymbol::getName() const{
        return this->name;
    }
    std::size_t SyntaxSymbol::getValue() const{
        return this->value;
    }
    std::size_t SyntaxSymbol::getSymbolSize() const{
        return this->symbolSize;
    }
    SymbolType SyntaxSymbol::getSymbolType() const{
        return this->symbolType;
    }
    SymbolBind SyntaxSymbol::getSymbolBind() const{
        return this->symbolBind;
    }
    std::uint8_t SyntaxSymbol::getUnused() const{
        return this->unused;
    }
    // bool operator==(const SyntaxSymbol& lhs, const SyntaxSymbol& rhs) {
    //     bool ok = true;
    //     ok = ok && (lhs.getName() == rhs.getName());
    //     ok = ok && (lhs.getValue() == rhs.getValue());
    //     ok = ok && (lhs.getSymbolSize() == rhs.getSymbolSize());
    //     ok = ok && (lhs.getSymbolType() == rhs.getSymbolType());
    //     ok = ok && (lhs.getSymbolBind() == rhs.getSymbolBind());
    //     ok = ok && (lhs.getUnused() == rhs.getUnused());
    //     return ok;
    // }
    // Overload for pointers (ensures at least one parameter is a class)
    bool operator==(const SyntaxSymbol* lhs, const SyntaxSymbol& rhs) {
        if (!lhs) return false;
        return *lhs == rhs;
    }
    bool operator==(const SyntaxSymbol& lhs, const SyntaxSymbol* rhs) {
        if (!rhs) return false;
        return lhs == *rhs;
    }
    bool SyntaxSymbol::operator==(SyntaxSymbol* other){
        (void)other;
        std::printf("Operator ==*\n");
        return true;
    }
    bool SyntaxSymbol::operator==(SyntaxSymbol& other){
        bool ok = true;
        ok = ok && (this->getName() == other.getName());
        ok = ok && (this->getValue() == other.getValue());
        ok = ok && (this->getSymbolSize() == other.getSymbolSize());
        ok = ok && (this->getSymbolType() == other.getSymbolType());
        ok = ok && (this->getUnused() == other.getUnused());
        return ok;
    }
    bool SyntaxSymbol::operator==(SyntaxSymbol*& other){
        (void)other;
        std::printf("Operator ==*&\n");
        return true;
    }
    bool SyntaxSymbol::operator==(SyntaxSymbol&& other){
        (void)other;
        std::printf("Operator ==&&\n");
        return true;
    }
    bool SyntaxSymbol::operator==(const SyntaxSymbol* other) const{
        (void)other;
        std::printf("Operator ==const *\n");
        return true;
    }
    bool SyntaxSymbol::operator==(const SyntaxSymbol& other) const{
        (void)other;
        std::printf("Operator ==const &\n");
        return true;
    }
    bool SyntaxSymbol::operator==(const SyntaxSymbol*& other) const{
        (void)other;
        std::printf("Operator ==const *&\n");
        return true;
    }
    bool SyntaxSymbol::operator==(const SyntaxSymbol&& other) const{
        (void)other;
        std::printf("Operator ==const &&\n");
        return true;
    }
    SyntaxNodeRegister::SyntaxNodeRegister(Token* registerToken, RegisterInfo info) :SyntaxNode(registerToken->getLoc(), SyntaxNodeType::Register){
        this->info = info;
    }
    SyntaxNodeRegister::~SyntaxNodeRegister(){}
};