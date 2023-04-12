#ifndef TRANSITION_H
#define TRANSITION_H
#include "State.hpp"
class State;
class NFA;
class Transition {
    friend class NFA;
    private:
        char left_simb;
        char right_simb;
        State * s;

    public:
        Transition(char _simb, State * _s) : left_simb(_simb), right_simb(_simb), s(_s) {}
        Transition(char _left_simb, char _right_simb, State * _s) : left_simb(_left_simb), right_simb(_right_simb), s(_s) {}
        Transition() : right_simb(-1), left_simb(-1), s((State *) nullptr) {}
        ~Transition() {}

        State * getState() {
            return s;
        }

        char getLeft_simb() {
            return left_simb;
        }

        bool isValid(char x) {
            return (left_simb <= x && x <= right_simb);
        }
};

#endif