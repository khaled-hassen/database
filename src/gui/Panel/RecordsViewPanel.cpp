#include "RecordsViewPanel.h"
#include "gui/wxWindowId.h"
#include "Utils/String/StringUtils.h"
#include <wx/listctrl.h>
#include <string>

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

    if (m_RecordsList == nullptr) return;
    std::string::size_type size = columns.size();
    int width = static_cast<int>(GetSize().GetWidth() / size);

    for (const auto& col: columns)
    {
        std::string name = col.substr(0, col.find(':'));
        StringUtils::Replace(name, '-', ' ');
        m_RecordsList->AppendColumn(name, wxLIST_FORMAT_LEFT, width);
    }

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
    if (m_RecordsList == nullptr) return;
    m_RecordsList->ClearAll();
}