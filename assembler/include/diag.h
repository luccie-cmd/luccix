#pragma once
#include <cstdarg>
#include <string>
#include <stack>

namespace luccix::assembler{
enum struct DiagLevel : int {
    Invalid,
    Note,
    Warning,
    Error,
    Ice,
};

class Diag{
    private:
        bool colors;
        bool verbose;
        std::stack<std::string> stackTrace;
    public:
        Diag(bool useColors, bool isVerbose);
        ~Diag();
        void vprint(const char* fmt, va_list args);
        void printVerbose(const char* fmt, ...);
        void print(DiagLevel level, const char* fmt, ...);
        void addTrace(std::string functionName);
        void popTrace();
        void printTrace();
};
}