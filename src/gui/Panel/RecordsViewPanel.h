#pragma once

#include "gui/wxwidgets.h"
#include "Utils/types/types.h"

class RecordsViewPanel : public wxPanel
{
private:
    class wxListView* m_RecordsList = nullptr;

public:
    RecordsViewPanel(wxWindow* parent, wxWindowID id);

    void ShowRecords(const Data& data, const Columns& columns);
};


