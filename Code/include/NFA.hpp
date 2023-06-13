#ifndef NFA_H
#define NFA_H
//#include "SetStates.hpp"
//#include "DFA.hpp"

#include "DFA.hpp"
#include "Symbols.hpp"
#include <iostream>
#include <list>
#include <set>
#include <vector>
#include <queue>
#include <map>
#include <memory>

class NFA
{

    struct Transition;
    struct State;

public:
    // Constructors
    NFA();
    NFA(char s1, char s2);
    NFA(std::list<std::shared_ptr<NFA>> n);
    ~NFA();

    // Classes for building the NFA
    void Union(std::shared_ptr<NFA> n);
    void Concatenation(std::shared_ptr<NFA> n);
    void Kleene_star();
    void Positive_closure();
    void Optional();

    // Auxiliar class for getting the DFA
    std::map<int, std::shared_ptr<State>> Epsilon_closure(const std::map<int, std::shared_ptr<State>> &initial_states, bool &containsFinalState, int &token);
    std::shared_ptr<DFA> getDFA();

    void setToken(const int token);
    int getId() {
        return id;
    }
    void print();

private:
    inline static int cntIdNFA = 0;
    int id;
    std::set<char> alphabet;
    std::shared_ptr<State> initial_state, final_state;
    std::map<int, std::shared_ptr<State>> states;


    struct SetStates
    {
        std::map<int, std::shared_ptr<State>> states;
        std::map<int, int> transitions;
        bool isAccepted;
        int token;
        SetStates(std::map<int, std::shared_ptr<State>> _states, bool _flag, int _token) : states(_states), isAccepted(_flag), token(_token) {}
    };

    struct Transition
    {
        char left_simb;
        char right_simb;
        std::shared_ptr<State> s;
        Transition(char _simb, std::shared_ptr<State> _s) : left_simb(_simb), right_simb(_simb), s(_s) {}
        Transition(char _left_simb, char _right_simb, std::shared_ptr<State> _s) : left_simb(_left_simb), right_simb(_right_simb), s(_s) {}
        Transition() : right_simb(-1), left_simb(-1), s(nullptr) {}
        ~Transition() {}
        bool isValid(char x)
        {
            return (left_simb <= x && x <= right_simb);
        }
    };

    struct State
    {
        bool isAccepted;
        int id;
        int token;
        std::list<Transition> transitions;
        inline static int cntId = 0;
        State() : id(cntId++), isAccepted(false), token(NO_TOKEN) {}
        ~State() {}
        void addTransition(Transition t) {
            transitions.push_back(t);
        }
    };
};

#endif