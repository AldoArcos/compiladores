#ifndef NFA_H
#define NFA_H
#include "State.hpp"
#include "SetStates.hpp"
#include "DFA.hpp"

#include <iostream>
#include <list>
#include <set>
#include <vector>
#include <queue>
#include <map>

class NFA {
    private:
        inline static int cntIdNFA = 0;
        int id;
        State * initial_state;
        State * final_state;
        std::set<char> alphabet;
        std::map<int, State*> states;
    
    public:
        NFA () : initial_state((State*) nullptr), final_state((State*) nullptr), id(cntIdNFA++) {}
        NFA(char s1, char s2);
        NFA(std::list<NFA*> n);
        ~NFA();

        void Union(NFA * n);
        void Concatenation(NFA * n);
        void Kleene_star();
        void Positive_closure();
        void Optional();
        std::map<int, State*> Epsilon_closure(const std::map<int, State*> &initial_states, bool &containsFinalState, int &token);
        DFA * getDFA();
        void setToken(int token);
        int getId() {
            return id;
        }
        void print();
};
#endif