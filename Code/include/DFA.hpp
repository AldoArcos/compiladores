#ifndef DFA_H
#define DFA_H
#include <set>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

class DFA
{
public:
    DFA();
    DFA(std::set<char> _alphabet, int totalStates);
    void print();
    void saveDFAToFile(std::string path);
    void loadDFAFromFile(std::string path);
    int getTransition(int id, int symbol);
    int getToken(int id);
    int getId();
    void setTransition(int id, int symbol, int id2);
    void setToken(int id, int token);
    std::vector<std::vector<int>> getTrans();

private:
    inline static int cntIdDFA = 0;
    int id;
    std::set<char> alphabet;
    std::vector<std::vector<int>> trans;
};
#endif