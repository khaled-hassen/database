#include "DbSelectionDialog.h"
#include "Database/Database.h"
#include <wx/listctrl.h>
#include "gui/wxWindowId.h"

// statically bind events to functions
BEGIN_EVENT_TABLE(DBSelectionDialog, wxDialog)
                EVT_LIST_ITEM_SELECTED(wxWindowId::DB_LIST_VIEW, DBSelectionDialog::OnSelectItem)
                EVT_LIST_ITEM_ACTIVATED(wxWindowId::DB_LIST_VIEW, DBSelectionDialog::OnItemActivated)
END_EVENT_TABLE()


DBSelectionDialog::DBSelectionDialog(wxWindow* parent, int id, const wxString& title)
        : wxDialog(parent, id, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE,
                   wxDialogNameStr)
{
    auto* mainSizer = new wxBoxSizer(wxVERTICAL);

    auto* databasesList = new wxListView(this, wxWindowId::DB_LIST_VIEW);
    databasesList->SetSingleStyle(wxLC_SINGLE_SEL); // to prevent multi selection

    databasesList->AppendColumn("Available Databases");
    databasesList->SetColumnWidth(0, 300);
    databasesList->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    const std::vector<std::string>& items = Database::ScanForDatabases();
    for (const auto& item: items) databasesList->InsertItem(0, item);
    mainSizer->Add(databasesList);

    auto* btnSizer = CreateButtonSizer(wxOK | wxCANCEL);
    mainSizer->Add(btnSizer);

    wxDialog::SetSizerAndFit(mainSizer);
}

void DBSelectionDialog::OnSelectItem(wxListEvent& event) { m_DbName = event.GetText(); }

void DBSelectionDialog::OnItemActivated(wxListEvent& event)
{
    m_DbName = event.GetText();
    wxDialog::EndModal(wxID_OK);
}
