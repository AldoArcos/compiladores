#include "LEX.hpp"

void LEX::setBuffer(const std::string &b)
{
    buffer = b;
    lexemeBegin = 0;
    lexemeEnd = -1;
    token = -1;
    forward = 0;
    isAccepted = false;
}

int LEX::yylex()
{
    while (true)
    {
        // Metemos el inicio del posible token al stack
        starts.push(forward);
        if (forward >= buffer.length())
        {
            lexeme = "";
            return END;
        }

        // Marcamos el inicio del lexema
        lexemeBegin = forward;
        // El fin todavia no se conoce
        lexemeEnd = -1;
        // Estado inicial del DFA
        state = 0;
        isAccepted = false;
        token = -1;

        while (forward < buffer.length())
        {

            // Obtenemos el caracter actual
            currentCharacter = buffer[forward];

            // Obtenemos el siguiente estado
            int nextState = dfa->getTransition(state, currentCharacter);

            // Si existe una transicion con ese caracter
            if (nextState != NO_STATE)
            {
                // Si ese estado le corresponde un token
                if (dfa->getToken(nextState) != NO_TOKEN)
                {
                    isAccepted = true;
                    token = dfa->getToken(nextState);

                    // Puede ser el final del lexema, aunque pueden seguir
                    // otros caracteres que tambien sean parte
                    lexemeEnd = forward;

                }
                forward++;
                state = nextState;
                continue;
            }

            break;
        }

        // Si no se paso por ningun estado de aceptacion
        // No pertenece a ninguna clase lexica
        if (!isAccepted)
        {
            forward = lexemeBegin + 1;
            lexeme = buffer.substr(lexemeBegin, 1);
            token = ERROR;
            return token;
        }

        lexeme = buffer.substr(lexemeBegin, lexemeEnd - lexemeBegin + 1);
        forward = lexemeEnd + 1;
        if (token == OMIT)
            continue;
        else return token;
    }
}

bool LEX::undoToken()
{
    if (starts.empty())
        return false;
    forward = starts.top();
    starts.pop();
    return true;
}

std::string LEX::getLexeme() {
    return lexeme;
}