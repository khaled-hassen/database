#include "TableSelectionPanel.h"
#include <wx/listctrl.h>
#include "gui/wxWindowId.h"

BEGIN_EVENT_TABLE(TableSelectionPanel, wxPanel)
                EVT_LIST_ITEM_ACTIVATED(wxWindowId::TABLE_LIST_VIEW, TableSelectionPanel::OnSelectTable)
END_EVENT_TABLE()

TableSelectionPanel::TableSelectionPanel(wxWindow* parent, wxWindowID id, const wxSize& size)
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

void TableSelectionPanel::ShowTablesList(const std::vector<std::string>& items)
{
    CHECK_NULL(m_TableList);
    m_TableList->DeleteAllItems(); // delete old names
    for (const auto& item: items) m_TableList->InsertItem(0, item);
}

void TableSelectionPanel::OnSelectTable(wxListEvent& event)
{
    CHECK_NULL(GetParent());

    m_TableName = event.GetText();
    wxCommandEvent ev(wxEVT_MENU, GetId());
    wxPostEvent(GetParent(), ev);
}
