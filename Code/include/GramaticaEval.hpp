#ifndef GRAMATICA_EVAL_H
#define GRAMATICA_EVAL_H
#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/treebase.h>
#include <list>
#include <memory>
#include <string>
#include "LEX.hpp"
#include "ReglaGramatical.hpp"

#define SEMICOLON 10
#define FLECHA 20
#define SIMBOLO 30
#define OR 40


class GramaticaEval {
    public:
    GramaticaEval(std::shared_ptr<LEX> l) : lexicalAnalyzer(l) {}
    bool check(wxTreeCtrl * tree);
    private:
    std::shared_ptr<LEX> lexicalAnalyzer;
    bool Gramatica(std::list<std::shared_ptr<ReglaGramatical>> & G, wxTreeCtrl * tree);
    bool ConjuntoReglas(std::list<std::shared_ptr<ReglaGramatical>> & G,  wxTreeCtrl * tree, wxTreeItemId * parent);
    bool ConjuntoReglasP(std::list<std::shared_ptr<ReglaGramatical>> & G,  wxTreeCtrl * tree,wxTreeItemId * parent);
    bool Reglas(std::list<std::shared_ptr<ReglaGramatical>> & G,  wxTreeCtrl * tree, wxTreeItemId * parent);
    bool LadoIzquierdo(std::string & s, wxTreeCtrl * tree, wxTreeItemId * parent);
    bool LadosDerechos(std::string & s, std::list<std::shared_ptr<ReglaGramatical>> & G, wxTreeCtrl * tree, wxTreeItemId * parent);
    bool LadosDerechosP(std::string & s, std::list<std::shared_ptr<ReglaGramatical>> & G, wxTreeCtrl * tree, wxTreeItemId * parent);
    bool LadoDerecho(std::shared_ptr<ReglaGramatical> & r, wxTreeCtrl * tree, wxTreeItemId * parent);
    bool SecSimbolos(std::shared_ptr<ReglaGramatical> & r, wxTreeCtrl * tree, wxTreeItemId * parent);
    bool SecSimbolosP(std::shared_ptr<ReglaGramatical> & r, wxTreeCtrl * tree, wxTreeItemId * parent);
};
#endif