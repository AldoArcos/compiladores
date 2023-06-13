#include <string>
#include <set>
#include <list>
#include <ReglaGramatical.hpp>
class LL1 {
    public:
        std::set<std::string> First(std::list<ReglaGramatical::Nodo> & l);
        std::set<std::string> Follow(std::string simboloNoTerminal);
    private:
        std::list<ReglaGramatical> reglas;

};