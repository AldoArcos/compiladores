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

std::map<int, State*> NFA::Epsilon_closure(const std::map<int, State*> & initial_states, bool & flag) {

    std::queue<State*> q; 
    std::map<int,State*> visited;

    for(auto[id, s] : initial_states) {
        visited.insert({id, s});
        q.push(s);
    }

    while(!q.empty()) {
        State * s = q.front();
        q.pop();
        if(s->isAccepted()) flag = 1; // El estado generado tiene un estado final.

        for(Transition t : s->getTransitions()) {
            if(t.isValid(EPSILON) && visited.count(t.getState()->getId()) == 0) {
                visited.insert({t.getState()->getId(), t.getState()});
                q.push(t.getState());
            }
        }
    }
    return visited;
}

DFA *  NFA::getDFA()
{
    std::map<int, State*> initial_states;
    initial_states.insert({initial_state->id, initial_state});

    bool flag = false;
    std::map<int, State*> ans = Epsilon_closure(initial_states,flag);

    // Lista de los estados que vayamos generando
    std::map<int,SetStates*> st;  
    std::queue<SetStates*> q;

    int cntStates = 0;
    SetStates * initial_dfa_state = new SetStates(ans, flag);

    st.insert({cntStates++, initial_dfa_state});
    q.push(initial_dfa_state);
    

    while(!q.empty()) {

        SetStates * s = q.front();
        q.pop();

        s->print();
        

        // Iteramos por cada simbolo del alfabeto
        for(char symbol : alphabet) {

            std::cout << symbol << ":\n";
            flag = false;
            initial_states.clear();
            ans.clear();

            // Vemos a que estados podemos llegar con el actual simbolo
            for(auto [id, x] : s->states) {

                for(Transition t : x->trans) {

                    if(t.getState() == nullptr) continue;

                    if(t.isValid(symbol)) {
                        // Si existe una transicion con el simbolo
                        initial_states.insert({t.getState()->getId(), t.getState()});
                    }

                }

            }

            // Creamos un nuevo estado
            ans = Epsilon_closure(initial_states, flag);

            std::cout << "Estados a los que podemos llegar = ";
            for(auto [id, s] : ans) std::cout << id << " ";
            std::cout << '\n';

            if(!ans.empty()) {

                bool flag_new_state = 1;

                // Verificamos que ese estado no exista

                for(auto[id, x] : st) {

                    if(x->states == ans) {
                        // Ya existe un estado con esos estados iniciales
                        flag_new_state = 0;

                        // Creamos la transicion
                        s->transitions[symbol] = id;

                        break;
                    }

                }

                // Sino existe ningun estado con esos estados iniciales
                if(flag_new_state) {

                    SetStates *aux = new SetStates(ans, flag);

                    s->transitions[symbol] = cntStates;

                    st.insert({cntStates++, aux});

                    q.push(aux);

                }

            }

        }
    }
    

    DFA * dfa = new DFA(alphabet, cntStates);
    for(auto [i, s] : st) {
        for(auto [j, id] : s->transitions) 
            dfa->trans[i][j] = id;
    }
    return dfa;
}

void NFA::setToken(int token) {
    final_state->setToken(token);
}