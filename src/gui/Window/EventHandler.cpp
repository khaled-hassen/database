#include "EventHandler.h"
#include "Database/Database.h"
#include <wx/frame.h>
#include <wx/msgdlg.h>
#include "gui/Dialog/DbSelectionDialog.h"
#include "gui/Dialog/DbCreationDialog.h"
#include "gui/Dialog/TableCreationDialog.h"
#include "gui/Dialog/RecordEditorDialog.h"
#include "gui/Panel/RecordsViewPanel.h"
#include "gui/Panel/TablesSelectionPanel.h"

EventHandler::EventHandler(wxFrame* parent, Database* db, RecordsViewPanel* recordsPanel)
        : m_Parent(parent), m_Db(db), m_RecordsPanel(recordsPanel) { }

void EventHandler::UpdateRecordsView() const
{
    CHECK_NULL(m_RecordsPanel);
    m_RecordsPanel->ShowRecords(m_Db->GetTable()->GetData(), m_Db->GetTable()->GetColumns());
}

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

void EventHandler::CreateDB(const UpdateUIFn& updateUI) const
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

void EventHandler::DropDB() const
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

void EventHandler::OpenTable(TablesSelectionPanel* tablesPanel, const UpdateTableUIFn& updateTableUI) const
{
    CHECK_NULL(tablesPanel);
    const std::string& tableName = tablesPanel->GetTableName();
    m_Db->OpenTable(tableName);
    updateTableUI(tableName, false);
}

void EventHandler::CreateTable(const UpdateTableUIFn& updateTableUI) const
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

void EventHandler::SaveTable() const { m_Db->GetTable()->Save(); }

void EventHandler::DropTable(const UpdateUIFn& updateUI) const
{
    CHECK_NULL(m_RecordsPanel);
    const std::string& tableName = m_Db->GetTable()->GetName();
    const auto& message = wxString::Format("Are you sure you want to delete: %s ?", tableName.c_str());
    auto* confirmDialog = new wxMessageDialog(m_Parent, message, "Delete table", wxYES | wxNO);
    int confirmId = confirmDialog->ShowModal();
    if (confirmId == wxID_YES)
    {
        m_Db->DropTable();
        m_RecordsPanel->ClearRecords();
        updateUI();
    }
    confirmDialog->Destroy();
}

void EventHandler::AddRecord() const
{
    CHECK_NULL(m_RecordsPanel);

    auto* dialog = new RecordEditorDialog(m_Parent, m_Db->GetTable()->GetColumns());
    int id = dialog->ShowModal();

    if (id == wxID_OK)
    {
        m_Db->GetTable()->InsertRecord(dialog->GetRecord());
        UpdateRecordsView();
    }
    dialog->Destroy();
}

void EventHandler::DeleteRecord() const
{
    CHECK_NULL(m_RecordsPanel);
    long index = m_RecordsPanel->GetSelectedRecord();
    if (index < 0)
    {
        wxMessageBox("No record is selected", "Error", wxICON_ERROR);
        return;
    }

    const char* message = "Are you sure you want to delete this record ?";
    auto* confirmDialog = new wxMessageDialog(m_Parent, message, "Delete record", wxYES | wxNO);
    int confirmId = confirmDialog->ShowModal();
    if (confirmId == wxID_YES)
    {
        m_Db->GetTable()->DeleteRecord(index);
        m_RecordsPanel->ResetSelectedRecord();
        UpdateRecordsView();
    }
    confirmDialog->Destroy();
}

void EventHandler::EditRecord() const
{
    CHECK_NULL(m_RecordsPanel);
    long index = m_RecordsPanel->GetSelectedRecord();
    if (index < 0)
    {
        wxMessageBox("No record is selected", "Error", wxICON_ERROR);
        return;
    }

    auto* dialog = new RecordEditorDialog(m_Parent, m_Db->GetTable()->GetColumns(), Mode::Edit);
    dialog->AddDefaultValues(m_Db->GetTable()->GetRecord(index));
    int id = dialog->ShowModal();

    if (id == wxID_OK)
    {
        m_Db->GetTable()->UpdateRecord(index, dialog->GetRecord());
        UpdateRecordsView();
    }
    dialog->Destroy();
}

