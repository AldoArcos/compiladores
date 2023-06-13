#include <wx/wx.h>
#include "MatrixDialog.hpp"
#include "NFADialog.hpp"
#include "RegexDialog.hpp"
#include "Dialogs/LL1Dialog.hpp"
#include "DFADialog.hpp"

#include "NFA.hpp"
class MyApp : public wxApp 
{
    public:
        bool OnInit() override;

    private:
        std::map<int, std::shared_ptr<NFA>> all_NFA;
        std::map<int, std::shared_ptr<DFA>> all_DFA;
        std::map<std::string, Matrix> variables;
};

wxIMPLEMENT_APP(MyApp);


// Declare our main frame Class
class MyFrame : public wxFrame {
    public:
    //Constructor
        MyFrame(const wxString& title, std::map<int, std::shared_ptr<NFA>>& _all_NFA, std::map<std::string, Matrix> & _variables, std::map<int, std::shared_ptr<DFA>> & _all_DFA);

    private:
    //Event handlers
        std::map<int, std::shared_ptr<DFA>> & all_DFA;
        std::map<int, std::shared_ptr<NFA>> & all_NFA;
        std::map<std::string, Matrix> & variables; 

        void OnOptionsNFA(wxCommandEvent& event);
        void OnCreateDFA(wxCommandEvent& event);
        void OnSaveDFA(wxCommandEvent& event);

        void OnLexicalAnalyzer(wxCommandEvent& event);

        void OnSyntacticAnalyzer(wxCommandEvent& event);
        void OnRegex(wxCommandEvent& event);
        void OnMatrix(wxCommandEvent& event);
};

enum {
    ID_NFA = 1,
    ID_CREATE_DFA,
    ID_SAVE_DFA,
    ID_LEXICAL,
    ID_SYNTACTICAL,
    ID_REGEX,
    ID_MATRIX

};

bool MyApp::OnInit() {
    // Create the main application window
    MyFrame *frame = new MyFrame(wxT("Compiladores"), all_NFA, variables, all_DFA);
    frame->Show(true);
    return true;
}


MyFrame::MyFrame(const wxString& title, std::map<int, std::shared_ptr<NFA>> & _all_NFA, std::map<std::string, Matrix> & _variables, std::map<int, std::shared_ptr<DFA>> & _all_DFA) : 
wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(600,600)),  
all_NFA(_all_NFA), variables(_variables), all_DFA(_all_DFA)
{
    // Create a menu bar
    wxMenu * automatonOptions = new wxMenu;
    automatonOptions->Append(ID_NFA, "&NFA", "Todas las opciones para la creacion de un NFA");
    automatonOptions->AppendSeparator();
    automatonOptions->Append(ID_CREATE_DFA, "&Create DFA", "Genera un DFA con NFA");
    automatonOptions->AppendSeparator();
    automatonOptions->Append(ID_SAVE_DFA, "&Save DFA", "Guarda un DFA");
    automatonOptions->AppendSeparator();

    wxMenu * lexicalOptions = new wxMenu;
    lexicalOptions->Append(ID_LEXICAL, "&Analyze string", "Analiza una cadena y te genera los tokens");
    lexicalOptions->AppendSeparator();

    wxMenu * syntacticalOptions = new wxMenu;
    syntacticalOptions->Append(ID_SYNTACTICAL, "&Syntactical", "Analiza una cadena sintacticamente");
    syntacticalOptions->AppendSeparator();
    syntacticalOptions->Append(ID_REGEX, "&Regex", "Se introduce una expresion regular y te genera los NFA");
    syntacticalOptions->AppendSeparator();
    syntacticalOptions->Append(ID_MATRIX, "&Matrix calculator", "Calculadora de matrices");

    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(automatonOptions, "&Automatons");
    menuBar->Append(lexicalOptions, "&Lexical Analyzer");
    menuBar->Append(syntacticalOptions, "&Syntactic Analyzer");

    SetMenuBar(menuBar);

    CreateStatusBar();
    SetStatusText(wxT("Bienvenido 1.0.0"));

    Bind(wxEVT_MENU, &MyFrame::OnOptionsNFA, this, ID_NFA);
    Bind(wxEVT_MENU, &MyFrame::OnCreateDFA, this, ID_CREATE_DFA);
    Bind(wxEVT_MENU, &MyFrame::OnSaveDFA, this, ID_SAVE_DFA);

    Bind(wxEVT_MENU, &MyFrame::OnLexicalAnalyzer, this, ID_LEXICAL);

    Bind(wxEVT_MENU, &MyFrame::OnSyntacticAnalyzer, this, ID_SYNTACTICAL);
    Bind(wxEVT_MENU, &MyFrame::OnRegex, this, ID_REGEX);
    Bind(wxEVT_MENU, &MyFrame::OnMatrix, this, ID_MATRIX);
}


void MyFrame::OnOptionsNFA(wxCommandEvent& event) {
    NFADialog * dialog = new NFADialog(this, "Creacion de un NFA", all_NFA);
    dialog->ShowModal();
}

void MyFrame::OnCreateDFA(wxCommandEvent& event) {
    DFADialog * dialog = new DFADialog(this, "Creacion de un DFA", all_NFA, all_DFA);
    dialog->ShowModal();
}

void MyFrame::OnSaveDFA(wxCommandEvent& event) {

    wxLogMessage("Hello world from wxWidgets!");
}

void MyFrame::OnLexicalAnalyzer(wxCommandEvent& event) {

    wxLogMessage("Hello world from wxWidgets!");
}

void MyFrame::OnSyntacticAnalyzer(wxCommandEvent &event) {
    LL1Dialog * dialog = new LL1Dialog(this, "LL1", all_DFA); 
    dialog->ShowModal();
}

void MyFrame::OnRegex(wxCommandEvent &event) {
    RegexDialog * dialog = new RegexDialog(this, "Regex", all_NFA);
    dialog->ShowModal();
}


void MyFrame::OnMatrix(wxCommandEvent &event) {
    MatrixDialog * dialog = new MatrixDialog(this, "Calculadora de Matrices", variables);
    dialog->ShowModal();
}