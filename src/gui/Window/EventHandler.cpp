#include "EventHandler.h"
#include "Window.h"
#include "gui/Dialog/DbSelectionDialog.h"
#include "gui/Dialog/DbCreationDialog.h"
#include "gui/Dialog/TableCreationDialog.h"
#include "gui/Dialog/RecordEditorDialog.h"
#include "gui/Panel/RecordsViewPanel.h"
#include "gui/Panel/TablesSelectionPanel.h"
#include "Database/Database.h"

EventHandler::EventHandler(Window* parent) : m_Parent(parent) { }

void EventHandler::UpdateRecordsView() const
{
    CHECK_NULL(m_Parent);
    CHECK_NULL(m_Database);
    auto* panel = m_Parent->GetRecordsPanel();
    CHECK_NULL(panel);

    panel->ShowRecords(m_Database->GetTable()->GetData(), m_Database->GetTable()->GetColumns());
}

void EventHandler::UpdateTableUI(const std::string& tableName, bool update)
{
    CHECK_NULL(m_Parent);
    if (update) m_Parent->UpdateUIData();

    m_Parent->SetStatusText(wxString::Format("Active table: %s", tableName.c_str()), 1);
    UpdateRecordsView();
    m_Parent->AddRecordTools();
}

void EventHandler::OpenDatabase()
{
    CHECK_NULL(m_Parent);
    if (!m_Parent->GetTablesPanel()->CanCloseTable()) return;
    auto* panel = m_Parent->GetRecordsPanel();
    CHECK_NULL(panel);

    auto* dialog = new DBSelectionDialog(m_Parent, wxID_ANY, "Open database");
    int id = dialog->ShowModal();

    if (id == wxID_OK)
    {
        m_Database = Pointer(new Database(dialog->GetDbName()));
        m_Database->OpenDb();
        panel->ClearRecords();
        m_Parent->UpdateUIData();
    }
    dialog->Destroy();
}

void EventHandler::CreateDB()
{
    CHECK_NULL(m_Parent);
    if (!m_Parent->GetTablesPanel()->CanCloseTable()) return;
    auto* panel = m_Parent->GetRecordsPanel();
    CHECK_NULL(panel);

    auto* dialog = new DBCreationDialog(m_Parent, wxID_ANY);
    int id = dialog->ShowModal();

    if (id == wxID_OK)
    {
        m_Database = Pointer(new Database(dialog->GetDbName()));
        m_Database->CreateDb();
        panel->ClearRecords();
        m_Parent->UpdateUIData();
    }
    dialog->Destroy();
}

void EventHandler::DropDB() const
{
    CHECK_NULL(m_Parent);
    auto* panel = m_Parent->GetRecordsPanel();
    CHECK_NULL(panel);

    auto* dialog = new DBSelectionDialog(m_Parent, wxID_ANY, "Delete database");
    int id = dialog->ShowModal();

    if (id == wxID_OK)
    {
        const std::string& dbName = dialog->GetDbName();
        const auto& message = wxString::Format("Are you sure you want to delete: %s ?", dbName.c_str());
        auto* confirmDialog = new wxMessageDialog(dialog, message, "Delete database", wxYES | wxNO);
        int confirmId = confirmDialog->ShowModal();
        if (confirmId == wxID_YES) Database::DropDb(dbName);
        confirmDialog->Destroy();
        if (m_Database != nullptr && m_Database->GetName() == dbName)
        {
            m_Parent->SetStatusText("No database is opened", 0);
            panel->ClearRecords();
            m_Parent->ClearTableView();
        }
    }

    dialog->Destroy();
}

void EventHandler::OpenTable()
{
    CHECK_NULL(m_Parent);
    CHECK_NULL(m_Database);
    auto* panel = m_Parent->GetTablesPanel();
    CHECK_NULL(panel);

    const std::string& tableName = panel->GetTableName();
    m_Database->OpenTable(tableName, panel->GetTableIndex());
    UpdateTableUI(tableName, false);
}

