#ifndef MATRIX_EVAL_H
#define MATRIX_EVAL_H
#include "LEX.hpp"
#include <memory>
#include <vector>
#include <string>
#include <map>
struct Matrix {
    int r,c;
    std::vector<std::vector<float>> m;

    Matrix(int rows, int columns) : m(rows), r(rows), c(columns) {
        for(auto& x : m) x.resize(c,(float)0.0);
    }

    Matrix() {
        r = -1;
        c = 0;
    }

    bool appendRow() {
        if(r == 0) {
            c = (int) m[0].size();
        }
        else if(r != -1 && c != (int) m[r].size()) return false;
        r++;


        m.push_back(std::vector<float>());
        return true;
    }

    bool appendNumber(float x) {
        if(r != 0 && (int) m[r].size() == c) return false;
        m[r].push_back(x);
        return true;
    }

    std::string print() {
        std::string ans;
        ans += "[";
        for(auto& x:m) {
            ans += "[";
            for(int i=0; i<x.size(); i++) {
                ans += std::to_string(x[i]);
                if(i != x.size() - 1) ans += ", ";
            }
            ans += "]";
        }
        ans += "]";
        return ans;
    }

    Matrix operator + (const Matrix& m2) {
        /*
        if(m2.r != r && m2.c != c) {
            return;
        }
        */

       std::cout << r << ' ' << c << "\n";
       r = (int) m.size();
       c = (int) m[0].size();
        Matrix temp(r, c);
        for(int i=0; i<r; i++) {
            for(int j=0; j<c; j++) {
                temp.m[i][j] = m[i][j] + m2.m[i][j];
            }
        }
        return temp;
    }

    Matrix operator - (const Matrix& m2) {
        /*
        if(m2.r != r && m2.c != c) {
            return;
        }
        */
       std::cout << r << ' ' << c << "\n";
       r = (int) m.size();
       c = (int) m[0].size();
        Matrix temp(r, c);
        for(int i=0; i<r; i++) {
            for(int j=0; j<c; j++) {
                temp.m[i][j] = m[i][j] - m2.m[i][j];
            }
        }
        return temp;
    }

    Matrix operator * (const Matrix& m2) {
        /*
        if(m2.r != c && m2.c != r) {
            return;
        }
        */
       int rr = (int) m.size();
       int cc = (int) m2.m[0].size();
        Matrix temp(rr, cc);

        for(int i=0; i<rr; i++) {
            for(int j=0; j<cc; j++) {
                for(int k=0; k<m[0].size(); k++) {
                    temp.m[i][j] += m[i][k] * m2.m[k][j];
                }
            }
        }
        return temp;
    }

    Matrix operator / (const Matrix& m2) {
        /*
        if(m2.r != c && m2.c != r) {
            return;
        }
        */
        Matrix temp(r, c);

        for(int i=0; i<r; i++) {
            for(int j=0; j<c; j++) {
                for(int k=0; k<r; k++) {
                    temp.m[i][j] = temp.m[i][j] + (m[i][k] / m2.m[k][i]);
                }
            }
        }

        return temp;
    }

    void operator=(const Matrix & m2) {
        r = m2.r;
        c = m2.c;
        m = m2.m;
    }

};

class MatrixEval {
    public:

    MatrixEval(std::shared_ptr<LEX> l, std::map<std::string, Matrix> & _variables) : lexicalAnalyzer(l), variables(_variables) {}
    bool check();
    Matrix m;

    private:
    std::shared_ptr<LEX> lexicalAnalyzer;
    std::map<std::string, Matrix> & variables;
    bool Asign(Matrix & m);
    bool E(Matrix & m);
    bool Ep(Matrix & m);
    bool T(Matrix & m);
    bool Tp(Matrix & m);
    bool F(Matrix & m);
    bool Matriz(Matrix & m);
    bool Renglones(Matrix & m);
    bool Renglonesp(Matrix & m);
    bool Renglon(Matrix & m);
    bool ListaNumeros(Matrix & m);
    bool ListaNumerosp(Matrix & m);

};
#endif