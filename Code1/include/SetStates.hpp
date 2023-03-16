#ifndef SET_STATES_H
#define SET_STATES_H
#include <set>
#include <array>
#include "State.hpp"
class SetStates : public State{
    private:
        std::set<int> initial_ids;
        std::set<int> all_ids;
        
    public:
        SetStates(std::set<int> _initial_ids, std::set<int> _all_ids, bool type) : initial_ids(_initial_ids), all_ids(_all_ids){
            // Crea un estado con 255 transiciones
            State(256);
            setAccepted(type);
        }

        std::set<int> & getAll_ids() {
            return all_ids;
        }
        std::set<int> & getInitial_ids() {
            return initial_ids;
        }

        bool operator == (const SetStates & s1) {
            int total = 0;
            for(auto x : initial_ids) {
                if(s1.initial_ids.find(x) != s1.initial_ids.end()) {
                    total++;
                }
                else return false;
            }
            if(s1.initial_ids.size() != total) return false;
            return true;
        }
};

#endif