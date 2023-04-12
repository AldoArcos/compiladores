#ifndef DFA_H
#define DFA_H
#include "State.hpp"
#include <set>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
class NFA;
class Lex;
class DFA {
    friend class NFA;
    friend class Lex;
    private:
        std::set<char> alphabet;
        std::vector<std::vector<int>> trans;
    public:

        DFA() {
            alphabet.clear();
            trans.clear();
        }

        DFA(std::set<char> _alphabet, int totalStates) : alphabet(_alphabet) {
            trans.resize(totalStates);
            for(int i=0; i<totalStates; i++) {
                trans[i].resize(257,-1);
            }
        }

        void print() {
            for(int i=0; i<trans.size(); i++) {
                std::cout << i << ": ";
                for(int j=0; j<256; j++) {
                    if(trans[i][j] != -1) std::cout << "(" << char(j) << ", " << trans[i][j] << ") ";
                }
                std::cout << " token = " << trans[i][256];
                std::cout << "\n";
            }
        }

        void saveDFAToFile(std::string path) {
            std::ofstream file(path);
            for(auto& s : trans) {
                for(int i=0; i<s.size(); i++) {
                   file << s[i];
                   if(i != s.size() - 1) file << ' ';
                }
                file << '\n';
            }
            file.close();
        }

        void loadDFAFromFile(std::string path) {
            std::ifstream file(path);
            std::string line;
            int state = 0;
            while(std::getline(file, line)) {
                std::stringstream ss(line);
                int x;
                std::vector<int> temp(257);
                int i = 0;
                while(ss >> x) {
                    temp[i++] = x;
                }
                trans.push_back(temp);
            }
            file.close();
        }
};
#endif