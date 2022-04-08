#pragma once

#include "gui/wxwidgets.h"
#include "Utils/types/types.h"

class RecordsViewPanel : public wxPanel
{
private:
    class wxListView* m_RecordsList = nullptr;

    long m_SelectedRecord = -1;
private:
    void OnSelectItem(class wxListEvent& event);

public:
    RecordsViewPanel(wxWindow* parent, wxWindowID id);

    [[nodiscard]] inline long GetSelectedRecord() const { return m_SelectedRecord; }

    void ShowRecords(const Data& data, const Columns& columns);

    void ClearRecords();

DECLARE_EVENT_TABLE()
};


