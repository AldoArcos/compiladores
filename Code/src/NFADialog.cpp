#include "NFADialog.hpp"

NFADialog::NFADialog(wxWindow *parent, const wxString &title, std::map<int, std::shared_ptr<NFA>> &_all_NFA) : wxDialog(parent, wxID_ANY, title), all_NFA(_all_NFA)
{

    // ================ NFA basic =========================
    wxStaticBox *staticBox = new wxStaticBox(this, wxID_ANY, wxT("&NFA Basico"));
    wxStaticBoxSizer *staticSizer = new wxStaticBoxSizer(staticBox, wxVERTICAL);
    wxStaticText *staticText1 = new wxStaticText(this, wxID_ANY, wxT("Simbolo 1"));
    wxStaticText *staticText2 = new wxStaticText(this, wxID_ANY, wxT("Simbolo 2"));
    simbolo1 = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition);
    simbolo2 = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition);
    wxButton *createNFA = new wxButton(this, wxID_ANY, "Crear");

    staticSizer->Add(staticText1, 0, wxALL, 5);
    staticSizer->Add(simbolo1, 0, wxALL | wxEXPAND, 5);
    staticSizer->Add(staticText2, 0, wxALL, 5);
    staticSizer->Add(simbolo2, 0, wxALL | wxEXPAND, 5);
    staticSizer->Add(createNFA, 0, wxALL | wxEXPAND, 5);

    // ================ Ids of NFA ========================
    wxScrolledWindow *scrolledWindow = new wxScrolledWindow(this, wxID_ANY,
                                                            wxDefaultPosition, wxSize(600, 300));
    checkBoxList = new wxCheckListBox(scrolledWindow, wxID_ANY);
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

    for (auto &[x, y] : all_NFA)
    {
        wxString label = wxString::Format("%d", x);
        checkBoxList->Append(label);
    }

    sizer->Add(checkBoxList, 1, wxEXPAND | wxALL, 5);
    scrolledWindow->SetSizer(sizer);
    scrolledWindow->SetScrollbars(0, 1, 0, 300);

    // ================= Token ==============================

    wxStaticText *staticText3 = new wxStaticText(this, wxID_ANY, wxT("Token"));
    wxButton *tokenButton = new wxButton(this, wxID_ANY, "token");
    token = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition);

    // ================= Operations =========================

    wxButton *unionButton = new wxButton(this, wxID_ANY, "Union");
    wxButton *concatenationButton = new wxButton(this, wxID_ANY, "Concatenation");
    wxButton *kleenButton = new wxButton(this, wxID_ANY, "Kleen");
    wxButton *positiveButton = new wxButton(this, wxID_ANY, "Positive");
    wxButton *optionalButton = new wxButton(this, wxID_ANY, "Optional");

    wxGridSizer *gridSizer = new wxGridSizer(2, 3, 0, 0);
    gridSizer->Add(unionButton, 0, wxALL | wxEXPAND, 5);
    gridSizer->Add(concatenationButton, 0, wxALL | wxEXPAND, 5);
    gridSizer->Add(kleenButton, 0, wxALL | wxEXPAND, 5);
    gridSizer->Add(positiveButton, 0, wxALL | wxEXPAND, 5);
    gridSizer->Add(optionalButton, 0, wxALL | wxEXPAND, 5);
    gridSizer->Add(tokenButton, 0, wxALL | wxEXPAND, 5);

    // ================ Main sizer of NFADialog =============
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(staticSizer, 0, wxALL | wxEXPAND, 5);
    mainSizer->Add(staticText3, 0, wxALL | wxEXPAND, 5);
    mainSizer->Add(token, 0, wxALL | wxEXPAND, 5);
    mainSizer->Add(gridSizer, 0, wxALL | wxEXPAND, 5);
    mainSizer->Add(scrolledWindow, 1, wxALL | wxEXPAND, 5);

    this->SetSizerAndFit(mainSizer);

    // ========== Binding functions ==================
    createNFA->Bind(wxEVT_BUTTON, &NFADialog::onCreateNFAButtonClick, this);
    concatenationButton->Bind(wxEVT_BUTTON, &NFADialog::onConcatenationButtonClick, this);
    kleenButton->Bind(wxEVT_BUTTON, &NFADialog::onKleenButtonClick, this);
    optionalButton->Bind(wxEVT_BUTTON, &NFADialog::onOptionalButtonClick, this);
    positiveButton->Bind(wxEVT_BUTTON, &NFADialog::onPositiveButtonClick, this);
    unionButton->Bind(wxEVT_BUTTON, &NFADialog::onUnionButtonClick, this);
}

void NFADialog::onCreateNFAButtonClick(wxCommandEvent &event)
{
    wxString s1 = simbolo1->GetLineText(0);
    wxString s2 = simbolo2->GetLineText(0);
    char a = s1[0];
    char b = s2[0];

    if (a > b)
        std::swap(a, b);
    std::shared_ptr<NFA> n = std::make_shared<NFA>(a, b);
    all_NFA.insert({n->getId(), n});

    wxString label = wxString::Format("%d", n->getId());
    checkBoxList->Append(label);
    wxMessageDialog dialog(this, wxString::Format("NFA creado con id: %d", n->getId()), "Alerta", wxOK | wxICON_INFORMATION);
    dialog.ShowModal();

    simbolo1->ChangeValue(wxT(""));
    simbolo2->ChangeValue(wxT(""));
}

