#include "EventHandler.h"
#include "Database/Database.h"
#include <wx/frame.h>
#include <wx/msgdlg.h>
#include "gui/Dialog/DbSelectionDialog.h"
#include "gui/Dialog/DbCreationDialog.h"
#include "gui/Dialog/TableCreationDialog.h"

EventHandler::EventHandler(wxFrame* parent, Database* db) : m_Parent(parent), m_Db(db) { }

void EventHandler::OpenDatabase(const UpdateUIFn& updateUI) const
{
    auto* dialog = new DBSelectionDialog(m_Parent, wxID_ANY, "Open database");
    int id = dialog->ShowModal();

    if (id == wxID_OK)
    {
        m_Db->OpenDb(dialog->GetDbName());
        updateUI();
    }
    dialog->Destroy();
}

void EventHandler::OnCreateDB(const UpdateUIFn& updateUI) const
{
    auto* dialog = new DBCreationDialog(m_Parent, wxID_ANY);
    int id = dialog->ShowModal();

    if (id == wxID_OK)
    {
        m_Db->CreateDb(dialog->GetDbName());
        updateUI();
    }
    dialog->Destroy();
}

void EventHandler::OnDropDb() const
{
    auto* dialog = new DBSelectionDialog(m_Parent, wxID_ANY, "Delete database");
    int id = dialog->ShowModal();

    if (id == wxID_OK)
    {
        const std::string& dbName = dialog->GetDbName();
        const auto& message = wxString::Format("Are you sure you want to delete: %s ?", dbName.c_str());
        auto* confirmDialog = new wxMessageDialog(dialog, message, "Delete database", wxYES | wxNO);
        int confirmId = confirmDialog->ShowModal();
        if (confirmId == wxID_YES) m_Db->DropDb(dbName);
        confirmDialog->Destroy();
        if (!m_Db->IsDbOpen()) m_Parent->SetStatusText("No database is opened", 0);
    }

    dialog->Destroy();
}

void EventHandler::OnCreateTable(const UpdateTableUIFn& updateTableUI) const
{
    auto* dialog = new TableCreationDialog(m_Parent, wxID_ANY);
    int id = dialog->ShowModal();

    if (id == wxID_OK)
    {
        m_Db->CreateTable(dialog->GetTableName(), dialog->GetColumns());
        updateTableUI(m_Db->GetTable()->GetName(), true);
    }
    dialog->Destroy();
}

void EventHandler::OnDropTable(const UpdateUIFn& updateUI) const
{
    const std::string& tableName = m_Db->GetTable()->GetName();
    const auto& message = wxString::Format("Are you sure you want to delete: %s ?", tableName.c_str());
    auto* confirmDialog = new wxMessageDialog(m_Parent, message, "Delete table", wxYES | wxNO);
    int confirmId = confirmDialog->ShowModal();
    if (confirmId == wxID_YES) m_Db->DropTable();
    confirmDialog->Destroy();
    updateUI();
}

void EventHandler::OnDeleteRecord(long index, VoidFn callback) const
{
    if (index < 0)
    {
        wxMessageBox("No record is selected", "Error", wxICON_ERROR);
        return;
    }

    const char* message = "Are you sure you want to delete this record ?";
    auto* confirmDialog = new wxMessageDialog(m_Parent, message, "Delete record", wxYES | wxNO);
    int confirmId = confirmDialog->ShowModal();
    if (confirmId == wxID_YES) m_Db->GetTable()->DeleteRecord(index);
    confirmDialog->Destroy();
    callback();
}