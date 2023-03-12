#include <set>
#include <queue>
#include <vector>

#define EPSILON 0
class State {
    private:
        bool accepted;
        int id;
        int token;
        Transition trans[2];

    public:
        inline static int cntId = 0;
        State() : id(cntId++), accepted(false), token(-1) {
            trans[0] = Transition();
            trans[1] = Transition();
        }

        Transition getTransition(int i) {
            return trans[i];
        }

        void addTransition(Transition t) {
            if(!trans[0].getState()) {
                trans[0] = t;
            }
            else if(!trans[1].getState()) {
                trans[1] = t;
            }
        }

        void setAccepted(bool _accepted) {
            accepted = _accepted;
        }

};

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

        char getSimbol() {
            return simb;
        }

};

class NFA {
    private:
        State * initial;
        State * final;
        std::set<char> alphabet;
    
    public:
        NFA () : initial(nullptr), final(nullptr) {}

        NFA(char symbol) {
            State * x = new State();
            State * y = new State();

            Transition t = Transition(symbol, x);

            alphabet.insert(symbol);
            x->addTransition(t);
            y->setAccepted(true);

            initial = x;
            final = y;
        }

        ~NFA(){}

        void union_expression(NFA * n) {
            State * x = new State();
            State * y = new State();

            y->setAccepted(true);

            alphabet.merge(n->alphabet);

            Transition t1 = Transition(EPSILON, initial);
            Transition t2 = Transition(EPSILON, n->initial);

            x->addTransition(t1);
            x->addTransition(t2);

            Transition t3 = Transition(EPSILON, y);

            final->addTransition(t3);
            n->final->addTransition(t3);

            final->setAccepted(false);
            n->final->setAccepted(false);

            initial = x;
            final = y;
            delete n;
        }

        static NFA * union_expression(NFA * n1, NFA * n2) {
            NFA * res = new NFA();
            State * x = new State();
            State * y = new State();

            y->setAccepted(true);

            res->alphabet.merge(n1->alphabet);
            res->alphabet.merge(n2->alphabet);

            Transition t1 = Transition(EPSILON, n1->initial);
            Transition t2 = Transition(EPSILON, n2->initial);

            x->addTransition(t1);
            x->addTransition(t2);

            Transition t3 = Transition(EPSILON, y);

            n1->final->addTransition(t3);
            n2->final->addTransition(t3);

            n1->final->setAccepted(false);
            n2->final->setAccepted(false);

            res->initial = x;
            res->final = y;

            delete n1;
            delete n2;

            return res;
        }


        void concatenation_expression(NFA * n) {

            alphabet.merge(n->alphabet);

            final->setAccepted(false);
            final->addTransition(n->initial->getTransition(0));
            final->addTransition(n->initial->getTransition(1));

            final = n->final;

            delete n->initial;
            delete n;
        }

        static NFA * concatenation_expression(NFA * n1, NFA * n2) {
            NFA * res = new NFA();

            res->alphabet.merge(n1->alphabet);
            res->alphabet.merge(n2->alphabet);

            n1->final->setAccepted(false);
            n1->final->addTransition(n2->initial->getTransition(0));
            n1->final->addTransition(n2->initial->getTransition(1));

            res->initial = n1->initial;
            res->final = n2->final;

            delete n2->initial;
            delete n1;
            delete n2;
            return res;
        }


        void kleene_star_expression() {
            State * x = new State();
            State * y = new State();
            y->setAccepted(true);
            final->setAccepted(false);
            Transition t1 = Transition(EPSILON, y);
            Transition t2 = Transition(EPSILON, initial);
            final->addTransition(t1);
            final->addTransition(t2);
            Transition t3 = Transition(EPSILON, initial);
            Transition t4 = Transition(EPSILON, y);
            x->addTransition(t3);
            x->addTransition(t4);
            initial = x;
            final = y;
        }

        static NFA * kleene_star_expression(NFA * n1) {
            NFA * res = new NFA();
            State * x = new State();
            State * y = new State();

            res->alphabet.merge(n1->alphabet);
            y->setAccepted(true);
            n1->final->setAccepted(false);

            Transition t1 = Transition(EPSILON, y);
            Transition t2 = Transition(EPSILON, n1->initial);
            n1->final->addTransition(t1);
            n1->final->addTransition(t2);

            Transition t3 = Transition(EPSILON, n1->initial);
            Transition t4 = Transition(EPSILON, y);
            x->addTransition(t3);
            x->addTransition(t4);

            res->initial = x;
            res->final = y;
            delete n1;

            return res;
        }

        static NFA * positive_closure_expression(NFA * n1) {
            NFA * res = new NFA();  
            State * x = new State();
            State * y = new State();

            res->alphabet.merge(n1->alphabet);
            y->setAccepted(true);
            n1->final->setAccepted(false);

            Transition t1 = Transition(EPSILON, y);
            Transition t2 = Transition(EPSILON, n1->initial);
            n1->final->addTransition(t1);
            n1->final->addTransition(t2);

            Transition t3 = Transition(EPSILON, n1->initial);
            x->addTransition(t3);

            res->initial = x;
            res->final = y;
            delete n1;

            return res;
        }

        void positive_closure_expression() {
            State * x = new State();
            State * y = new State();

            y->setAccepted(true);
            final->setAccepted(false);

            Transition t1 = Transition(EPSILON, y);
            Transition t2 = Transition(EPSILON, initial);
            final->addTransition(t1);
            final->addTransition(t2);
            Transition t3 = Transition(EPSILON, initial);
            x->addTransition(t3);

            initial = x;
            final = y;
        }

        State * getInitialState() {
            return initial;
        }

        State * getFinalState() {
            return final;
        }

        void getDFA() {
            std::set<int> s0;
            std::queue<State*> q;
            std::vector<int> visited(State::cntId);
            q.push(initial);
            while(!q.empty()) {

            }

        }

};

class DFA {

};