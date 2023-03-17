#ifndef STATE_H
#define STATE_H
#include "Transition.hpp"
#include <list>
class Transition;
class NFA;

class State {
    friend class NFA;
    private:
        bool accepted;
        int id;
        int token;
        std::list<Transition> trans;

    public:
        inline static int cntId = 0;

        State() : id(cntId++), accepted(false), token(-1) {}

        int getId() {
            return id;
        }

        std::list<Transition> & getTransitions() {
            return trans;
        }

        void addTransition(Transition t) {
            trans.push_back(t);
        }

        void setAccepted(bool _accepted) {
            accepted = _accepted;
        }

        bool isAccepted() {
            return accepted;
        }

        void setToken(int _token) {
            token = _token;
        }
};
#endif