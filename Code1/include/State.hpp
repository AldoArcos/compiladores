#ifndef STATE_H
#define STATE_H

#include "Transition.hpp"
#include <vector>
class Transition;

class State {
    private:
        bool accepted;
        int id;
        int token;
        std::vector<Transition> trans;

    public:
        inline static int cntId = 0;

        State() : id(cntId++), accepted(false), token(-1) {}
        State(int len) : id(cntId++), accepted(false), token(-1) {
            trans.resize(len);
        }

        int getId() {
            return id;
        }

        std::vector<Transition> & getTransitions() {
            return trans;
        }

        void addTransition(Transition t) {
            trans.push_back(t);
        }

        void setAccepted(bool _accepted) {
            accepted = _accepted;
        }

        void setTransition(int id, Transition t) {
            trans[id] = t;
        }

        bool isAccepted() {
            return accepted;
        }

        void setToken(int _token) {
            token = _token;
        }
};
#endif