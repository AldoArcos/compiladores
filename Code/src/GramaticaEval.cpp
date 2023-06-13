#include "GramaticaEval.hpp"

bool GramaticaEval::Gramatica(std::list<std::shared_ptr<ReglaGramatical>> & G,  wxTreeCtrl * tree) {
    wxTreeItemId root = tree->AddRoot("Gramatica");
    if(ConjuntoReglas(G, tree, &root)) {
        return true;
    }
    return false;
}

bool GramaticaEval::ConjuntoReglas(std::list<std::shared_ptr<ReglaGramatical>> & G,  wxTreeCtrl * tree,wxTreeItemId * parent) {
    wxTreeItemId child = tree->AppendItem(*parent, "ConjuntoReglas");
    if(Reglas(G, tree, &child)) {
        int token = lexicalAnalyzer->yylex();
        wxTreeItemId child2 = tree->AppendItem(child, "yylex");
        if(token == SEMICOLON) {
            if(ConjuntoReglasP(G, tree, &child)) {
                return true;
            }
        }
    }
    return false;
}

bool GramaticaEval::ConjuntoReglasP(std::list<std::shared_ptr<ReglaGramatical>> & G,  wxTreeCtrl * tree, wxTreeItemId * parent) {
    wxTreeItemId child = tree->AppendItem(*parent, "ConjuntoReglasP");
    if(Reglas(G, tree, &child)) {
        int token = lexicalAnalyzer->yylex();
        wxTreeItemId child2 = tree->AppendItem(child, "yylex");
        if(token == SEMICOLON) {
            if(ConjuntoReglasP(G, tree, &child)) {
                return true;
            }
        }
        return false;
    }
    lexicalAnalyzer->undoToken();
    return true;
}

bool GramaticaEval::Reglas(std::list<std::shared_ptr<ReglaGramatical>> & G,  wxTreeCtrl * tree,wxTreeItemId * parent) {
    std::string s;
    wxTreeItemId child = tree->AppendItem(*parent, "Reglas");
    if (LadoIzquierdo(s, tree, &child))
    {
        int token = lexicalAnalyzer->yylex();
        wxTreeItemId child2 = tree->AppendItem(child, "yylex");
        if (token == FLECHA)
        {
            if (LadosDerechos(s,G, tree, &child))
            {
                return true;
            }
        }
    }
    return false;
}

bool GramaticaEval::LadoIzquierdo(std::string & s,  wxTreeCtrl * tree,wxTreeItemId * parent) {
    wxTreeItemId child = tree->AppendItem(*parent, "LadoIzquierdo");
    wxTreeItemId child2 = tree->AppendItem(child, "yylex");
    int token = lexicalAnalyzer->yylex();
    if(token == SIMBOLO) {
        s = lexicalAnalyzer->getLexeme();
        return true;
    }
    return false;
}

bool GramaticaEval::LadosDerechos(std::string & s, std::list<std::shared_ptr<ReglaGramatical>> & G,  wxTreeCtrl * tree,wxTreeItemId * parent) {
    wxTreeItemId child = tree->AppendItem(parent, "LadosDerechos");
    std::shared_ptr<ReglaGramatical> r = std::make_shared<ReglaGramatical>();
    r->ladoIzquierdo = s;
    if(LadoDerecho(r, tree, &child)) {
        G.push_back(r);
        if(LadosDerechosP(s, G, tree, &child)) {
            return true;
        }
    }
    return false;
}

bool GramaticaEval::LadosDerechosP(std::string & s, std::list<std::shared_ptr<ReglaGramatical>> & G,  wxTreeCtrl * tree,wxTreeItemId * parent) {
    wxTreeItemId child = tree->AppendItem(*parent, "LadosDerechosP");
    wxTreeItemId child2 = tree->AppendItem(child, "yylex");
    int token = lexicalAnalyzer->yylex();
    if(token == OR) {
        std::shared_ptr<ReglaGramatical> r = std::make_shared<ReglaGramatical>();
        r->ladoIzquierdo = s;
        if(LadoDerecho(r, tree, &child)) {
            G.push_back(r);
            if(LadosDerechosP(s, G, tree, &child)) {
                return true;
            }
        }
        return false;
    }
    lexicalAnalyzer->undoToken();
    return true;
}

bool GramaticaEval::LadoDerecho(std::shared_ptr<ReglaGramatical> & r,  wxTreeCtrl * tree,wxTreeItemId * parent) {

    wxTreeItemId child = tree->AppendItem(*parent, "LadoDerecho");
    if(SecSimbolos(r, tree, &child)) {
        return true;
    }
    return false;
}

bool GramaticaEval::SecSimbolos(std::shared_ptr<ReglaGramatical> & r,  wxTreeCtrl * tree,wxTreeItemId * parent) {
    wxTreeItemId child = tree->AppendItem(*parent, "SecSimbolos");
    wxTreeItemId child2 = tree->AppendItem(child, "yylex");
    int token = lexicalAnalyzer->yylex();
    if(token == SIMBOLO) {
        
        std::string s = lexicalAnalyzer->getLexeme();
        r->l.push_back(ReglaGramatical::Nodo(s, false));
        if(SecSimbolosP(r, tree, &child)) {
            return true;
        }
    }
    return false;
}

bool GramaticaEval::SecSimbolosP(std::shared_ptr<ReglaGramatical> & r,  wxTreeCtrl * tree, wxTreeItemId * parent) {
    wxTreeItemId child = tree->AppendItem(*parent, "SecSimbolosP");
    wxTreeItemId child2 = tree->AppendItem(child, "yylex");
    int token = lexicalAnalyzer->yylex();
    if(token == SIMBOLO) {
        std::string s = lexicalAnalyzer->getLexeme();
        r->l.push_back(ReglaGramatical::Nodo(s, false));
        if(SecSimbolosP(r, tree, &child)) {
            return true;
        }
        return false;
    }
    lexicalAnalyzer->undoToken();
    return true;
}

bool GramaticaEval::check(wxTreeCtrl * tree) {
    std::list<std::shared_ptr<ReglaGramatical>> G;
    if(Gramatica(G, tree)) {
        return true;
    }
    return false;
}