#include <ir.h>

namespace luccix::assembler{
    IrTree::IrTree(){}
    IrTree::~IrTree(){}
    bool IrTree::nameInStr(std::string name){
        for(IrNodeString* str : this->strings){
            if(str->getStr() == name){
                return true;
            }
        }
        return false;
    }
    void IrTree::addString(IrNodeString* str){
        this->strings.push_back(str);
    }
    std::size_t IrTree::findStrIdx(std::string name){
        for(std::size_t i = 0; i < this->strings.size(); ++i){
            if(this->strings.at(i)->getStr() == name){
                return i;
            }
        }
        return -1;
    }
    void IrTree::addSymbol(IrNodeSymbol* sym){
        this->symbols.push_back(sym);
    }
    void IrTree::addInst(IrNodeInst* inst){
        this->insts.push_back(inst);
    }
    IrNodeSymbol::IrNodeSymbol(SyntaxSymbol* sym){
        this->synSym = sym;
    }
    SyntaxSymbol* IrNodeSymbol::getSynSym(){
        return this->synSym;
    }
    IrNodeSymbol::~IrNodeSymbol(){
        delete this->synSym;
    }
    IrNodeString::IrNodeString(std::string str){
        this->str = str;
    }
    std::string IrNodeString::getStr(){
        return this->str;
    }
    IrNodeInst::IrNodeInst(IrInstType instType, IrInstOpType operationType, std::array<IrInstOperand, 3> operands){
        this->instType = instType;
        this->operationType = operationType;
        this->operands = operands;
    }
    IrNodeInst::~IrNodeInst(){}
    void IrTree::printSymbols(Diag* diag){
        for(IrNodeSymbol *symbol : this->symbols){
            diag->printVerbose("Symbol %lu\n", symbol->getSynSym()->getName());
        }
    }
    void IrTree::printStrings(Diag* diag){
        for(size_t i = 0; i < this->strings.size(); ++i){
            IrNodeString* string = this->strings.at(i);
            diag->printVerbose("String %lu: %s\n", i, string->getStr().c_str());
        }
    }
    void IrTree::printInsts(Diag* diag){
        for(IrNodeInst* inst : this->insts){
            diag->printVerbose("Inst Type: %u, operand type: %u\n", (unsigned int)inst->getInstType(), (unsigned int)inst->getOperationType());
            switch(inst->getOperationType()){
                case IrInstOpType::R64I64: {
                    auto reg = std::get<IrOperandRegister*>(inst->getOperands().at(0));
                    auto imm = std::get<IrOperandImmediate*>(inst->getOperands().at(1));
                    diag->printVerbose("- Operand 0 register:\n");
                    diag->printVerbose("\t- Size: %u register: %u\n", reg->size, (unsigned int)reg->registerN);
                    diag->printVerbose("- Operand 1 immediate:\n");
                    diag->printVerbose("\t- Size: %u value: %lu\n", imm->size, imm->value);
                } break;
                case IrInstOpType::IMPLICIT:
                case IrInstOpType::None: {} break;
                default: {
                    diag->print(DiagLevel::Ice, "TODO: Operation type %u\n", (unsigned int)inst->getOperationType());
                    std::exit(1);
                } break;
            }
        }
    }
    void IrTree::print(Diag* diag){
        diag->printVerbose("SYMBOLS:\n");
        this->printSymbols(diag);
        diag->printVerbose("STRINGS:\n");
        this->printStrings(diag);
        diag->printVerbose("INSTS:\n");
        this->printInsts(diag);
    }
};