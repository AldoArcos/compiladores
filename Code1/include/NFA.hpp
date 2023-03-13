#include "State.hpp"
#include "SetStates.hpp"
#include <set>
#include <vector>
#include <queue>
#include <map>

class NFA {
    private:
        State * initial_state;
        State * final_state;
        std::set<char> alphabet;
        std::map<int, State*> states;
    
    public:
        NFA () : initial_state(nullptr), final_state(nullptr) {}

        NFA(char symbol) {
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

        void union_expression(NFA * n);
        void concatenation_expression(NFA * n);
        void kleene_star_expression();
        void positive_closure_expression();
        void getDFA();
};
