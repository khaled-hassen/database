#include "TablesSelectionPanel.h"
#include <wx/listctrl.h>
#include "gui/wxWindowId.h"

BEGIN_EVENT_TABLE(TablesSelectionPanel, wxPanel)
                EVT_LIST_ITEM_ACTIVATED(wxWindowId::TABLE_LIST_VIEW, TablesSelectionPanel::OnSelectTable)
END_EVENT_TABLE()

TablesSelectionPanel::TablesSelectionPanel(wxWindow* parent, wxWindowID id, const wxSize& size)
        : wxPanel(parent, id, wxDefaultPosition, size, wxTAB_TRAVERSAL, wxPanelNameStr)
{
    auto* sizer = new wxBoxSizer(wxVERTICAL);

    m_TableList = new wxListView(this, wxWindowId::TABLE_LIST_VIEW);
    m_TableList->SetSingleStyle(wxLC_SINGLE_SEL);
    m_TableList->SetBackgroundColour(wxColor("#F7F5F2"));
    sizer->Add(m_TableList, 1, wxEXPAND);

    m_TableList->AppendColumn("Available Tables");
    m_TableList->SetColumnWidth(0, size.GetWidth());
    m_TableList->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

    SetSizer(sizer);
}

void TablesSelectionPanel::ShowTablesList(const std::vector<std::string>& items)
{
    CHECK_NULL(m_TableList);
    m_TableList->DeleteAllItems(); // delete old names
    for (const auto& item: items) m_TableList->InsertItem(0, item);
}

void TablesSelectionPanel::ClearTables()
{
    CHECK_NULL(m_TableList);
    m_TableList->ClearAll();
}

void TablesSelectionPanel::OnSelectTable(wxListEvent& event)
{
    if (m_TableIndex == event.GetIndex()) return;

    bool deselected = true;
    if (m_TableIndex > -1) deselected = CanCloseTable();
    if (!deselected) return;

    CHECK_NULL(GetParent());
    m_TableIndex = event.GetIndex();
    m_TableName = event.GetText();
    wxCommandEvent ev(wxEVT_MENU, GetId());
    wxPostEvent(GetParent(), ev);
}

bool TablesSelectionPanel::CanCloseTable()
{
    if (!m_HasUnsavedChanges) return true;
    auto* dialog = new wxMessageDialog(this,
                                       "There are some unsaved changes. Leaving without saving will result in losing these changes.",
                                       "Warning", wxOK | wxCANCEL | wxICON_WARNING);
    int id = dialog->ShowModal();
    bool cancel = id == wxID_CANCEL;
    bool ok = id == wxID_OK;
    dialog->Destroy();
    if (cancel)
    {
        CHECK_NULL(m_TableList) false; // same as if (m_TableList == nullptr) return false;
        m_TableList->Select(m_TableIndex);
        return false;
    }

    if (ok) SetTableSaveState(m_TableIndex, false);
    return true;
}

void TablesSelectionPanel::SetTableSaveState(long index, bool unsaved)
{
    CHECK_NULL(m_TableList);

    m_HasUnsavedChanges = unsaved;
    const wxString& text = m_TableList->GetItemText(index);
    if (unsaved) m_TableList->SetItemText(index, text + "*");
    else m_TableList->SetItemText(index, text.substr(0, text.length() - 1));
}
