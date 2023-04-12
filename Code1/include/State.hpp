#ifndef STATE_H
#define STATE_H
#include "Transition.hpp"
#include <list>
#ifndef NO_TOKEN
#define NO_TOKEN -1
#endif
class Transition;
class NFA;

class State {
    friend class NFA;
    private:
        bool isAccepted;
        int id;
        int token;
        std::list<Transition> transitions;

    public:
        inline static int cntId = 0;

        State() : id(cntId++), isAccepted(false), token(NO_TOKEN) {}

        int getId() {
            return id;
        }

        std::list<Transition> & getTransitions() {
            return transitions;
        }

        void addTransition(Transition t) {
            transitions.push_back(t);
        }

        void setAccepted(bool _accepted) {
            isAccepted = _accepted;
        }

        void setToken(int _token) {
            token = _token;
        }
};
#endif