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
        NFA () : initial_state(nullptr), final_state(nullptr), id(cntIdNFA++) {}

        NFA(char symbol) : id(cntIdNFA++) {
            State * x = new State();
            State * y = new State();
            y->setAccepted(true);

            states.insert({x->getId(), x});
            states.insert({y->getId(), y});

            alphabet.insert(symbol);
            x->addTransition(Transition(symbol, y));

            initial_state = x;
            final_state = y;
        }

        ~NFA(){}

        void Union(NFA * n);
        void Concatenation(NFA * n);
        void Kleene_star();
        void Positive_closure();
        std::set<int> Epsilon_closure(const std::set<int> & initial_states, bool & flag);
        DFA * getDFA();
        void setToken(int token);
        int getId() {
            return id;
        }
        void print() {
            std::cout << "\n\n";
            std::queue<State*> q;
            q.push(initial_state);
            std::set<int> visited;
            visited.insert(initial_state->getId());
            while (!q.empty()) {
                State * s = q.front();
                q.pop();
                std::cout << "---> Estado: " << s->getId() << "\n";
                std::cout << "\tTransiciones:\n";
                for(Transition t : s->getTransitions()) {
                    std::cout << "\t\tid: " << t.getState()->getId() << "   " << "simbolo: ";
                    if(t.getLeft_simb() == 0) std::cout << "EPS\n";
                    else std::cout << t.getLeft_simb() << "\n";
                    if(visited.find(t.getState()->getId()) == visited.end()) {
                        q.push(t.getState());
                        visited.insert(t.getState()->getId());
                    }
                }
            }
            std::cout << "\n";
        }
};