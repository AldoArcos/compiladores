#include "State.hpp"
#include <map>
#include <set>
class DFA {
    private:
        State * initial_state;
        std::map<int, State*> states;
        std::set<char> alphabet;

    public:
        DFA(State * _initial_state, std::map<int, State*> _states, std::set<char> _alphabet) : initial_state(_initial_state), states(_states), alphabet(_alphabet) {}
};