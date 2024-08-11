#include <diag.h>

#define ANSI_COLOR_RESET             "\x1b[0m"
#define ANSI_COLOR_BLACK             "\x1b[30m"
#define ANSI_COLOR_RED               "\x1b[31m"
#define ANSI_COLOR_GREEN             "\x1b[32m"
#define ANSI_COLOR_YELLOW            "\x1b[33m"
#define ANSI_COLOR_BLUE              "\x1b[34m"
#define ANSI_COLOR_MAGENTA           "\x1b[35m"
#define ANSI_COLOR_CYAN              "\x1b[36m"
#define ANSI_COLOR_WHITE             "\x1b[37m"
#define ANSI_COLOR_BRIGHT_BLACK      "\x1b[30;1m"
#define ANSI_COLOR_BRIGHT_RED        "\x1b[31;1m"
#define ANSI_COLOR_BRIGHT_GREEN      "\x1b[32;1m"
#define ANSI_COLOR_BRIGHT_YELLOW     "\x1b[33;1m"
#define ANSI_COLOR_BRIGHT_BLUE       "\x1b[34;1m"
#define ANSI_COLOR_BRIGHT_MAGENTA    "\x1b[35;1m"
#define ANSI_COLOR_BRIGHT_CYAN       "\x1b[36;1m"
#define ANSI_COLOR_BRIGHT_WHITE      "\x1b[37;1m"
#define ANSI_BG_COLOR_BLACK          "\x1b[40m"
#define ANSI_BG_COLOR_RED            "\x1b[41m"
#define ANSI_BG_COLOR_GREEN          "\x1b[42m"
#define ANSI_BG_COLOR_YELLOW         "\x1b[43m"
#define ANSI_BG_COLOR_BLUE           "\x1b[44m"
#define ANSI_BG_COLOR_MAGENTA        "\x1b[45m"
#define ANSI_BG_COLOR_CYAN           "\x1b[46m"
#define ANSI_BG_COLOR_WHITE          "\x1b[47m"
#define ANSI_BG_COLOR_BRIGHT_BLACK   "\x1b[40;1m"
#define ANSI_BG_COLOR_BRIGHT_RED     "\x1b[41;1m"
#define ANSI_BG_COLOR_BRIGHT_GREEN   "\x1b[42;1m"
#define ANSI_BG_COLOR_BRIGHT_YELLOW  "\x1b[43;1m"
#define ANSI_BG_COLOR_BRIGHT_BLUE    "\x1b[44;1m"
#define ANSI_BG_COLOR_BRIGHT_MAGENTA "\x1b[45;1m"
#define ANSI_BG_COLOR_BRIGHT_CYAN    "\x1b[46;1m"
#define ANSI_BG_COLOR_BRIGHT_WHITE   "\x1b[47;1m"
#define ANSI_STYLE_BOLD              "\x1b[1m"
#define ANSI_STYLE_UNDERLINE         "\x1b[4m"
#define ANSI_STYLE_REVERSED          "\x1b[7m"

namespace luccix::assembler{
    static std::string DiagLevelToCstr(DiagLevel level){
        switch(level){
            case DiagLevel::Note: {
                return ANSI_COLOR_RESET;
            } break;
            case DiagLevel::Warning: {
                return ANSI_COLOR_BRIGHT_MAGENTA;
            } break;
            case DiagLevel::Error:
            case DiagLevel::Ice: {
                return ANSI_COLOR_BRIGHT_RED;
            } break;
            case DiagLevel::Invalid:
            default: {
                return "Invalid";
            } break;
        }
        return "Unreachable";
    }

    static std::string LevelToCstr(DiagLevel level){
        switch(level){
            case DiagLevel::Note: {
                return "Note";
            } break;
            case DiagLevel::Warning: {
                return "Warning";
            } break;
            case DiagLevel::Error: {
                return "Error";
            } break;
            case DiagLevel::Ice: {
                return "Ice";
            } break;
            case DiagLevel::Invalid:
            default: {
                return "Invalid";
            } break;
        }
        return "Unreachable";
    }

    Diag::Diag(bool useColors, bool isVerbose){
        this->colors = useColors;
        this->verbose = isVerbose;
    }

    Diag::~Diag(){}

    void Diag::vprint(const char* fmt, va_list args){
        std::vprintf(fmt, args);
    }

    void Diag::printVerbose(const char* fmt, ...){
        if(!this->verbose){
            return;
        }
        va_list args;
        va_start(args, fmt);
        this->vprint(fmt, args);
        va_end(args);
    }

    void Diag::print(DiagLevel level, const char* fmt, ...){
        if(this->colors){
            std::printf("%s", DiagLevelToCstr(level).c_str());
        }
        printf("%s: ", LevelToCstr(level).c_str());
        printf("%s", ANSI_COLOR_RESET);
        va_list args;
        va_start(args, fmt);
        this->vprint(fmt, args);
        va_end(args);
    }

    void Diag::addTrace(std::string functionName){
        this->stackTrace.push(functionName);
    }

    void Diag::popTrace(){
        this->stackTrace.pop();
    }

    void Diag::printTrace(){
        std::printf("Stack trace:\n");
        for(int64_t i = 0; i < (int64_t)this->stackTrace.size(); ++i){
            std::printf("%s\n", this->stackTrace.top().c_str());
            this->stackTrace.pop();
        }
    }
}