#pragma once
#include <cstdarg>
#include <string>
#include <vector>

namespace luccix::assembler{
enum struct DiagLevel : int {
    Invalid,
    Note,
    Warning,
    Error,
    Ice,
};

class Location{
    private:
        std::size_t coll;
        std::size_t row;
        std::string fileName;
    public:
        Location(std::string fileName);
        Location(std::string fileName, std::size_t coll, std::size_t row);
        ~Location();
        void print();
        void update(std::size_t newColl, std::size_t newRow);
        std::size_t getRow();
        std::size_t getColl();
};

class Diag{
    private:
        bool colors;
        bool verbose;
        std::vector<std::string> stackTrace;
    public:
        Diag(bool useColors, bool isVerbose);
        ~Diag();
        void vprint(const char* fmt, va_list args);
        void printVerbose(const char* fmt, ...);
        void print(Location *loc, DiagLevel level, const char* fmt, ...);
        void print(DiagLevel level, const char* fmt, ...);
        void addTrace(std::string functionName);
        void popTrace();
        void printTrace();
};
}