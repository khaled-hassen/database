#include "DbCreationDialog.h"

BEGIN_EVENT_TABLE(DBCreationDialog, wxDialog)
                EVT_BUTTON(wxID_OK, DBCreationDialog::OnCreate)
END_EVENT_TABLE()

DBCreationDialog::DBCreationDialog(wxWindow* parent, wxWindowID id)
        : wxDialog(parent, id, "Create database", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE,
                   wxDialogNameStr)
{
    auto* mainSizer = new wxBoxSizer(wxVERTICAL);
    const int GAP = 20;

    auto* label = new wxStaticText(this, wxID_ANY, "Database name");
    label->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    mainSizer->AddSpacer(GAP / 2);
    mainSizer->Add(label);
    mainSizer->AddSpacer(GAP);

    textCtrlValue = new wxTextCtrl(this, wxID_ANY);
    mainSizer->Add(textCtrlValue, 0, wxEXPAND);
    mainSizer->AddSpacer(GAP);

    auto* btnSizer = CreateButtonSizer(wxOK | wxCANCEL);
    mainSizer->Add(btnSizer);
    mainSizer->AddSpacer(GAP / 2);

    mainSizer->SetMinSize(wxSize(300, -1));
    wxDialog::SetSizer(mainSizer);
    wxDialog::Fit();
}

const std::string& DBCreationDialog::GetDbName() const { return dbName; }

void DBCreationDialog::OnCreate(wxCommandEvent& event)
{
    if (textCtrlValue != nullptr)
    {
        dbName = textCtrlValue->GetValue().ToStdString();
        if (dbName.empty())
        {
            wxMessageBox("Database name is required", "Error", wxICON_ERROR);
            return;
        }
    }

    wxDialog::EndModal(wxID_OK);
}
