#include "State.hpp"
#include <set>
#include <vector>
class NFA;
class DFA {
    friend class NFA;
    private:

        std::set<char> alphabet;
        std::vector<std::vector<int>> trans;
    public:

        DFA(std::set<char> _alphabet, int totalStates) : alphabet(_alphabet) {
            trans.resize(totalStates);
            for(int i=0; i<totalStates; i++) {
                trans[i].resize(256,-1);
            }
        }

        void print() {
            for(int i=0; i<trans.size(); i++) {
                std::cout << i << ": ";
                for(int j=0; j<256; j++) {
                    if(trans[i][j] != -1) std::cout << "(" << j << ", " << trans[i][j] << ") ";
                }
                std::cout << "\n";
            }
        }
};