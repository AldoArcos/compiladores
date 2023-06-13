#include "MatrixDialog.hpp"

MatrixDialog::MatrixDialog(wxWindow *parent, const wxString &title, std::map<std::string, Matrix> &_variables)
    : wxDialog(parent, wxID_ANY, title), variables(_variables)
{

    // ================= Expression =========================
    wxStaticBox *staticBox = new wxStaticBox(this, wxID_ANY, wxT("&Ingrese una expresion"));
    wxStaticBoxSizer *stacticSizer = new wxStaticBoxSizer(staticBox, wxHORIZONTAL);
    wxButton *ok_button = new wxButton(this, wxID_ANY, "OK");

    exp = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition);

    stacticSizer->Add(exp, 2, wxTOP | wxLEFT | wxBOTTOM, 5);
    stacticSizer->Add(ok_button, 1, wxALL, 5);

    //===========  Resultado =================
    wxStaticText *staticText = new wxStaticText(this, wxID_ANY, wxT("Resultado"));
    res = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition);

    // ========== Tokens =========================
    listTokens = new wxListView(this, wxID_ANY, wxDefaultPosition, wxSize(400, 300));
    listTokens->InsertColumn(0, "Token");
    listTokens->InsertColumn(1, "Lexema");

    // ========== Variables ====================
    listVariables = new wxListView(this, wxID_ANY, wxDefaultPosition, wxSize(400, 300));
    listVariables->InsertColumn(0, "Identificador", wxLIST_FORMAT_LEFT, 200);
    listVariables->InsertColumn(1, "Valor");
    int id = 0;
    for (auto [x, y] : variables)
    {
        long index = listVariables->InsertItem(listVariables->GetItemCount(), x);
        listVariables->SetItem(index, 1, y.print());
    }

    wxBoxSizer *middleSizer = new wxBoxSizer(wxHORIZONTAL);
    middleSizer->Add(listTokens, 0, wxEXPAND | wxALL, 5);
    middleSizer->Add(listVariables, 0, wxEXPAND | wxALL, 5);

    // ============= Buttons ======================
    wxButton *clear_button = new wxButton(this, wxID_ANY, "Clear");
    wxButton *file_button = new wxButton(this, wxID_ANY, "Load File");
    wxButton *save_button = new wxButton(this, wxID_ANY, "Save");
    wxBoxSizer *buttonsSizer = new wxBoxSizer(wxHORIZONTAL);

    buttonsSizer->Add(file_button, 0, wxRIGHT, 5);
    buttonsSizer->Add(clear_button, 0, wxRIGHT, 5);
    buttonsSizer->Add(save_button, 0, wxRIGHT, 0);

    // =============== Main sizer of MatrixDialog ===============
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(stacticSizer, 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(staticText, 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(res, 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(middleSizer, 1, wxEXPAND | wxALL, 5);
    mainSizer->Add(buttonsSizer, 0, wxALIGN_RIGHT | wxRIGHT | wxBOTTOM, 5);

    this->SetSizerAndFit(mainSizer);

    // =============== Binding functions =======================
    file_button->Bind(wxEVT_BUTTON, &MatrixDialog::onFileButtonClick, this);
    ok_button->Bind(wxEVT_BUTTON, &MatrixDialog::onOkButtonClick, this);
    clear_button->Bind(wxEVT_BUTTON, &MatrixDialog::onClearButtonClick, this);
    save_button->Bind(wxEVT_BUTTON, &MatrixDialog::onSaveButtonClick, this);

    isFileSelected = false;
}

void MatrixDialog::onOkButtonClick(wxCommandEvent &event)
{
    if (isFileSelected)
    {
        std::string expr = std::string(exp->GetLineText(0).mb_str());
        l->setBuffer(expr);
        int token;
        listTokens->DeleteAllItems();
        bool name = false;
        while ((token = l->yylex()) != '\0')
        {
            long index = listTokens->InsertItem(listTokens->GetItemCount(), wxString::Format("%d", token));
            listTokens->SetItem(index, 1, l->getLexeme());
            if (token == 120 && !name)
            {
                variableName = l->getLexeme();
                name = true;
            }
        }

        l->setBuffer(expr);
        MatrixEval r(l, variables);
        if (r.check())
        {
            isCorrect = true;
            res->ChangeValue(r.m.print());
            m = r.m;
        }
    }
    else
    {
        isCorrect = false;
        wxMessageDialog dialog(this, "Seleccionar la ruta del DFA", "Alerta", wxOK | wxICON_ERROR);
        dialog.ShowModal();
    }
}

void MatrixDialog::onClearButtonClick(wxCommandEvent &event)
{
    listTokens->DeleteAllItems();
    exp->ChangeValue("");
    res->ChangeValue("");
}

void MatrixDialog::onFileButtonClick(wxCommandEvent &event)
{
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

void MatrixDialog::onSaveButtonClick(wxCommandEvent &event)
{
    long index = listVariables->InsertItem(listVariables->GetItemCount(), variableName);
    listVariables->SetItem(index, 1, res->GetLineText(0));
    variables.insert({variableName, m});
}