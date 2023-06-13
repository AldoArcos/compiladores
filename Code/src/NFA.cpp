#include "NFA.hpp"
#include "Symbols.hpp"

NFA::NFA() : initial_state(nullptr), final_state(nullptr), id(cntIdNFA++)
{
}

NFA::NFA(char s1, char s2) : id(cntIdNFA++)
{
    std::shared_ptr<NFA::State> x = std::make_shared<NFA::State>();
    std::shared_ptr<NFA::State> y = std::make_shared<NFA::State>();

    y->isAccepted = true;

    states.insert({x->id, x});
    states.insert({y->id, y});

    for (char c = s1; c <= s2; c++)
    {
        alphabet.insert(c);
    }

    x->addTransition(Transition(s1, s2, y));

    initial_state = x;
    final_state = y;
}

NFA::NFA(std::list<std::shared_ptr<NFA>> n) : id(cntIdNFA++)
{
    std::shared_ptr<NFA::State> x = std::make_shared<NFA::State>();
    for (const auto & y : n)
    {
        x->addTransition(Transition(EPSILON, y->initial_state));
        alphabet.merge(y->alphabet);
        states.merge(y->states);
    }

    initial_state = x;
    return;
}

NFA::~NFA() {
}

void NFA::Union(std::shared_ptr<NFA> n)
{
    std::shared_ptr<NFA::State> x = std::make_shared<NFA::State>();
    std::shared_ptr<NFA::State> y = std::make_shared<NFA::State>();

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
}

void NFA::Concatenation(std::shared_ptr<NFA> n)
{
    states.merge(n->states);
    alphabet.merge(n->alphabet);
    final_state->isAccepted = false;

    for (const auto &t : n->initial_state->transitions)
    {
        final_state->addTransition(t);
    }

    final_state = n->final_state;
}

void NFA::Kleene_star()
{

    std::shared_ptr<NFA::State> x = std::make_shared<NFA::State>();
    std::shared_ptr<NFA::State> y = std::make_shared<NFA::State>();

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
    std::shared_ptr<NFA::State> x = std::make_shared<NFA::State>();
    std::shared_ptr<NFA::State> y = std::make_shared<NFA::State>();

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
    std::shared_ptr<NFA::State> x = std::make_shared<NFA::State>();
    std::shared_ptr<NFA::State> y = std::make_shared<NFA::State>();

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

std::map<int, std::shared_ptr<NFA::State>> NFA::Epsilon_closure(const std::map<int, std::shared_ptr<NFA::State>> & initial_states, bool & isFinalState, int & token) {

    std::queue<std::shared_ptr<NFA::State>> q; 
    std::map<int,std::shared_ptr<NFA::State>> visited;

    // Marcamos como visitados todos los nodos a los que podemos llegar
    for(auto[id, s] : initial_states) {
        visited.insert({id, s});
        q.push(s);
    }

    while(!q.empty()) {
        auto s = q.front();
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

std::shared_ptr<DFA> NFA::getDFA()
{

    // Estados iniciales
    std::map<int, std::shared_ptr<NFA::State>> initial_states;
    // Se inserta primero el estado inicial
    initial_states.insert({initial_state->id, initial_state});

    // Bandera para saber si contiene un estado final
    bool isFinalState = false;
    int token = NO_TOKEN;

    // Estados a los que podemos llegar
    auto ans = Epsilon_closure(initial_states, isFinalState, token);

    // Lista de los estados que vayamos generando
    std::map<int, std::shared_ptr<NFA::SetStates>> st;  

    // Orden para los nuevos estados
    std::queue<std::shared_ptr<NFA::SetStates>> q;

    int cntStates = 0;
    std::shared_ptr<NFA::SetStates> initial_dfa_state = std::make_shared<NFA::SetStates>(ans, isFinalState, token);

    st.insert({cntStates++, initial_dfa_state});
    q.push(initial_dfa_state);
    

    while(!q.empty()) {

        auto s = q.front();
        q.pop();

        // Iteramos por cada simbolo del alfabeto
        for(const char symbol : alphabet) {

            token = NO_TOKEN;
            isFinalState = false;
            initial_states.clear();
            ans.clear();

            // Vemos a que estados podemos llegar con el actual simbolo y estado
            for(auto [id, x] : s->states) {

                for(NFA::Transition t : x->transitions) {

                    if(t.s == nullptr) continue;

                    if(t.isValid(symbol)) {
                        // Si existe una transicion con el simbolo
                        initial_states.insert({t.s->id, t.s});
                    }

                }

            }

            // Creamos un nuevo estado
            ans = Epsilon_closure(initial_states, isFinalState, token);

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

                    std::shared_ptr<NFA::SetStates> aux = std::make_shared<NFA::SetStates>(ans, isFinalState, token);

                    s->transitions[symbol] = cntStates;

                    st.insert({cntStates++, aux});

                    q.push(aux);

                }

            }

        }
    }
    

    std::shared_ptr<DFA> dfa = std::make_shared<DFA>(alphabet, cntStates);

    // Con todos los nuevos estados que generamos
    for(auto [id1, s] : st) {
        std::cout << id1 << "\n";
        for(auto [symbol, id2] : s->transitions) {
            std::cout << symbol << ' ' << id2 << "\n";
            dfa->setTransition(id1, symbol, id2);
        }
        dfa->setToken(id1, s->token);
    }

    return dfa;
}


void NFA::setToken(const int token) {
    final_state->token = token;
}

void NFA::print()
{
    std::cout << "Initial state: " << initial_state->id << '\n';
    std::queue<std::shared_ptr<NFA::State>> q;

    q.push(initial_state);

    std::set<int> visited;

    visited.insert(initial_state->id);
    while (!q.empty())
    {
        auto s = q.front();
        q.pop();
        std::cout << "---> Estado: " << s->id << "\n";
        std::cout << "\tTransiciones:\n";
        for (NFA::Transition t : s->transitions)
        {
            std::cout << "\t\tid: " << t.s->id << "   "
                      << "simbolo: ";
            if (t.left_simb == EPSILON)
                std::cout << "EPS\n";
            else
                std::cout << t.left_simb << "\n";
            if (visited.find(t.s->id) == visited.end())
            {
                q.push(t.s);
                visited.insert(t.s->id);
            }
        }
    }
    std::cout << "\n";
}
