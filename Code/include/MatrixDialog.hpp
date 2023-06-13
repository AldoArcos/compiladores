#ifndef MATRIX_DIALOG_H
#define MATRIX_DIALOG_H
#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/stattext.h>
#include "MatrixEval.hpp"
class MatrixDialog : public wxDialog
{
public:
    MatrixDialog(wxWindow *parent, const wxString &title, std::map<std::string, Matrix> & _variables);

private:
    wxListView * listTokens;
    wxListView * listVariables;
    wxTextCtrl * res;
    wxTextCtrl * exp;
    std::shared_ptr<LEX> l;
    Matrix m;
    bool isFileSelected;
    bool isCorrect;
    std::string variableName;
    std::map<std::string, Matrix> & variables;

    void onOkButtonClick(wxCommandEvent& event);
    void onClearButtonClick(wxCommandEvent& event);
    void onSaveButtonClick(wxCommandEvent& event);
    void onFileButtonClick(wxCommandEvent& event);
};
#endif