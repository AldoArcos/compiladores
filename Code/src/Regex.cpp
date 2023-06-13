#include "Regex.hpp"

#define OR 10
#define AND 20
#define POSITIVE 30
#define KLEEN 40
#define OPTIONAL 50
#define LEFT_PARENTHESIS 60
#define RIGHT_PARENTHESIS 70
#define LEFT_BRACKET 80
#define RIGHT_BRACKET 90
#define HYPHEN 100
#define CHARACTER 110

Regex::Regex(std::shared_ptr<LEX> l) : lex(l) {}

bool Regex::E(std::shared_ptr<NFA>& n)
{
    if (T(n))
    {
        if (Ep(n))
            return true;
    }
    return false;
}

bool Regex::Ep(std::shared_ptr<NFA>& n)
{
    int token = lex->yylex();
    std::shared_ptr<NFA> n2 = std::make_shared<NFA>();
    if (token == OR)
    {
        if (T(n2))
        {
            n->Union(n2);
            if (Ep(n))
                return true;
        }
        return false;
    }
    lex->undoToken();
    return true;
}

bool Regex::T(std::shared_ptr<NFA>& n)
{
    if (C(n))
    {
        if (Tp(n))
            return true;
    }
    return false;
}

bool Regex::Tp(std::shared_ptr<NFA>& n)
{
    int token = lex->yylex();
    std::shared_ptr<NFA> n2;
    if (token == AND)
    {
        if (C(n2))
        {
            n->Concatenation(n2);
            if (Tp(n))
            {
                return true;
            }
        }
        return false;
    }
    lex->undoToken();
    return true;
}

bool Regex::C(std::shared_ptr<NFA>& n)
{
    if (F(n))
    {
        if (Cp(n))
        {
            return true;
        }
    }
    return false;
}

bool Regex::Cp(std::shared_ptr<NFA>& n)
{
    int token = lex->yylex();
    if(token == POSITIVE) {
        n->Positive_closure();
        return (Cp(n));
    }
    else if(token == KLEEN) {
        n->Kleene_star();
        return (Cp(n));
    }
    else if(token == OPTIONAL) {
        n->Optional();
        return (Cp(n));
    }
    lex->undoToken();
    return true;
}

bool Regex::F(std::shared_ptr<NFA>& n)
{
    int token = lex->yylex();
    std::string lexema1, lexema2;
    if (token == LEFT_PARENTHESIS)
    {
        if (E(n))
        {
            token = lex->yylex();
            if (token == RIGHT_PARENTHESIS)
                return true;
        }
        return false;
    }
    else if (token == CHARACTER) {
        lexema1 = lex->getLexeme();
        char c = lexema1[0];
        if(lexema1[0] == '\\') c = lexema1[1];
        std::cout << c << "\n";
        n = std::make_shared<NFA>(c,c);
        return true;
    }
    else if (token == LEFT_BRACKET)
    {
        token = lex->yylex();
        if (token == CHARACTER)
        {
            lexema1 = lex->getLexeme();    
            token = lex->yylex();
            if (token == HYPHEN)
            {
                token = lex->yylex();
                if (token == CHARACTER)
                {
                    lexema2 = lex->getLexeme();
                    token = lex->yylex();
                    if (token == RIGHT_BRACKET) {
                        char c = lexema1[0], c2 = lexema2[0];
                        if(lexema1[0] == '\\') c = lexema1[1];
                        if(lexema2[0] == '\\') c2 = lexema2[1];
                        if(c > c2) std::swap(c, c2);
                        n = std::make_shared<NFA>(c, c2);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool Regex::check() {
    if(E(n)) {
        std::cout << "OK" << std::endl;
        n->print();
        return true;
    }
    return false;
}