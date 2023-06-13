#ifndef REGEX_H
#define REGEX_H

#include "LEX.hpp"
#include "NFA.hpp"
#include <memory>
#include <string>
#include <utility>
#include <iostream>

class Regex
{
private:
    std::shared_ptr<LEX> lex;
    bool Ep(std::shared_ptr<NFA>& n);
    bool T(std::shared_ptr<NFA>& n);
    bool Tp(std::shared_ptr<NFA>& n);
    bool C(std::shared_ptr<NFA>& n);
    bool Cp(std::shared_ptr<NFA>& n);
    bool F(std::shared_ptr<NFA>& n);
    bool E(std::shared_ptr<NFA>& n); // Simbolo inicial
public:
    std::shared_ptr<NFA> n;
    Regex(std::shared_ptr<LEX> l);
    bool check();
};
#endif