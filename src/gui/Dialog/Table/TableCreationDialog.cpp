#include "TableCreationDialog.h"
#include "../../../Utils/String/StringUtils.h"
#include "../../ID.h"
#include <wx/valtext.h>

BEGIN_EVENT_TABLE(TableCreationDialog, wxDialog)
                EVT_BUTTON(wxID_OK, TableCreationDialog::OnCreate)
                EVT_BUTTON(ID::ADD_ROW, TableCreationDialog::OnAddDataRow)
END_EVENT_TABLE()

int TableCreationDialog::GAP = 20;

TableCreationDialog::TableCreationDialog(wxWindow* parent, wxWindowID id)
        : wxDialog(parent, id, "Create table", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE,
                   wxDialogNameStr)
{
    mainSizer = new wxBoxSizer(wxVERTICAL);
    const wxFont FONT = wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

    auto* tableNameLabel = new wxStaticText(this, wxID_ANY, "Table name");
    tableNameLabel->SetFont(FONT);
    mainSizer->AddSpacer(GAP / 2);
    mainSizer->Add(tableNameLabel);
    mainSizer->AddSpacer(GAP / 2);

    tableNameTextCtrl = new wxTextCtrl(this, wxID_ANY);
    mainSizer->Add(tableNameTextCtrl, 0, wxEXPAND);
    mainSizer->AddSpacer(GAP);

    auto* colLabel = new wxStaticText(this, wxID_ANY, "Column names and data types");
    colLabel->SetFont(FONT);
    mainSizer->Add(colLabel);
    mainSizer->AddSpacer(GAP / 2);

    auto* addRowBtn = new wxButton(this, ID::ADD_ROW, "Add");
    mainSizer->Add(addRowBtn);
    mainSizer->AddSpacer(GAP);

    auto* btnSizer = CreateButtonSizer(wxOK | wxCANCEL);
    mainSizer->Add(btnSizer);
    mainSizer->AddSpacer(GAP / 2);

    AddNewDataRow();
    mainSizer->SetMinSize(wxSize(300, -1));
    wxDialog::SetSizerAndFit(mainSizer);
}

void TableCreationDialog::OnCreate(wxCommandEvent& event)
{
    if (tableNameTextCtrl)
    {
        tableName = StringUtils::Trim(tableNameTextCtrl->GetValue().ToStdString());
        if (tableName.empty())
        {
            wxMessageBox("Table name is required", "Error", wxICON_ERROR);
            return;
        }
    }

    columns.reserve(dataCtrls.size() + 1);
    columns.push_back("id:ID");
    for (const auto& dataCtrl: dataCtrls)
    {
        auto* textCtrl = dynamic_cast<wxTextCtrl*>(dataCtrl[0]);
        if (textCtrl == nullptr) return;

        std::string colName = StringUtils::Trim(textCtrl->GetValue().ToStdString());
        if (colName.empty())
        {
            wxMessageBox("Column name is required", "Error", wxICON_ERROR);
            return;
        }

        auto* choiceCtrl = dynamic_cast<wxChoice*>(dataCtrl[1]);
        if (choiceCtrl == nullptr) return;

        const std::string& type = choiceCtrl->GetString(choiceCtrl->GetSelection()).ToStdString();
        colName += ":" + type;
        columns.push_back(colName);
    }

    wxDialog::EndModal(wxID_OK);
}

void TableCreationDialog::AddNewDataRow()
{
    if (mainSizer == nullptr) return;

    auto* colSizer = new wxBoxSizer(wxHORIZONTAL);
    auto* colNameTextCtrl = new wxTextCtrl(this, wxID_ANY);
    colNameTextCtrl->SetValidator(wxTextValidator(wxFILTER_ALPHANUMERIC | wxFILTER_SPACE));
    colSizer->Add(colNameTextCtrl, 1);

    wxString types[3] = { "string", "int", "double" };
    auto* typeChoice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 3, types);
    typeChoice->SetSelection(0);
    colSizer->Add(typeChoice);

    size_t lastIdx = mainSizer->GetItemCount() - 1;
    mainSizer->Insert(lastIdx - 3, colSizer, 0, wxEXPAND);
    mainSizer->InsertSpacer(lastIdx - 2, GAP / 2);

    dataCtrls.push_back({ colNameTextCtrl, typeChoice });
    Fit();
}

void TableCreationDialog::OnAddDataRow(wxCommandEvent& event) { AddNewDataRow(); }

const std::string& TableCreationDialog::GetTableName() const { return tableName; }

const Columns& TableCreationDialog::GetColumns() const { return columns; }

