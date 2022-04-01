#include "TableCreationDialog.h"
#include "../../../Utils/String/StringUtils.h"

BEGIN_EVENT_TABLE(TableCreationDialog, wxDialog)
                EVT_BUTTON(wxID_OK, TableCreationDialog::OnCreate)
END_EVENT_TABLE()

TableCreationDialog::TableCreationDialog(wxWindow* parent, wxWindowID id)
        : wxDialog(parent, id, "Create table", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE,
                   wxDialogNameStr)
{
    auto* mainSizer = new wxBoxSizer(wxVERTICAL);
    const int GAP = 20;

    auto* label = new wxStaticText(this, wxID_ANY, "Table name");
    label->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    mainSizer->AddSpacer(GAP / 2);
    mainSizer->Add(label);
    mainSizer->AddSpacer(GAP);

    textCtrl = new wxTextCtrl(this, wxID_ANY);
    mainSizer->Add(textCtrl, 0, wxEXPAND);
    mainSizer->AddSpacer(GAP);

    auto* btnSizer = CreateButtonSizer(wxOK | wxCANCEL);
    mainSizer->Add(btnSizer);
    mainSizer->AddSpacer(GAP);

    wxString types[3] = { "string", "int", "double" };
    auto* typeChoice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 3, types);
    typeChoice->SetSelection(0);
    mainSizer->Add(typeChoice);
    mainSizer->AddSpacer(GAP / 2);

    mainSizer->SetMinSize(wxSize(300, -1));
    wxDialog::SetSizer(mainSizer);
    wxDialog::Fit();
}

const std::string& TableCreationDialog::GetTableName() const { return tableName; }

void TableCreationDialog::OnCreate(wxCommandEvent& event)
{
    if (textCtrl)
    {
        tableName = StringUtils::Trim(textCtrl->GetValue().ToStdString());
        if (tableName.empty())
        {
            wxMessageBox("Table name is required", "Error", wxICON_ERROR);
            return;
        }
    }

    wxDialog::EndModal(wxID_OK);
}

