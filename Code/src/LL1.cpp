#include "LL1.hpp"

std::set<std::string> LL1::First(std::list<ReglaGramatical::Nodo> & l) {
    std::set<std::string> r;
    if(l.empty()) return r;
    for(auto N : l) {
        if(N.terminal || N.simbolo == "EPSILON") {
            r.insert(N.simbolo);
            return r;
        }

        // Si el simbolo es un no terminal
        // Nos fijamos en el lado izquierdo de las listas
        for(auto regla : reglas) {
            
            // Se quedaria en un ciclo infinito al no ser un simbolo terminal
            // Y tambien porque es el mismo
            if(regla.l.begin()->simbolo == N.simbolo) {
                continue;
            }

            if(regla.ladoIzquierdo == N.simbolo) {
                std::set<std::string> ans = First(regla.l);
                r.insert(ans.begin(), ans.end());
            }
        }

        // Si no tiene un epsilon el resultado acabamos el algoritmo
        if(r.find("EPSILON") != r.end()) {
            if(N.last) continue;
            r.erase("EPSILON");
        }
        else break;
    }
    return r;
}

std::set<std::string> LL1::Follow(std::string simboloNoTerminal) {
    int cnt = 0;
    std::set<std::string> ans;
    for(auto regla : reglas) {
        for(auto l : regla.l) {
            if(l.simbolo == simboloNoTerminal && !l.last) {
                std::list<ReglaGramatical::Nodo> aux;
                int j = 0;
                for(auto x : regla.l) {
                    if(j >= cnt) aux.push_back(x);
                    j++;
                }
                std::set<std::string> temp = First(aux);
                if(temp.find("EPSILON") != temp.end()) {
                    temp.erase("EPSILON");
                    std::set<std::string> temp2 = Follow(regla.ladoIzquierdo);
                    ans.insert(temp2.begin(), temp2.end());
                }
                ans.insert(temp.begin(), temp.end());
                break;
            }
            else if(l.simbolo == simboloNoTerminal){
                if(regla.ladoIzquierdo == l.simbolo) continue;
                std::set<std::string> temp = Follow(regla.ladoIzquierdo);
                ans.insert(temp.begin(), temp.end());
            }
            cnt++;
        }
    }
    if(simboloNoTerminal == reglas.begin()->ladoIzquierdo) {
        ans.insert("$");
    }
    return ans;
}