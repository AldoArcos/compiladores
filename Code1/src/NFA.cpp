#include "NFA.hpp"
#ifndef EPSILON
#define EPSILON 0
#endif

void NFA::Union(NFA *n)
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

void NFA::Concatenation(NFA *n)
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

void NFA::Kleene_star()
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

void NFA::Positive_closure()
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

std::set<int> NFA::Epsilon_closure(const std::set<int> & initial_states, bool & flag) {
    std::queue<int> q; 
    std::set<int> visited; // Id de estados visitados
    for(int idState : initial_states) {
        q.push(idState);
        visited.insert(idState);
    }

    while(!q.empty()) {
        int id = q.front();
        q.pop();
        State * s = states[id];
        if(s->isAccepted()) flag = 1;
        for(Transition t : s->getTransitions()) {
            if(t.isValid(EPSILON) && visited.find(t.getState()->getId()) == visited.end()) {
                visited.insert(t.getState()->getId());
                q.push(t.getState()->getId());
            }
        }
    }
    return visited;
}

DFA *  NFA::getDFA()
{
    std::set<int> initial_states;
    initial_states.insert(initial_state->getId());

    bool flag = false;
    std::set<int> reachable_states = Epsilon_closure(initial_states,flag);

    // Lista de los estados que vayamos generando
    std::list<SetStates*> st;  

    SetStates * initial_dfa_state = new SetStates(initial_states, reachable_states, flag);
    st.push_back(initial_dfa_state);
    

    for(SetStates * s : st) {
        
        // Iteramos por cada simbolo del alfabeto
        for(char symbol : alphabet) {
            
            flag = false;
            initial_states.clear();
            reachable_states.clear();

            // Vemos a que estados podemos llegar con el actual simbolo
            for(int id : s->getAll_ids()) {
                for(Transition t : states[id]->getTransitions()) {
                    if(t.isValid(symbol)) {
                        // Si existe una transicion con el simbolo
                        initial_states.insert(id);
                    }
                }
            }

            // Creamos un nuevo estado
            reachable_states = Epsilon_closure(initial_states, flag);

            if(!reachable_states.empty()) {

                bool flag_new_state = 1;

                for(SetStates * aux : st) {
                    if(*aux == *s) {
                        // Ya existe un estado con esos estados iniciales
                        flag_new_state = 0;
                        s->setTransition(symbol, Transition(symbol, aux));
                        break;
                    }
                }

                // Sino existe ningun estado con esos estados iniciales
                if(flag_new_state) {
                    SetStates *aux = new SetStates(initial_states, reachable_states, flag);
                    s->setTransition(symbol, Transition(symbol, aux));
                    st.push_back(aux);
                }
            }
        }
    }
    std::map<int, State*> dfa_states;
    for(SetStates * s : st) {
        dfa_states.insert({s->getId(), s});
    }

    DFA * dfa = new DFA(initial_dfa_state, dfa_states, alphabet);
    return dfa;
}

void NFA::setToken(int token) {
    final_state->setToken(token);
}