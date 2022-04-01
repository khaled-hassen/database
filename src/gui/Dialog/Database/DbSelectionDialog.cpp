#include "DbSelectionDialog.h"
#include "../../../Database/Database.h"
#include <wx/msw/window.h>
#include <wx/msw/dialog.h>
#include <wx/listctrl.h>

BEGIN_EVENT_TABLE(DBSelectionDialog, wxDialog)
                EVT_LIST_ITEM_SELECTED(wxID_VIEW_LIST, DBSelectionDialog::OnSelectItem)
                EVT_LIST_ITEM_ACTIVATED(wxID_VIEW_LIST, DBSelectionDialog::OnItemSelected)
END_EVENT_TABLE()


DBSelectionDialog::DBSelectionDialog(wxWindow* parent, int id)
        : wxDialog(parent, id, "Open database", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE,
                   wxDialogNameStr)
{
    auto* mainSizer = new wxBoxSizer(wxVERTICAL);

    auto* databasesList = new wxListView(this, wxID_VIEW_LIST);
    databasesList->SetSingleStyle(wxLC_SINGLE_SEL); // to prevent multi selection

    databasesList->AppendColumn("Available Databases");
    databasesList->SetColumnWidth(0, 300);
    databasesList->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    const std::vector<std::string>& items = Database::ScanForDatabases();
    for (const auto& item: items) databasesList->InsertItem(0, item);
    mainSizer->Add(databasesList);

    auto* btnSizer = CreateButtonSizer(wxOK | wxCANCEL);
    mainSizer->Add(btnSizer);

    wxDialog::SetSizer(mainSizer);
    wxDialog::Fit();
}

const std::string& DBSelectionDialog::GetDbName() const { return dbName; }

void DBSelectionDialog::OnSelectItem(wxListEvent& event) { dbName = event.GetText(); }

void DBSelectionDialog::OnItemSelected(wxListEvent& event)
{
    dbName = event.GetText();
    wxDialog::EndModal(wxID_OK);
}
