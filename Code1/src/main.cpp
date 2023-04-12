#include <iostream>
#include "NFA.hpp"
#include "Lex.hpp"
#include <map>
#include <limits>

#define nl '\n'

using std::cout, std::cin;
int main() {
    std::map<int, NFA*> all_NFA;
    std::map<int, DFA*> all_DFA;
    int idDFA = 0;
    int opt;

    while(1) {
        cout << "0. NFA ids" << nl;
        cout << "1. NFA basico" << nl;
        cout << "2. NFA union" << nl;
        cout << "3. NFA concatenation" << nl;
        cout << "4. NFA kleen closure" << nl;
        cout << "5. NFA positive closure" << nl;
        cout << "6. NFA optional" << nl;
        cout << "7. NFA print" << nl;
        cout << "8. NFA to DFA" << nl;
        cout << "9. Set token" << nl;
        cout << "10. Save DFA" << nl;
        cout << "11. Load DFA" << nl;
        cout << "\n\n";
        cin >> opt;
        if(opt == 0) {
            //cout << "*** NFA ids ***" << nl;
            for(auto x : all_NFA) cout << x.first << nl;
        }
        else if(opt == 1) {
            char s1, s2;
            cout << "*** NFA basico ***" << nl;
            cout << "Simbolo: ";
            cin >> s1 >> s2;
            NFA * n = new NFA(s1,s2);
            //cout << "\nId: " << n->getId() << nl;
            all_NFA.insert({n->getId(), n});
        }
        else if(opt == 2) {

            cout << "*** NFA union ***" << nl;
            int id1, id2;
            cout << "Id1: ";
            cin >> id1;
            cout << "Id2: ";
            cin >> id2;
            if(id1 == id2) {
                cout << "Ids tienene que ser distintos" << nl;
                return 0;
            }
            all_NFA[id1]->Union(all_NFA[id2]);
            all_NFA.erase(id2);
        }
        else if(opt == 3) {
            cout << "*** NFA concatenation ***" << nl;
            int id1, id2;
            cout << "Id1: ";
            cin >> id1;
            cout << "Id2: ";
            cin >> id2;
            if(id1 == id2) {
                cout << "Ids tienene que ser distintos" << nl;
                return 0;
            }
            all_NFA[id1]->Concatenation(all_NFA[id2]);
            all_NFA.erase(id2);
        }
        else if(opt == 4) {
            cout << "*** NFA kleen ***" << nl;
            int id;
            cout << "Id: ";
            cin >> id;
            all_NFA[id]->Kleene_star();
        }
        else if(opt == 5) {
            cout << "*** NFA positive ***" << nl;
            int id;
            cout << "Id: ";
            cin >> id;
            all_NFA[id]->Positive_closure();
        }
        else if(opt == 6) {
            cout << "*** NFA optional ***" << nl;
            int id;
            cout << "Id: ";
            cin >> id;
            all_NFA[id]->Optional();
        }
        else if(opt == 7) {
            cout << "*** NFA print ***" << nl;
            int id;
            cout << "Id: ";
            cin >> id;
            all_NFA[id]->print();
        }
        else if(opt == 8) {
            cout << "*** NFA to DFA ***" << nl;
            cout << "Total: " << nl;
            int cnt;
            int id;
            cin >> cnt;
            std::list<NFA*> l;
            for(int i=0; i<cnt; i++) {
                cout << "Id: ";
                cin >> id;
                l.push_back(all_NFA[id]);
                all_NFA.erase(id);
            }
            NFA * n = new NFA(l);
            DFA * dfa = n->getDFA();
            all_DFA[idDFA++] = dfa;
            //dfa->print();
        }
        else if(opt == 9) {
            cout << "*** NFA token ***" << nl;
            cout << "id: " << nl;
            int id, token;
            cin >> id;
            cout << "token: " << nl;
            cin >> token;
            all_NFA[id]->setToken(token);
        }
        else if(opt == 10) {
            cout << "*** Save DFA *** " << nl;
            int id;
            cin >> id;
            std::string path;
            cin >> path;
            all_DFA[id]->saveDFAToFile(path);
        }
        else if(opt == 11) {
            cout << "*** Load DFA ***" << nl;
            std::string path;
            cin >> path;
            DFA * dfa = new DFA();
            dfa->loadDFAFromFile(path);
            dfa->print();
            all_DFA[idDFA++] = dfa;
        }
        else if(opt == 12) {
            cout << "*** Tokens ***" << nl;
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');            
            std::string cadena;
            std::getline(cin, cadena);
            cout << cadena << nl;
            int id;
            cin >> id;
            Lex * l = new Lex(all_DFA[id]);
            l->setBuffer(cadena);
            int token;
            while((token = l->yylex()) != '\0') {
                cout << ' ' << token << nl;
            }
            int aux;
            cin >> aux;
            if(aux == 10) break;
        }
        else break;
    }

    return 0;
}