void NFADialog::onUnionButtonClick(wxCommandEvent &event)
{
    wxArrayInt checkedItems;
    checkBoxList->GetCheckedItems(checkedItems);
    int id1, id2;
    int total = checkedItems.GetCount();
    if (total != 2)
    {
        wxMessageDialog dialog(this, "Se deben seleccionar dos ids", "Alerta", wxOK | wxICON_ERROR);
        dialog.ShowModal();
    }
    else
    {
        id1 = wxAtoi(checkBoxList->GetString(checkedItems[0]));
        id2 = wxAtoi(checkBoxList->GetString(checkedItems[1]));
        std::cout << checkedItems[0] << ' ' << checkedItems[1] << "\n";
        all_NFA[id1]->Union(all_NFA[id2]);
        all_NFA.erase(id2);
        checkBoxList->Delete(checkedItems[1]);
        wxMessageDialog dialog(this, "Operacion union exitosa", "Alerta", wxOK | wxICON_INFORMATION);
        dialog.ShowModal();
        checkBoxList->Check(id1, false);
    }
}

void NFADialog::onConcatenationButtonClick(wxCommandEvent &event)
{
    wxArrayInt checkedItems;
    checkBoxList->GetCheckedItems(checkedItems);
    int id1, id2;
    int total = checkedItems.GetCount();
    if (total != 2)
    {
        wxMessageDialog dialog(this, "Se deben seleccionar dos ids", "Alerta", wxOK | wxICON_ERROR);
        dialog.ShowModal();
    }
    else
    {
        id1 = wxAtoi(checkBoxList->GetString(checkedItems[0]));
        id2 = wxAtoi(checkBoxList->GetString(checkedItems[1]));
        all_NFA[id1]->Concatenation(all_NFA[id2]);
        all_NFA.erase(id2);
        checkBoxList->Delete(checkedItems[1]);
        wxMessageDialog dialog(this, "Operacion concatenation exitosa", "Alerta", wxOK | wxICON_INFORMATION);
        dialog.ShowModal();
        checkBoxList->Check(id1, false);
    }
}

void NFADialog::onKleenButtonClick(wxCommandEvent &event)
{
    wxArrayInt checkedItems;
    checkBoxList->GetCheckedItems(checkedItems);
    int id1;
    int total = checkedItems.GetCount();
    if (total != 1)
    {
        wxMessageDialog dialog(this, "Se debe seleccionar un solo id", "Alerta", wxOK | wxICON_ERROR);
        dialog.ShowModal();
    }
    else
    {
        id1 = wxAtoi(checkBoxList->GetString(checkedItems[0]));
        all_NFA[id1]->Kleene_star();
        wxMessageDialog dialog(this, "Operacion Kleen exitosa", "Alerta", wxOK | wxICON_INFORMATION);
        dialog.ShowModal();
        checkBoxList->Check(id1, false);
    }
}

void NFADialog::onPositiveButtonClick(wxCommandEvent &event)
{
    wxArrayInt checkedItems;
    checkBoxList->GetCheckedItems(checkedItems);
    int id1;
    int total = checkedItems.GetCount();
    if (total != 1)
    {
        wxMessageDialog dialog(this, "Se debe seleccionar un solo id", "Alerta", wxOK | wxICON_ERROR);
        dialog.ShowModal();
    }
    else
    {
        id1 = wxAtoi(checkBoxList->GetString(checkedItems[0]));
        all_NFA[id1]->Positive_closure();
        wxMessageDialog dialog(this, "Operacion Positive exitosa", "Alerta", wxOK | wxICON_INFORMATION);
        dialog.ShowModal();
        checkBoxList->Check(id1, false);
    }
}

void NFADialog::onOptionalButtonClick(wxCommandEvent &event)
{
    wxArrayInt checkedItems;
    checkBoxList->GetCheckedItems(checkedItems);
    int id1;
    int total = checkedItems.GetCount();
    if (total != 1)
    {
        wxMessageDialog dialog(this, "Se debe seleccionar un solo id", "Alerta", wxOK | wxICON_ERROR);
        dialog.ShowModal();
    }
    else
    {
        id1 = wxAtoi(checkBoxList->GetString(checkedItems[0]));
        all_NFA[id1]->Optional();
        wxMessageDialog dialog(this, "Operacion Optional exitosa", "Alerta", wxOK | wxICON_INFORMATION);
        dialog.ShowModal();
        checkBoxList->Check(id1, false);
    }
}

void NFADialog::onTokenButtonClick(wxCommandEvent &event)
{
    wxArrayInt checkedItems;
    checkBoxList->GetCheckedItems(checkedItems);
    int id1;
    int t;
    int total = checkedItems.GetCount();
    if (total != 1)
    {
        wxMessageDialog dialog(this, "Se debe seleccionar un solo id", "Alerta", wxOK | wxICON_ERROR);
        dialog.ShowModal();
    }
    else
    {
        id1 = wxAtoi(checkBoxList->GetString(checkedItems[0]));
        t = wxAtoi(token->GetLineText(0));
        token->ChangeValue("");
        all_NFA[id1]->setToken(t);
        wxMessageDialog dialog(this, "Operacion token exitosa", "Alerta", wxOK | wxICON_INFORMATION);
        dialog.ShowModal();
        checkBoxList->Check(id1, false);
    }
}