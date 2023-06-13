#include "MatrixEval.hpp"
#define EQUAL 10
#define SEMICOLON 20
#define PLUS 30
#define MINUS 40
#define ASTERIK 50
#define LEFT_PARENTHESIS 60
#define RIGHT_PARENTHESIS 70
#define LEFT_BRACKET 80
#define RIGHT_BRACKET 90
#define COMMA 100
#define NUMBER 110
#define IDEN 120

bool MatrixEval::Asign(Matrix &m)
{
    std::cout << "Asign\n";
    int token = lexicalAnalyzer->yylex();
    std::cout << "Asign: " << token << "\n";
    if (token == IDEN)
    {
        std::string identificador = lexicalAnalyzer->getLexeme();
        token = lexicalAnalyzer->yylex();
        if (token == EQUAL)
        {
            if (E(m))
            {
                token = lexicalAnalyzer->yylex();
                if (token == SEMICOLON)
                {
                    variables.insert({identificador, m});
                    return true;
                }
            }
        }
    }
    return false;
}

bool MatrixEval::E(Matrix &m)
{
    std::cout << "E\n";
    if (T(m))
    {
        if (Ep(m))
        {
            return true;
        }
    }
    return false;
}

bool MatrixEval::Ep(Matrix &m)
{
    int token = lexicalAnalyzer->yylex();
    if (token == PLUS)
    {
        std::cout << "Ep\n";
        Matrix m2;
        if (T(m2))
        {
            m = m + m2;
            std::cout << m.r << ' ' << m.c << "\n";
            if (Ep(m))
            {
                return true;
            }
        }
        return false;
    }
    else if (token == MINUS)
    {
        Matrix m2;
        if (T(m2))
        {
            m = m - m2;
            std::cout << m.r << ' ' << m.c << "\n";
            if (Ep(m))
            {
                return true;
            }
        }
    }
    lexicalAnalyzer->undoToken();
    return true;
}

bool MatrixEval::T(Matrix &m)
{
    std::cout << "T\n";
    if (F(m))
    {
        if (Tp(m))
        {
            return true;
        }
    }
    return false;
}

bool MatrixEval::Tp(Matrix &m)
{
    int token = lexicalAnalyzer->yylex();
    if (token == ASTERIK)
    {
        Matrix m2;
        if (F(m2))
        {
            m = m * m2;
            if (Tp(m))
            {
                return true;
            }
        }
        return false;
    }
    lexicalAnalyzer->undoToken();
    return true;
}

bool MatrixEval::F(Matrix &m)
{
    std::cout << "F\n";
    int token = lexicalAnalyzer->yylex();
    std::cout << "F: " << token << "\n";
    if (token == LEFT_PARENTHESIS)
    {
        if (E(m))
        {
            token = lexicalAnalyzer->yylex();
            if (token == RIGHT_PARENTHESIS)
            {
                return true;
            }
        }
        return false;
    }
    else if (token == IDEN)
    {
        // Saber si la matrix existe
        std::string identificador = lexicalAnalyzer->getLexeme();
        auto it = variables.find(identificador);
        if (it == variables.end())
            return false;
        m = it->second;
        return true;
    }
    else 
    {
        lexicalAnalyzer->undoToken();
        if (Matriz(m))
            return true;
    }
    return false;
}

bool MatrixEval::Matriz(Matrix &m)
{
    std::cout << "Matrix\n";
    int token = lexicalAnalyzer->yylex();
    std::cout << "Matrix: " << token << "\n";
    if (token == LEFT_BRACKET)
    {
        // Se crea una nueva matriz
        if (Renglones(m))
        {
            token = lexicalAnalyzer->yylex();
            std::cout << "Matrix: " << token << "\n";
            if (token == RIGHT_BRACKET)
            {
                return true;
            }
        }
    }
    return false;
}

bool MatrixEval::Renglones(Matrix &m)
{
    std::cout << "Renglones\n";
    if (Renglon(m))
    {
        if (Renglonesp(m))
        {
            return true;
        }
    }
    return false;
}

bool MatrixEval::Renglonesp(Matrix &m)
{
    std::cout << "RenglonesP\n";
    if (Renglon(m))
    {
        if (Renglonesp(m))
        {
            return true;
        }
        return false;
    }
    lexicalAnalyzer->undoToken();
    return true;
}

bool MatrixEval::Renglon(Matrix &m)
{
    std::cout << "Renglon\n";
    int token = lexicalAnalyzer->yylex();
    std::cout << "Renglon: " << token << "\n";
    if (token == LEFT_BRACKET)
    {
        if (m.appendRow())
        {
            if (ListaNumeros(m))
            {
                token = lexicalAnalyzer->yylex();
                std::cout << "Renglon: " << token << "\n";
                if (token == RIGHT_BRACKET)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool MatrixEval::ListaNumeros(Matrix &m)
{
    std::cout << "ListaNumeros\n";
    int token = lexicalAnalyzer->yylex();
    std::cout << "ListaNumeros: " << token << "\n";
    if (token == NUMBER)
    {
        float x = ::atof(lexicalAnalyzer->getLexeme().c_str());
        std::cout << x << '\n';
        std::cout << lexicalAnalyzer->getLexeme();
        if (m.appendNumber(x))
        {
            if (ListaNumerosp(m))
            {
                return true;
            }
        }
    }
    return false;
}

bool MatrixEval::ListaNumerosp(Matrix &m)
{
    std::cout << "ListaNumerosp\n";
    int token = lexicalAnalyzer->yylex();
    std::cout << "ListaP: " << token << "\n";
    if (token == COMMA)
    {
        token = lexicalAnalyzer->yylex();
        std::cout << "ListaP: " << token << "\n";
        if (token == NUMBER)
        {
            float x = ::atof(lexicalAnalyzer->getLexeme().c_str());
            if (m.appendNumber(x))
            {
                if (ListaNumerosp(m))
                {
                    return true;
                }
            }
        }
        return false;
    }
    lexicalAnalyzer->undoToken();
    return true;
}

bool MatrixEval::check() {
    if(Asign(m)) {
        return true;
    }
    return false;
}