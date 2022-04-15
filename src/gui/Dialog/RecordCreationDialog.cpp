#include "RecordCreationDialog.h"
#include <sstream>
#include <string>
#include <wx/valtext.h>
#include <wx/valnum.h>
#include "Utils/StringUtils.h"

BEGIN_EVENT_TABLE(RecordCreationDialog, wxDialog)
                EVT_BUTTON(wxID_OK, RecordCreationDialog::OnCreate)
END_EVENT_TABLE()


RecordCreationDialog::RecordCreationDialog(wxWindow* parent, wxWindowID id, const Columns& cols)
        : wxDialog(parent, id, "Insert record", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE,
                   wxDialogNameStr)
{
    const wxFont FONT = wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    const int GAP = 20;

    auto* mainSizer = new wxBoxSizer(wxVERTICAL);
    auto* title = new wxStaticText(this, wxID_ANY, "Insert a new record");
    title->SetFont(FONT);
    mainSizer->AddSpacer(GAP / 2);
    mainSizer->Add(title);
    mainSizer->AddSpacer(GAP);

    m_DataCtrl.reserve(cols.size());
    for (const auto& col: cols)
    {
        // skip the id column (automatically generated)
        if (col == "id:ID") continue;
        std::string name, type;
        std::stringstream stream(col);
        std::getline(stream, name, ':');
        std::getline(stream, type, ':');
        std::string displayName = name;
        StringUtils::Replace(displayName, '-', ' ');

        auto* colSizer = new wxBoxSizer(wxHORIZONTAL);
        auto* label = new wxStaticText(this, wxID_ANY, displayName);
        label->SetFont(FONT);
        colSizer->Add(label);

        auto* textCtrl = new wxTextCtrl(this, wxID_ANY);
        if (type == "int")
            textCtrl->SetValidator(wxIntegerValidator<int>());
        else if (type == "double")
            textCtrl->SetValidator(wxFloatingPointValidator<double>(wxNUM_VAL_NO_TRAILING_ZEROES));
        else if (type == "string")
            textCtrl->SetValidator(wxTextValidator(wxFILTER_ALPHANUMERIC | wxFILTER_SPACE));

        colSizer->AddSpacer(5);
        colSizer->Add(textCtrl, 1);

        mainSizer->Add(colSizer, 0, wxEXPAND);
        mainSizer->AddSpacer(GAP / 2);

        m_DataCtrl.insert({ name, textCtrl });
    }

    auto* btnSizer = CreateButtonSizer(wxOK | wxCANCEL);
    mainSizer->AddSpacer(GAP / 2);
    mainSizer->Add(btnSizer);

    wxDialog::SetSizer(mainSizer);
}

void RecordCreationDialog::OnCreate(wxCommandEvent& event)
{
    m_Record.clear();
    m_Record.reserve(m_DataCtrl.size());

    for (const auto&[name, textCtrl]: m_DataCtrl)
    {
        std::string value = StringUtils::Trim(textCtrl->GetValue().ToStdString());
        if (value.empty())
        {
            wxMessageBox(wxString::Format("%s is required", name), "Error", wxICON_ERROR);
            return;
        }
        m_Record.insert({ name, value });
    }

    wxDialog::EndModal(wxID_OK);
}