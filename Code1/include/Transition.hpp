#ifndef TRANSITION_H
#define TRANSITION_H
#include "State.hpp"
class State;

class Transition {
    private:
        char simb;
        State * s;

    public:
        Transition(char _simb, State * _s) : simb(_simb), s(_s) {}
        Transition() : simb(-1), s(nullptr) {}
        ~Transition() {}

        State * getState() {
            return s;
        }

        char getSymbol() {
            return simb;
        }
};

#endif