void EventHandler::CreateTable()
{
    CHECK_NULL(m_Parent);
    if (!m_Parent->GetTablesPanel()->CanCloseTable()) return;
    CHECK_NULL(m_Database);

    auto* dialog = new TableCreationDialog(m_Parent, wxID_ANY);
    int id = dialog->ShowModal();

    if (id == wxID_OK)
    {
        m_Database->CreateTable(dialog->GetTableName(), dialog->GetColumns());
        UpdateTableUI(m_Database->GetTable()->GetName(), true);
        m_Parent->GetTablesPanel()->Select(m_Database->GetTable()->GetIndex());
    }
    dialog->Destroy();
}

void EventHandler::SaveTable() const
{
    CHECK_NULL(m_Parent);
    CHECK_NULL(m_Database);
    const Pointer<Table>& table = m_Database->GetTable();
    table->Save();
    m_Parent->GetTablesPanel()->SetTableSaveState(table->GetIndex(), table->HasUnsavedChanged());
}

void EventHandler::DropTable() const
{
    CHECK_NULL(m_Parent);
    CHECK_NULL(m_Database);
    auto* panel = m_Parent->GetRecordsPanel();
    CHECK_NULL(panel);

    const std::string& tableName = m_Database->GetTable()->GetName();
    const auto& message = wxString::Format("Are you sure you want to delete: %s ?", tableName.c_str());
    auto* confirmDialog = new wxMessageDialog(m_Parent, message, "Delete table", wxYES | wxNO);
    int confirmId = confirmDialog->ShowModal();
    if (confirmId == wxID_YES)
    {
        m_Database->DropTable();
        panel->ClearRecords();
        m_Parent->UpdateUIData();
        m_Parent->GetTablesPanel()->ResetSelectedTable();
    }
    confirmDialog->Destroy();
}

void EventHandler::AddRecord() const
{
    CHECK_NULL(m_Parent);
    CHECK_NULL(m_Database);

    auto* dialog = new RecordEditorDialog(m_Parent, m_Database->GetTable()->GetColumns());
    int id = dialog->ShowModal();

    if (id == wxID_OK)
    {
        const Pointer<Table>& table = m_Database->GetTable();
        table->InsertRecord(dialog->GetRecord());
        UpdateRecordsView();
        m_Parent->GetTablesPanel()->SetTableSaveState(table->GetIndex(), table->HasUnsavedChanged());
    }
    dialog->Destroy();
}

void EventHandler::DeleteRecord() const
{
    CHECK_NULL(m_Parent);
    CHECK_NULL(m_Database);
    auto* panel = m_Parent->GetRecordsPanel();
    CHECK_NULL(panel);

    long index = panel->GetSelectedRecord();
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
        const Pointer<Table>& table = m_Database->GetTable();
        table->DeleteRecord(index);
        panel->ResetSelectedRecord();
        UpdateRecordsView();
        m_Parent->GetTablesPanel()->SetTableSaveState(table->GetIndex(), table->HasUnsavedChanged());
    }
    confirmDialog->Destroy();
}

void EventHandler::EditRecord() const
{
    CHECK_NULL(m_Parent);
    CHECK_NULL(m_Database);
    auto* panel = m_Parent->GetRecordsPanel();
    CHECK_NULL(panel);

    long index = panel->GetSelectedRecord();
    if (index < 0)
    {
        wxMessageBox("No record is selected", "Error", wxICON_ERROR);
        return;
    }

    auto* dialog = new RecordEditorDialog(m_Parent, m_Database->GetTable()->GetColumns(), Mode::Edit);
    dialog->AddDefaultValues(m_Database->GetTable()->GetRecord(index));
    int id = dialog->ShowModal();

    if (id == wxID_OK)
    {
        const Pointer<Table>& table = m_Database->GetTable();
        table->UpdateRecord(index, dialog->GetRecord());
        UpdateRecordsView();
        m_Parent->GetTablesPanel()->SetTableSaveState(table->GetIndex(), table->HasUnsavedChanged());
    }
    dialog->Destroy();
}

