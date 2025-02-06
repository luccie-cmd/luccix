#pragma once
#include "ir.h"

namespace luccix::assembler{
    class CodeGen{
        private:
            IrTree* inTree;
            std::vector<uint8_t> outSymbols;
            std::vector<uint8_t> outStrings;
            std::vector<uint8_t> outInsts;
            std::string outFile;
            void doSymbols();
            void doStrings();
            void doInsts();
        public:
            CodeGen(IrTree* tree, std::string outFile);
            ~CodeGen();
            void doCodegen();
    };
};