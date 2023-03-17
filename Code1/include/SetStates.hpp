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
        std::map<int, State*> states;
        std::map<int, int> transitions;
        bool accepted;
        
    public:
        SetStates(std::map<int, State*> _states, bool _flag) : states(_states), accepted(_flag) {}

        void print() {
            std::cout << "Estado conjunto = {";
            for(auto [x, s] : states) std::cout << x << ' ';
            std::cout << "}\n";
        }

};

#endif