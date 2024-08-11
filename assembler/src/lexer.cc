#include <lexer.h>

namespace luccix::assembler{
    Lexer::Lexer(std::string inData, std::string inFileName, Diag* diag){
        this->diag = diag;
        this->data = inData;
        this->index = 0;
        this->status = LexerStatus::Progress;
        this->currentLocation = new Location(inFileName, 1, 1);
        this->advance();
    }
    Lexer::~Lexer(){
        this->status = LexerStatus::Invalid;
    }
    void Lexer::advance(){
        this->diag->addTrace(__PRETTY_FUNCTION__);
        if(this->index >= this->data.size()){
            this->c = '\0';
        } else{
            this->currentLocation->update(this->c == '\n' ? 0 : this->currentLocation->getColl() + 1, this->c == '\n' ? this->currentLocation->getRow() + 1 : this->currentLocation->getRow());
            this->c = this->data[this->index++];
        }
        this->diag->popTrace();
    }
    void Lexer::skipWhitespace(){
        while(isspace(this->c) && this->c != '\0'){
            this->advance();
        }
    }
    std::vector<Token*> Lexer::lexLine(){
        this->diag->addTrace(__PRETTY_FUNCTION__);
        std::vector<Token*> tokens;
        while(this->c != '\0' && this->status != LexerStatus::Error){
            this->skipWhitespace();
            switch(this->c){
                // Valid identifier start, identifier will stop after a space
                case 'a': case 'b': case 'c': case 'd': case 'e':
                case 'f': case 'g': case 'h': case 'i': case 'j':
                case 'k': case 'l': case 'm': case 'n': case 'o':
                case 'p': case 'q': case 'r': case 's': case 't':
                case 'u': case 'v': case 'w': case 'x': case 'y':
                case 'z':
                case 'A': case 'B': case 'C': case 'D': case 'E':
                case 'F': case 'G': case 'H': case 'I': case 'J':
                case 'K': case 'L': case 'M': case 'N': case 'O':
                case 'P': case 'Q': case 'R': case 'S': case 'T':
                case 'U': case 'V': case 'W': case 'X': case 'Y':
                case 'Z':
                case '_': {
                    tokens.push_back(this->lexIdentifier());
                } break;

                default: {
                    this->diag->print(this->currentLocation, DiagLevel::Error, "Stray `%c` in program\n", this->c);
                    this->advance();
                    // this->status = LexerStatus::Error;
                    // this->diag->printTrace();
                } break;
            }
            if(this->c == '\n'){
                tokens.push_back(new Token(TokenType::Eol, "Eol"));
                this->advance();
            }
        }
        if(this->c == '\0' && tokens.size() > 0 && tokens.back()->getType() != TokenType::Eof){
            tokens.push_back(new Token(TokenType::Eof, "Eof"));
        }
        this->diag->popTrace();
        return tokens;
    }

    Token* Lexer::lexIdentifier(){
        std::string data(1, this->c);
        this->advance();
        while(!isspace(this->c) && this->c != '\0'){
            data.push_back(this->c);
            this->advance();
        }
        return new Token(TokenType::Identifier, data);
    }
}