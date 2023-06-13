#include "RegexDialog.hpp"

RegexDialog::RegexDialog(wxWindow *parent, const wxString &title, std::map<int, std::shared_ptr<NFA>> &_all_NFA) : wxDialog(parent, wxID_ANY, title), all_NFA(_all_NFA)
{
    wxStaticBox *staticBox = new wxStaticBox(this, wxID_ANY, wxT("&Ingrese una expresion"));
    wxStaticBoxSizer *staticSizer = new wxStaticBoxSizer(staticBox, wxHORIZONTAL);

    res = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition);
    wxButton *ok_button = new wxButton(this, wxID_ANY, "OK");
    staticSizer->Add(res, 2, wxTOP | wxLEFT | wxBOTTOM, 5);
    staticSizer->Add(ok_button, 1, wxALL, 5);

    listTokens = new wxListView(this, wxID_ANY, wxDefaultPosition, wxSize(400, 300));
    listTokens->InsertColumn(0, "Token");
    listTokens->InsertColumn(1, "Lexema");

    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(staticSizer, 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(listTokens, 0, wxEXPAND | wxALL, 5);

    wxBoxSizer *buttonsSizer = new wxBoxSizer(wxHORIZONTAL);

    wxButton *clear_button = new wxButton(this, wxID_ANY, "Clear");
    wxButton *file_button = new wxButton(this, wxID_ANY, "File");
    buttonsSizer->Add(file_button, 0, wxRIGHT, 5);
    buttonsSizer->Add(clear_button, 0, wxRIGHT, 0);
    mainSizer->Add(buttonsSizer, 0, wxALIGN_RIGHT | wxRIGHT | wxBOTTOM, 5);

    this->SetSizerAndFit(mainSizer);

    clear_button->Bind(wxEVT_BUTTON, &RegexDialog::onClearButtonClick, this);
    ok_button->Bind(wxEVT_BUTTON, &RegexDialog::onOkButtonClick, this);
    file_button->Bind(wxEVT_BUTTON, &RegexDialog::onFileButtonClick, this);
    n = nullptr;
}
void RegexDialog::onClearButtonClick(wxCommandEvent& event) {
    listTokens->DeleteAllItems();
    res->ChangeValue("");
}

void RegexDialog::onOkButtonClick(wxCommandEvent& event) {
    if(isFileSelected) {
        std::string expr = std::string(res->GetLineText(0).mb_str());
        std::cout << expr;
        l->setBuffer(expr);

        int token;
        listTokens->DeleteAllItems();
        while((token = l->yylex()) != '\0') {
            long index = listTokens->InsertItem(listTokens->GetItemCount(), wxString::Format("%d", token));
            listTokens->SetItem(index, 1, l->getLexeme());
        }
        l->setBuffer(expr);
        Regex r(l);
        if(r.check()) {
            wxMessageDialog dialog(this, "La expresion es correcta", "Evaluador Regex", wxYES_NO | wxCANCEL | wxICON_INFORMATION);
            int opt = dialog.ShowModal();
            n = r.n;
            if(opt == wxYES) {
                std::cout << n->getId() << "\n";
                //all_NFA.insert();
            }
        }
    }
}

void RegexDialog::onFileButtonClick(wxCommandEvent& event) {
    wxFileDialog openFileDialog(this, "Ubicar archivo", "", "", "", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_OK)
    {
        std::string path = std::string(openFileDialog.GetPath().mb_str());
        std::cout << path << "\n";
        isFileSelected = true;
        std::shared_ptr<DFA> dfa = std::make_shared<DFA>();
        dfa->loadDFAFromFile(path);
        l = std::make_shared<LEX>(dfa);
    }
}