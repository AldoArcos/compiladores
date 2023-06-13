#include <list>
#include <string>
struct ReglaGramatical {
    struct Nodo {
        std::string simbolo;
        bool terminal;
        bool last;
        Nodo(std::string s, bool f) : simbolo(s), terminal(f) {}
    };
    std::string ladoIzquierdo;
    std::list<Nodo> l;

    ReglaGramatical() {}
};