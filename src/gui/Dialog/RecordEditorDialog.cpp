#include "RecordEditorDialog.h"
#include <sstream>
#include <string>
#include <wx/valtext.h>
#include <wx/valnum.h>
#include "Utils/StringUtils.h"

BEGIN_EVENT_TABLE(RecordEditorDialog, wxDialog)
                EVT_BUTTON(wxID_OK, RecordEditorDialog::OnCreate)
END_EVENT_TABLE()

RecordEditorDialog::RecordEditorDialog(wxWindow* parent, const Columns& cols, Mode mode)
        : wxDialog(parent, wxID_ANY, "Insert record", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE,
                   wxDialogNameStr), m_Mode(mode)
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

void RecordEditorDialog::OnCreate(wxCommandEvent& event)
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

void RecordEditorDialog::AddDefaultValues(const Record& record)
{
    if (m_Mode == Mode::Create) return;

    for (const auto&[key, value]: record)
    {
        if (key == "id") continue;
        m_DataCtrl.at(key)->SetValue(value);
    }
}
