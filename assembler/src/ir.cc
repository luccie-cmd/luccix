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
    IrNodeSymbol::IrNodeSymbol(SyntaxSymbol* sym){
        this->synSym = sym;
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
};