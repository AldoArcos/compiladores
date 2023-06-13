#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/stattext.h>

#include "NFA.hpp"
#include "DFA.hpp"
#include <memory>
#include <map>

class DFADialog : public wxDialog {
    public:
    DFADialog(wxWindow * parent, const wxString &title, std::map<int, std::shared_ptr<NFA>> & _all_NFA, std::map<int, std::shared_ptr<DFA>> & _all_DFA) : wxDialog(parent, wxID_ANY, title), all_NFA(_all_NFA), all_DFA(_all_DFA) {
       wxBoxSizer * mainSizer = new wxBoxSizer(wxVERTICAL); 
       wxBoxSizer * firstSizer = new wxBoxSizer(wxHORIZONTAL);
       wxBoxSizer * buttonsSizer = new wxBoxSizer(wxVERTICAL);

       wxButton * createDFA = new wxButton(this, wxID_ANY, "Create");
       wxButton * clear = new wxButton(this, wxID_ANY, "Clear");
       wxButton * cancel = new wxButton(this, wxID_ANY, "Cancel");
       wxButton * loadDFA = new wxButton(this, wxID_ANY, "Load DFA");

       buttonsSizer->Add(loadDFA, 0, wxDOWN, 5);
       buttonsSizer->Add(createDFA, 0, wxDOWN, 5);
       buttonsSizer->Add(clear, 0, wxDOWN, 5);
       buttonsSizer->Add(cancel, 0, wxDOWN, 0);

       wxScrolledWindow * scrolledWindow = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxSize(200,300));

       checkBoxList = new wxCheckListBox(scrolledWindow, wxID_ANY);
       for(auto& [x,y] : all_NFA) {
            wxString label = wxString::Format("%d", x);
            checkBoxList->Append(label);
       }

       wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
       sizer->Add(checkBoxList, 1, wxEXPAND | wxALL, 5);
       scrolledWindow->SetSizer(sizer);
       scrolledWindow->SetScrollbars(0,1,0,300);

        firstSizer->Add(scrolledWindow, 1, wxALL | wxEXPAND, 5);
       firstSizer->Add(buttonsSizer, 0, wxEXPAND | wxALL, 5);

        trans = new wxListView(this, wxID_ANY, wxDefaultPosition, wxSize(600,300));
        trans->InsertColumn(0, "Estado", wxLIST_FORMAT_LEFT, 200);
        for(int i=0; i<257; i++) {
            if(i != 256) trans->InsertColumn(i + 1, wxString::Format("%d", i));
            else trans->InsertColumn(i + 1, wxT("Token"));
        }

       mainSizer->Add(firstSizer, 0, wxEXPAND | wxALL, 5);
       mainSizer->Add(trans, 0, wxEXPAND | wxALL, 5);
       this->SetSizerAndFit(mainSizer);

       createDFA->Bind(wxEVT_BUTTON, &DFADialog::onCreateButtonClick, this);
       loadDFA->Bind(wxEVT_BUTTON, &DFADialog::onLoadButtonClick, this);
    }

    private:
    wxCheckListBox *checkBoxList;
    wxListView * trans;
    std::map<int, std::shared_ptr<NFA>> & all_NFA;
    std::map<int, std::shared_ptr<DFA>> & all_DFA;
    void onCreateButtonClick(wxCommandEvent& event);
    void onLoadButtonClick(wxCommandEvent& event);
};

void DFADialog::onCreateButtonClick(wxCommandEvent& event) {
    wxArrayInt checkedItems;
    checkBoxList->GetCheckedItems(checkedItems);
    int total = checkedItems.GetCount();
    if(total == 0) {
        wxMessageDialog dialog(this, "Se deben seleccionar los ids", "Alerta", wxOK | wxICON_ERROR);
        dialog.ShowModal();
    }
    else {
        std::list<std::shared_ptr<NFA>> l;
        for(int i=0; i<total; i++) {
            int id = wxAtoi(checkBoxList->GetString(checkedItems[i]));
            l.push_back(all_NFA[id]);
            all_NFA.erase(id);
        }
        std::shared_ptr<NFA> n = std::make_shared<NFA>(l);
        std::shared_ptr<DFA> dfa = n->getDFA();
        wxMessageDialog dialog(this, wxString::Format("DFA creado con id: %d", dfa->getId()), "Alerta", wxOK | wxICON_INFORMATION);
        dialog.ShowModal();
        all_DFA[dfa->getId()] = dfa;
        for(int i=total-1; i>=0; i--) {
            checkBoxList->Delete(checkedItems[i]);
        }
        std::vector<std::vector<int>> table = dfa->getTrans();
        for(int i=0; i<table.size(); i++) {
            std::string temp;
            std::string token;
            long index = trans->InsertItem(trans->GetItemCount(), wxString::Format("%d",i));
            for(int j=0; j<257; j++) {
                trans->SetItem(index, j + 1, wxString::Format("%d", table[i][j]));
            }
        }
    }
}

void DFADialog::onLoadButtonClick(wxCommandEvent & event) {
    wxFileDialog openFileDialog(this, "Ubicar archivo", "", "", "", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_OK)
    {
        std::string path = std::string(openFileDialog.GetPath().mb_str());
        std::shared_ptr<DFA> dfa = std::make_shared<DFA>();
        dfa->loadDFAFromFile(path);
        dfa->print();
        all_DFA[dfa->getId()] = dfa;
        wxMessageDialog dialog(this, wxString::Format("DFA cargado, id: %d", dfa->getId()), "Alerta", wxOK | wxICON_INFORMATION);
        dialog.ShowModal();
    }
}