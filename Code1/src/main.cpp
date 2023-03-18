#include <iostream>
#include "NFA.hpp"
#include <map>

#define nl '\n'

using std::cout, std::cin;
int main() {
    std::map<int, NFA*> all_NFA;
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
        cin >> opt;
        cout << "\n\n";
        if(opt == 0) {
            cout << "*** NFA ids ***" << nl;
            for(auto x : all_NFA) cout << x.first << nl;
        }
        if(opt == 1) {
            char symbol;
            cout << "*** NFA basico ***" << nl;
            cout << "Simbolo: ";
            cin >> symbol;
            NFA * n = new NFA(symbol);
            cout << "\nId: " << n->getId() << nl;
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
            dfa->print();
        }
    }

    return 0;
}