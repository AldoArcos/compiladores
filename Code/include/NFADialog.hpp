#ifndef NFA_DIALOG_H
#define NFA_DIALOG_H

// wxWidgets libraries
#include <wx/wx.h>
#include <wx/listctrl.h>

#include <memory>
#include <map>

#include "NFA.hpp"

class NFADialog : public wxDialog
{
public:

    // Constructor
    NFADialog(wxWindow *parent, const wxString &title, std::map<int, std::shared_ptr<NFA>> &_all_NFA);
private:
    std::map<int, std::shared_ptr<NFA>> & all_NFA;
    wxCheckListBox *checkBoxList;
    wxTextCtrl *simbolo1;
    wxTextCtrl *simbolo2;
    wxTextCtrl *token;

    void onCreateNFAButtonClick(wxCommandEvent &event);
    void onUnionButtonClick(wxCommandEvent &event);
    void onConcatenationButtonClick(wxCommandEvent &event);
    void onKleenButtonClick(wxCommandEvent &event);
    void onPositiveButtonClick(wxCommandEvent &event);
    void onOptionalButtonClick(wxCommandEvent &event);
    void onTokenButtonClick(wxCommandEvent &event);
};
#endif