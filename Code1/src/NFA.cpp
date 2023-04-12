#include "NFA.hpp"
#ifndef EPSILON
#define EPSILON 0
#endif
#ifndef NO_TOKEN
#define NO_TOKEN -1
#endif

NFA::NFA(char s1, char s2) : id(cntIdNFA++)
{
    State *x = new State();
    State *y = new State();
    y->isAccepted = true;

    states.insert({x->id, x});
    states.insert({y->id, y});

    for(char c=s1; c<=s2; c++)
        alphabet.insert(c);

    x->addTransition(Transition(s1,s2, y));
    

    initial_state = x;
    final_state = y;
}

void NFA::Union(NFA *n)
{
    State *x = new State();
    State *y = new State();

    states.insert({x->id, x});
    states.insert({y->id, y});


    alphabet.merge(n->alphabet);
    states.merge(n->states);

    x->addTransition(Transition(EPSILON, initial_state));
    x->addTransition(Transition(EPSILON, n->initial_state));

    final_state->addTransition(Transition(EPSILON, y));
    n->final_state->addTransition(Transition(EPSILON, y));

    final_state->isAccepted = false;
    n->final_state->isAccepted = false;
    y->isAccepted = true;

    initial_state = x;
    final_state = y;
    delete n;
}

void NFA::Concatenation(NFA *n)
{
    states.merge(n->states);
    alphabet.merge(n->alphabet);
    final_state->isAccepted = false;

    for (const auto &t : n->initial_state->transitions)
    {
        final_state->addTransition(t);
    }

    final_state = n->final_state;
    delete n;
}

void NFA::Kleene_star()
{

    State *x = new State();
    State *y = new State();

    states.insert({x->id, x});
    states.insert({y->id, y});

    y->isAccepted = true;
    final_state->isAccepted = false;

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

    states.insert({x->id, x});
    states.insert({y->id, y});
    y->isAccepted = true;
    final_state->isAccepted = false;

    final_state->addTransition(Transition(EPSILON, y));
    final_state->addTransition(Transition(EPSILON, initial_state));

    x->addTransition(Transition(EPSILON, initial_state));

    initial_state = x;
    final_state = y;
}

void NFA::Optional() 
{
    State * x = new State();
    State * y = new State();

    states.insert({x->id, x});
    states.insert({y->id, y});

    y->isAccepted = true;
    final_state->isAccepted = false;

    final_state->addTransition(Transition(EPSILON, y));

    x->addTransition(Transition(EPSILON, initial_state));
    x->addTransition(Transition(EPSILON, y));

    initial_state = x;
    final_state = y;
}

std::map<int, State*> NFA::Epsilon_closure(const std::map<int, State*> & initial_states, bool & isFinalState, int & token) {

    std::queue<State*> q; 
    std::map<int,State*> visited;

    // Marcamos como visitados todos los nodos a los que podemos llegar
    for(auto[id, s] : initial_states) {
        visited.insert({id, s});
        q.push(s);
    }

    while(!q.empty()) {
        State * s = q.front();
        q.pop();
        if(s->isAccepted) {
            isFinalState = true; // El estado generado tiene un estado final.
            token = s->token;    
        }

        for(Transition &t : s->transitions) {
            if(t.isValid(EPSILON) && visited.count(t.s->id) == 0) {
                visited.insert({t.s->id, t.s});
                q.push(t.s);
            }
        }
    }
    return visited;
}

DFA *  NFA::getDFA()
{

    // Estados iniciales
    std::map<int, State*> initial_states;
    // Se inserta primero el estado inicial
    initial_states.insert({initial_state->id, initial_state});

    // Bandera para saber si contiene un estado final
    bool isFinalState = false;
    int token = NO_TOKEN;

    // Estados a los que podemos llegar
    std::map<int, State*> ans = Epsilon_closure(initial_states, isFinalState, token);

    // Lista de los estados que vayamos generando
    std::map<int,SetStates*> st;  

    // Orden para los nuevos estados
    std::queue<SetStates*> q;

    int cntStates = 0;
    SetStates * initial_dfa_state = new SetStates(ans, isFinalState, token);

    st.insert({cntStates++, initial_dfa_state});
    q.push(initial_dfa_state);
    

    while(!q.empty()) {

        SetStates * s = q.front();
        q.pop();

        //s->print();
        

        // Iteramos por cada simbolo del alfabeto
        for(char symbol : alphabet) {

            //std::cout << symbol << ":\n";
            token = NO_TOKEN;
            isFinalState = false;
            initial_states.clear();
            ans.clear();

            // Vemos a que estados podemos llegar con el actual simbolo y estado
            for(auto [id, x] : s->states) {

                for(Transition t : x->transitions) {

                    if(t.s == (State*) nullptr) continue;

                    if(t.isValid(symbol)) {
                        // Si existe una transicion con el simbolo
                        initial_states.insert({t.s->id, t.s});
                    }

                }

            }

            // Creamos un nuevo estado
            ans = Epsilon_closure(initial_states, isFinalState, token);

            //std::cout << "Estados a los que podemos llegar = ";
            //for(auto [id, s] : ans) std::cout << id << " ";
            //std::cout << '\n';

            // Si podemos llegar a otros estados
            if(!ans.empty()) {

                bool isNewState = true;

                // Verificamos que ese estado no exista

                for(auto[id, x] : st) {

                    if(x->states == ans) {
                        // Ya existe un estado con esos estados iniciales
                        isNewState = false;

                        // Creamos la transicion
                        s->transitions[symbol] = id;

                        break;
                    }

                }

                // Sino existe ningun estado con esos estados iniciales
                if(isNewState) {

                    SetStates *aux = new SetStates(ans, isFinalState, token);

                    s->transitions[symbol] = cntStates;

                    st.insert({cntStates++, aux});

                    q.push(aux);

                }

            }

        }
    }
    

    DFA * dfa = new DFA(alphabet, cntStates);

    // Con todos los nuevos estados que generamos
    for(auto [id1, s] : st) {
        for(auto [symbol, id2] : s->transitions) 
            dfa->trans[id1][symbol] = id2;
        dfa->trans[id1][256] = s->token;
    }

    for(auto [id, s] : st) {
        delete s;
    }

    return dfa;
}

NFA::NFA(std::list<NFA*> n) {
    State * x = new State();
    for(NFA * y : n) {
        x->addTransition(Transition(EPSILON, y->initial_state));
        alphabet.merge(y->alphabet);
        states.merge(y->states);
    }
    for(const NFA * y : n) {
        delete y;
    }

    initial_state = x;
    return;
}

void NFA::setToken(int token) {
    final_state->setToken(token);
}

void NFA::print()
{
    std::cout << "Initial state: " << initial_state->id << '\n';
    std::queue<State *> q;
    q.push(initial_state);
    std::set<int> visited;
    visited.insert(initial_state->getId());
    while (!q.empty())
    {
        State *s = q.front();
        q.pop();
        std::cout << "---> Estado: " << s->getId() << "\n";
        std::cout << "\tTransiciones:\n";
        for (Transition t : s->getTransitions())
        {
            std::cout << "\t\tid: " << t.getState()->getId() << "   "
                      << "simbolo: ";
            if (t.getLeft_simb() == 0)
                std::cout << "EPS\n";
            else
                std::cout << t.getLeft_simb() << "\n";
            if (visited.find(t.getState()->getId()) == visited.end())
            {
                q.push(t.getState());
                visited.insert(t.getState()->getId());
            }
        }
    }
    std::cout << "\n";
}

NFA::~NFA() {

}