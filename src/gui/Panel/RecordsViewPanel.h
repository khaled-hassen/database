#pragma once

#include "gui/wxwidgets.h"
#include "Utils/types.h"

class wxListView;

class wxListEvent;

class RecordsViewPanel : public wxPanel
{
private:
    wxListView* m_RecordsList = nullptr;
    long m_SelectedRecord = -1;

private:
    void OnSelectItem(wxListEvent& event);

    void OnDeselectItem(wxListEvent& event);

public:
    RecordsViewPanel(wxWindow* parent, wxWindowID id);

    [[nodiscard]] inline long GetSelectedRecord() const { return m_SelectedRecord; }

    inline void ResetSelectedRecord() { m_SelectedRecord = -1; }

    void ShowRecords(const Data& data, const Columns& columns);

    void ClearRecords();

DECLARE_EVENT_TABLE()
};


