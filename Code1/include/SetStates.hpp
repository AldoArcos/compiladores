#ifndef SET_STATES_H
#define SET_STATES_H
#include "State.hpp"
#include <map>
#include <iostream>
#include <array>

class NFA;

class SetStates {

    friend class NFA;
    private:
        // Estados del NFA
        std::map<int, State*> states;
        // Simbol y numero del SetState
        std::map<int, int> transitions;
        // Es final
        bool isAccepted;

        int token;
        
    public:
        SetStates(std::map<int, State*> _states, bool _flag, int _token) : states(_states), isAccepted(_flag), token(_token) {}

        void print() {
            std::cout << "Estado conjunto = {";
            for(auto [x, s] : states) std::cout << x << ' ';
            std::cout << "}\n";
        }

};

#endif