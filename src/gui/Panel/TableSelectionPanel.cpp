#include "TableSelectionPanel.h"
#include <wx/listctrl.h>
#include "../ID.h"

TableSelectionPanel::TableSelectionPanel(wxWindow* parent, wxWindowID id, const wxSize& size)
        : wxPanel(parent, id, wxDefaultPosition, size, wxTAB_TRAVERSAL, wxPanelNameStr)
{
    auto color = wxColor("#F7F5F2");
    wxPanel::SetBackgroundColour(color);
    auto* sizer = new wxBoxSizer(wxVERTICAL);

    tableList = new wxListView(this, ID::TABLE_LIST_VIEW);
    tableList->SetSingleStyle(wxLC_SINGLE_SEL);
    tableList->SetBackgroundColour(color);
    sizer->Add(tableList, 1, wxEXPAND);

    tableList->AppendColumn("Available Tables");
    tableList->SetColumnWidth(0, size.GetWidth());
    tableList->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

    SetSizer(sizer);
}

void TableSelectionPanel::ShowTablesList(const std::vector<std::string>& items)
{
    if (tableList == nullptr) return;
    for (const auto& item: items) tableList->InsertItem(0, item);
}
