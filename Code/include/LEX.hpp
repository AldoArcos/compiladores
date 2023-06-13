#ifndef LEX_H
#define LEX_H
#include "DFA.hpp"
#include "Symbols.hpp"
#include <stack>
#include <string>
#include <memory>

class LEX
{
public:
    LEX(std::shared_ptr<DFA> _dfa) : dfa(_dfa) {}
    void setBuffer(const std::string &b);
    int yylex();
    bool undoToken();
    std::string getLexeme();

private:
    std::shared_ptr<DFA> dfa;
    bool isAccepted;       // Paso por estado de aceptacion
    char currentCharacter; // Caracater actual
    int lexemeBegin;       // Inicio del Lexema
    int lexemeEnd;         // Fin del lexema
    int forward;           // Posicion actual
    int token;             // Token
    int state;
    std::stack<int> starts; // Inicio de cada lexema
    std::string buffer;     // Buffer
    std::string lexeme;     // Ultimo lexema encontrado
};
#endif