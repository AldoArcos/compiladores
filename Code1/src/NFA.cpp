#include "NFA.hpp"
#ifndef EPSILON
#define EPSILON 0
#endif

void NFA::union_expression(NFA *n)
{
    State *x = new State();
    State *y = new State();
    states.insert({x->getId(), x});
    states.insert({y->getId(), y});

    y->setAccepted(true);
    final_state->setAccepted(false);

    alphabet.merge(n->alphabet);
    states.merge(n->states);

    x->addTransition(Transition(EPSILON, initial_state));
    x->addTransition(Transition(EPSILON, n->initial_state));

    final_state->addTransition(Transition(EPSILON, y));
    n->final_state->addTransition(Transition(EPSILON, y));

    final_state->setAccepted(false);
    n->final_state->setAccepted(false);

    initial_state = x;
    final_state = y;
}

void NFA::concatenation_expression(NFA *n)
{

    states.merge(n->states);
    alphabet.merge(n->alphabet);
    final_state->setAccepted(false);

    auto trans = n->initial_state->getTransitions();

    for (auto t : trans)
    {
        final_state->addTransition(t);
    }

    final_state = n->final_state;
}

void NFA::kleene_star_expression()
{

    State *x = new State();
    State *y = new State();

    states.insert({x->getId(), x});
    states.insert({y->getId(), y});
    y->setAccepted(true);
    final_state->setAccepted(false);

    final_state->addTransition(Transition(EPSILON, y));
    final_state->addTransition(Transition(EPSILON, initial_state));

    x->addTransition(Transition(EPSILON, initial_state));
    x->addTransition(Transition(EPSILON, y));

    initial_state = x;
    final_state = y;
}

void NFA::positive_closure_expression()
{
    State *x = new State();
    State *y = new State();

    states.insert({x->getId(), x});
    states.insert({y->getId(), y});
    y->setAccepted(true);
    final_state->setAccepted(false);

    final_state->addTransition(Transition(EPSILON, y));
    final_state->addTransition(Transition(EPSILON, initial_state));

    x->addTransition(Transition(EPSILON, initial_state));

    initial_state = x;
    final_state = y;
}

void NFA::getDFA()
{
    std::set<int> state_ids;
    std::set<int> initial;
    std::set<int> visited;
    std::queue<int> q;
    initial.insert(initial_state->getId());
    q.push(initial_state->getId());

    while(!q.empty()) {
        int id = q.front();
        State * s = states[id];
        state_ids.insert(id);
        for(Transition t : s->getTransitions()) {
            if(t.getSymbol() == EPSILON && visited.find(t.getState()->getId()) == visited.end()) {
                visited.insert(t.getState()->getId());
                q.push(t.getState()->getId());
            }
        }
    }


    std::list<SetStates*> st;
    st.push_back(new SetStates(initial, state_ids));

    for(SetStates * s : st) {
        for(char symbol : alphabet) {
            initial.clear();
            state_ids.clear();
            visited.clear();
            for(int id : s->getAll_ids()) {
                for(Transition t : states[id]->getTransitions()) {
                    if(t.getSymbol() == symbol) {
                        initial.insert(id);
                    }
                }
            }

            for(int id : initial) {
                q.push(id);
            }

            while(!q.empty()) {
                int id = q.front();
                State * s = states[id];
                state_ids.insert(id);
                for(Transition t : s->getTransitions()) {
                    if(t.getSymbol() == EPSILON && visited.find(t.getState()->getId()) == visited.end()) {
                        visited.insert(t.getState()->getId());
                        q.push(t.getState()->getId());
                    }
                }
            }

        }
    }
}
