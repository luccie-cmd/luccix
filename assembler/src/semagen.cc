#include <semagen.h>
#include <cstring>

namespace luccix::assembler{
    SemaGen::SemaGen(SyntaxTree* tree, Diag* diag){
        this->inTree = tree;
        this->diag = diag;
        this->outTree = nullptr;
        this->status = SemaGenStatus::Progress;
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
        SyntaxSymbol* symbol = new SyntaxSymbol(this->outTree->findStrIdx(label->getName()->getData()), 0, 0, SymbolType::NoType, SymbolBind::Unknown, 0);
        this->outTree->addSymbol(new IrNodeSymbol(symbol));
        this->diag->popTrace();
    }
    static IrRegister getRegisterFromName(Diag* diag, const char* name){
        IrRegister reg = IrRegister::RAX;
        if(std::strcmp(name, "rax") == 0){
            reg = IrRegister::RAX;
        } else if(std::strcmp(name, "rdi") == 0){
            reg = IrRegister::RDI;
        } else{
            diag->print(DiagLevel::Ice, "Unhandled register `%s`\n", name);
            std::exit(1);
        }
        return reg;
    }
    void SemaGen::translateInst(SyntaxNodeInst* inst){
        this->diag->addTrace(__PRETTY_FUNCTION__);
        std::vector<SyntaxNode*> args = inst->getArguments();
        std::array<SyntaxNodeType, 3> types = {};
        IrInstOpType opType = IrInstOpType::None;
        std::array<IrInstOperand, 3> irArgs = {};
        for(size_t i = 0; i < args.size(); ++i){
            types.at(i) = args.at(i)->getType();
        }
        if(types.at(0) == SyntaxNodeType::Register && types.at(1) == SyntaxNodeType::NumberLiteral && types.at(2) == SyntaxNodeType::Invalid){
            auto nodeReg = reinterpret_cast<SyntaxNodeRegister*>(args.at(0));
            auto immReg = reinterpret_cast<SyntaxNodeLiteralNumber*>(args.at(1));
            RegisterType regType = nodeReg->getInfo().type;
            std::uint64_t immValue = std::stoull(immReg->getToken()->getData());
            if(regType < RegisterType::Qword && immValue > UINT32_MAX){
                this->diag->print(inst->getLoc(), DiagLevel::Warning, "Overflow of mov instruction (register %s not big enough to hold value 0x%lx)\n", nodeReg->getInfo().name, immValue);
            }
            if(regType == RegisterType::Qword){
                opType = IrInstOpType::R64I64;
            } else if(regType <= RegisterType::Dword){
                opType = IrInstOpType::R32I32;
            } else{
                this->diag->print(inst->getLoc(), DiagLevel::Ice, "Unhandled register type %u (%s)\n", (unsigned int)regType, nodeReg->getInfo().name);
                this->status = SemaGenStatus::Error;
            }
            IrOperandRegister* irReg = new IrOperandRegister {
                .registerN = getRegisterFromName(this->diag, nodeReg->getInfo().name),
                .size = regType
            };
            IrOperandImmediate* irImm = new IrOperandImmediate {
                .value = immValue,
                .size = static_cast<uint8_t>(immValue < UINT8_MAX ? 8 : (immValue < UINT16_MAX ? 16 : (immValue < UINT32_MAX ? 32 : (immValue < UINT64_MAX ? 64 : 0))))
            };
            irArgs[0] = IrInstOperand(irReg);
            irArgs[1] = IrInstOperand(irImm);
        } else if(types.at(0) == types.at(1) && types.at(1) == types.at(2) && types.at(2) == SyntaxNodeType::Invalid){
            opType = IrInstOpType::None;
        } else{
            this->diag->print(inst->getLoc(), DiagLevel::Ice, "Unhandled syntax types %u %u %u\n", (unsigned int)types.at(0), (unsigned int)types.at(1), (unsigned int)types.at(2));
            this->status = SemaGenStatus::Error;
        };
        IrNodeInst* irInst = nullptr;
        switch(inst->getInstType()){
            case SyntaxNodeInstType::Mov: {
                irInst = new IrNodeInst(IrInstType::Mov, opType, irArgs);
            } break;
            case SyntaxNodeInstType::Syscall: {
                irInst = new IrNodeInst(IrInstType::Syscall, opType, irArgs);
            } break;
            case SyntaxNodeInstType::Ret: {
                irInst = new IrNodeInst(IrInstType::Ret, opType, irArgs);
            } break;
            default: {
                this->diag->print(inst->getLoc(), DiagLevel::Ice, "Unhandled instruction type %u\n", (unsigned int)inst->getInstType());
                this->status = SemaGenStatus::Error;
            } break;
        }
        this->outTree->addInst(irInst);
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
            case SyntaxNodeType::Inst: {
                this->translateInst(static_cast<SyntaxNodeInst*>(node));
            } break;
            default: {
                this->diag->printTrace();
                this->diag->print(node->getLoc(), DiagLevel::Ice, "Unhandled syntaxnode type %d\n", (int)node->getType());
                this->status = SemaGenStatus::Error;
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