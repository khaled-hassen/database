#include "RecordsViewPanel.h"
#include "gui/wxWindowId.h"
#include "Utils/StringUtils.h"
#include <wx/listctrl.h>

BEGIN_EVENT_TABLE(RecordsViewPanel, wxPanel)
                EVT_LIST_ITEM_SELECTED(wxWindowId::RECORDS_VIEW, RecordsViewPanel::OnSelectItem)
                EVT_LIST_ITEM_DESELECTED(wxWindowId::RECORDS_VIEW, RecordsViewPanel::OnDeselectItem)
                EVT_LIST_ITEM_ACTIVATED(wxWindowId::RECORDS_VIEW, RecordsViewPanel::OnActiveItem)
END_EVENT_TABLE()

RecordsViewPanel::RecordsViewPanel(wxWindow* parent, wxWindowID id)
        : wxPanel(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxPanelNameStr)
{
    auto* sizer = new wxBoxSizer(wxVERTICAL);
    m_RecordsList = new wxListView(this, wxWindowId::RECORDS_VIEW);

    m_RecordsList->SetSingleStyle(wxLC_SINGLE_SEL);
    m_RecordsList->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

    sizer->Add(m_RecordsList, 1, wxEXPAND);
    SetSizer(sizer);
}

void RecordsViewPanel::ShowRecords(const Data& data, const Columns& columns)
{
    ClearRecords();

    CHECK_NULL(m_RecordsList);
    std::string::size_type size = columns.size();
    int width = static_cast<int>(GetSize().GetWidth() / size);

    for (const auto&[name, _]: columns)
        m_RecordsList->AppendColumn(StringUtils::Replace(name, '-', ' '), wxLIST_FORMAT_LEFT, width);

    int index = 0;
    for (const auto& record: data)
    {
        int subIndex = 0;
        for (const auto&[_, value]: record)
        {
            if (subIndex == 0)
            {
                m_RecordsList->InsertItem(index, value);
                ++subIndex;
            }
            else m_RecordsList->SetItem(index, subIndex++, value);
        }
        ++index;
    }
}

void RecordsViewPanel::ClearRecords()
{
    CHECK_NULL(m_RecordsList);
    m_RecordsList->ClearAll();
}

void RecordsViewPanel::OnSelectItem(wxListEvent& event) { m_SelectedRecord = event.GetIndex(); }

void RecordsViewPanel::OnDeselectItem(wxListEvent& event) { ResetSelectedRecord(); }

void RecordsViewPanel::OnActiveItem(wxListEvent& event)
{
    CHECK_NULL(GetParent());

    m_SelectedRecord = event.GetIndex();
    wxCommandEvent ev(wxEVT_MENU, GetId());
    wxPostEvent(GetParent(), ev);
}
