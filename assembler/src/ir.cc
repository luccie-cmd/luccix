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
        (void)diag;
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