#include <codegen.h>
#include <cstring>

namespace luccix::assembler{
    static std::vector<uint8_t> getUint8List(size_t n){
        std::vector<uint8_t> result(sizeof(n));
        std::memcpy(result.data(), &n, sizeof(n));
        return result;
    }
    CodeGen::CodeGen(IrTree* tree, std::string outFile){
        this->outSymbols.clear();
        this->outStrings.clear();
        this->outInsts.clear();
        this->inTree = tree;
        this->outFile = outFile;
    }
    void CodeGen::doCodegen(){
        this->doSymbols();
        this->doStrings();
        this->doInsts();
        for(uint64_t i = 0; i < this->outSymbols.size(); ++i){
            std::printf("0x%02x ", this->outSymbols.at(i));
            if((i+1)%16==0){
                std::printf("\n");
            }
        }
        std::printf("\n");
    }
    void CodeGen::doSymbols(){
        for(IrNodeSymbol* symbol : this->inTree->getSymbols()){
            for(uint8_t sym : getUint8List(symbol->getSynSym()->getName())){
                this->outSymbols.push_back(sym);
            }
        }
    }
    void CodeGen::doStrings(){
        for(IrNodeString* symbol : this->inTree->getStrings()){
            for(char c : symbol->getStr()){
                this->outSymbols.push_back((uint8_t)(c));
            }
            this->outSymbols.push_back((uint8_t)('\0'));
        }
    }
    void CodeGen::doInsts(){

    }
    CodeGen::~CodeGen(){}
};