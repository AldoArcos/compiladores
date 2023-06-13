#ifndef REGEX_DIALOG_H
#define REGEX_DIALOG_H
#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/stattext.h>
#include "NFA.hpp"
#include "Regex.hpp"
#include <map>
class RegexDialog : public wxDialog
{
public:
    RegexDialog(wxWindow *parent, const wxString &title, std::map<int, std::shared_ptr<NFA>> & _all_NFA);
private:
    std::map<int, std::shared_ptr<NFA>> & all_NFA;
    std::shared_ptr<LEX> l;
    std::shared_ptr<NFA> n;
    bool isFileSelected;
    wxListView * listTokens;
    wxTextCtrl * res;
    void onSaveButtonClick(wxCommandEvent& event);
    void onClearButtonClick(wxCommandEvent& event);
    void onOkButtonClick(wxCommandEvent& event);
    void onFileButtonClick(wxCommandEvent& event);
};
#endif