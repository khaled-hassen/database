#include "TableCreationDialog.h"
#include "Utils/StringUtils.h"
#include "gui/wxWindowId.h"
#include <array>

BEGIN_EVENT_TABLE(TableCreationDialog, wxDialog)
                EVT_BUTTON(wxID_OK, TableCreationDialog::OnCreate)
                EVT_BUTTON(wxWindowId::ADD_ROW, TableCreationDialog::OnAddDataRow)
END_EVENT_TABLE()

int TableCreationDialog::GAP = 20;

TableCreationDialog::TableCreationDialog(wxWindow* parent, wxWindowID id)
        : wxDialog(parent, id, "Create table", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE,
                   wxDialogNameStr)
{
    m_MainSizer = new wxBoxSizer(wxVERTICAL);
    const wxFont FONT = wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

    auto* tableNameLabel = new wxStaticText(this, wxID_ANY, "Table name");
    tableNameLabel->SetFont(FONT);
    m_MainSizer->AddSpacer(GAP / 2);
    m_MainSizer->Add(tableNameLabel);
    m_MainSizer->AddSpacer(GAP / 2);

    m_TableNameTextCtrl = new wxTextCtrl(this, wxID_ANY);
    m_MainSizer->Add(m_TableNameTextCtrl, 0, wxEXPAND);
    m_MainSizer->AddSpacer(GAP);

    auto* colLabel = new wxStaticText(this, wxID_ANY, "Column names and data types");
    colLabel->SetFont(FONT);
    m_MainSizer->Add(colLabel);
    m_MainSizer->AddSpacer(GAP / 2);

    auto* addRowBtn = new wxButton(this, wxWindowId::ADD_ROW, "Add");
    m_MainSizer->Add(addRowBtn);
    m_MainSizer->AddSpacer(GAP);

    auto* btnSizer = CreateButtonSizer(wxOK | wxCANCEL);
    m_MainSizer->Add(btnSizer);
    m_MainSizer->AddSpacer(GAP / 2);

    AddNewDataRow();
    m_MainSizer->SetMinSize(wxSize(300, -1));
    wxDialog::SetSizerAndFit(m_MainSizer);
}

void TableCreationDialog::OnCreate(wxCommandEvent& event)
{
    if (m_TableNameTextCtrl)
    {
        m_TableName = StringUtils::Trim(m_TableNameTextCtrl->GetValue().ToStdString());
        if (m_TableName.empty())
        {
            wxMessageBox("Table name is required", "Error", wxICON_ERROR);
            return;
        }
    }

    m_Columns.reserve(m_DataCtrls.size() + 1);
    m_Columns["id"] = "ID";
    for (const auto& dataCtrl: m_DataCtrls)
    {
        auto* textCtrl = dynamic_cast<wxTextCtrl*>(dataCtrl[0]);
        CHECK_NULL(textCtrl);

        std::string colName = StringUtils::Trim(textCtrl->GetValue().ToStdString());
        if (colName.empty())
        {
            wxMessageBox("Column name is required", "Error", wxICON_ERROR);
            return;
        }

        auto* choiceCtrl = dynamic_cast<wxChoice*>(dataCtrl[1]);
        CHECK_NULL(choiceCtrl);

        const std::string& type = choiceCtrl->GetString(choiceCtrl->GetSelection()).ToStdString();
        // removing the spaces because it messes up the input order in unordered_map
        m_Columns[StringUtils::Replace(colName, ' ', '-')] = type;
    }

    wxDialog::EndModal(wxID_OK);
}

void TableCreationDialog::AddNewDataRow()
{
    CHECK_NULL(m_MainSizer);

    auto* colSizer = new wxBoxSizer(wxHORIZONTAL);
    auto* colNameTextCtrl = new wxTextCtrl(this, wxID_ANY);
    colNameTextCtrl->SetValidator(wxTextValidator(wxFILTER_ALPHANUMERIC | wxFILTER_SPACE));
    colSizer->Add(colNameTextCtrl, 1);

    wxString types[3] = { "string", "int", "double" };
    auto* typeChoice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 3, types);
    typeChoice->SetSelection(0);
    colSizer->Add(typeChoice);

    size_t lastIdx = m_MainSizer->GetItemCount() - 1;
    m_MainSizer->Insert(lastIdx - 3, colSizer, 0, wxEXPAND);
    m_MainSizer->InsertSpacer(lastIdx - 2, GAP / 2);

    m_DataCtrls.push_back({ colNameTextCtrl, typeChoice });
    Fit();
}

void TableCreationDialog::OnAddDataRow(wxCommandEvent& event) { AddNewDataRow(); }
