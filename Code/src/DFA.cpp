#include "DFA.hpp"

DFA::DFA() : id(cntIdDFA++)
{
    alphabet.clear();
    trans.clear();
}

DFA::DFA(std::set<char> _alphabet, int totalStates) : alphabet(_alphabet), id(cntIdDFA++)
{
    trans.resize(totalStates);
    for (int i = 0; i < totalStates; i++)
    {
        trans[i].resize(257, -1);
    }
}

void DFA::print()
{
    for (int i = 0; i < trans.size(); i++)
    {
        std::cout << i << ": ";
        for (int j = 0; j < 256; j++)
        {
            if (trans[i][j] != -1)
                std::cout << "(" << char(j) << ", " << trans[i][j] << ") ";
        }
        std::cout << " token = " << trans[i][256];
        std::cout << "\n";
    }
}

void DFA::saveDFAToFile(std::string path)
{
    std::ofstream file(path);
    for (auto &s : trans)
    {
        for (int i = 0; i < s.size(); i++)
        {
            file << s[i];
            if (i != s.size() - 1)
                file << ' ';
        }
        file << '\n';
    }
    file.close();
}

void DFA::loadDFAFromFile(std::string path)
{
    std::ifstream file(path);
    std::string line;
    int state = 0;
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        int x;
        std::vector<int> temp(257);
        int i = 0;
        while (ss >> x)
        {
            temp[i++] = x;
        }
        trans.push_back(temp);
    }
    file.close();
}

void DFA::setTransition(int id, int symbol, int id2)
{
    trans[id][symbol] = id2;
}

int DFA::getTransition(int id, int symbol)
{
    return trans[id][symbol];
}

int DFA::getToken(int id)
{
    return trans[id][256];
}

void DFA::setToken(int id, int token)
{
    trans[id][256] = token;
}

int DFA::getId() {
    return id;
}

std::vector<std::vector<int>> DFA::getTrans() {
    return trans;
}