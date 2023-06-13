#include "Dialogs/LL1Dialog.hpp"
LL1Dialog::LL1Dialog(wxWindow *parent, const wxString &title, std::map<int, std::shared_ptr<DFA>> & _all_DFA) : wxDialog(parent, wxID_ANY, title), all_DFA(_all_DFA) {
    gramatica = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(600, 300), wxTE_MULTILINE);
    wxFont font(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    gramatica->SetFont(font);

    wxButton *file_button = new wxButton(this, wxID_ANY, "Cargar gramatica");
    wxButton *analizar_button = new wxButton(this, wxID_ANY, "Analizar");

    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(gramatica, 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(file_button, 0, wxALL, 5);
    mainSizer->Add(analizar_button, 0, wxALL, 5);


    tree = new wxTreeCtrl(this, wxID_ANY, wxDefaultPosition, wxSize(300, 600), wxTR_DEFAULT_STYLE);

    mainSizer->Add(tree, 0, wxALL, 5);
    this->SetSizerAndFit(mainSizer);
    file_button->Bind(wxEVT_BUTTON, &LL1Dialog::onLoadGramaticaButtonClick, this);
    analizar_button->Bind(wxEVT_BUTTON, &LL1Dialog::onAnalizerButtonClick, this);
}

void LL1Dialog::onLoadGramaticaButtonClick(wxCommandEvent& event) {
    wxFileDialog openFileDialog(this, "Ubicar archivo", "", "", "", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_OK)
    {
        std::string path = std::string(openFileDialog.GetPath().mb_str());
        wxTextFile textFile;
        if(textFile.Open(path)) {
            wxString content;
            for(int i=0; i<textFile.GetLineCount(); i++) {
                content += textFile.GetLine(i);
                if(i < textFile.GetLineCount() - 1) content += "\n";
            }
            gramatica->SetValue(content);
            textFile.Close();
        }

    }
}

void LL1Dialog::onAnalizerButtonClick(wxCommandEvent& event) {
    int total = gramatica->GetNumberOfLines();
    std::string cadena;
    for(int i=0; i<total; i++) {
        cadena += std::string(gramatica->GetLineText(i).mb_str());
    }
}