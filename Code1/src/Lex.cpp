#include "Lex.hpp"
#define TOKEN 256
#define OMIT 1000
#define END '\0'
#define ERROR -1000

void Lex::setBuffer(const std::string &b) {
    buffer = b;
    lexemeBegin = 0;
    lexemeEnd = -1;
    token = -1;
    forward = 0;
    isAccepted = false;
}

int Lex::yylex() {
    while(true) {
        starts.push(forward);
        if(forward >= buffer.length()) {
            lexeme = "";
            return END;
        }
        lexemeBegin = forward;
        lexemeEnd = -1;
        state = 0;
        isAccepted = false;
        token = -1;
        while (forward < buffer.length())
        {
            currentCharacter = buffer[forward];
            int nextState = dfa->trans[state][currentCharacter];
            if(nextState != -1) {
                if(dfa->trans[nextState][TOKEN] != -1) {
                    isAccepted = true;
                    token = dfa->trans[nextState][TOKEN];
                    lexemeEnd = forward;
                }
                forward++;
                state = nextState;
                continue;
            }
            break;
        }

        if(!isAccepted) {
            forward = lexemeBegin + 1;
            lexeme = buffer.substr(lexemeBegin, 1);
            token = ERROR;
            return token;
        }
        lexeme = buffer.substr(lexemeBegin, lexemeEnd - lexemeBegin + 1);
        forward = lexemeEnd + 1;

        if(token == OMIT) continue;
        else {
            for(int i=lexemeBegin; i<=lexemeEnd; i++) {
                std::cout << buffer[i];
            }
            std::cout << ' ';
            return token;
        }
    }

}

bool Lex::undoToken()
{
    if (starts.empty())
        return false;
    forward = starts.top();
    starts.pop();
    return true;
}