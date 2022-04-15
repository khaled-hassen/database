#pragma once

#include "gui/wxwidgets.h"
#include "Utils/types.h"
#include <unordered_map>
#include <string>

class RecordCreationDialog : public wxDialog
{
private:
    std::unordered_map<std::string, wxTextCtrl*> m_DataCtrl;
    Record m_Record;

public:
    RecordCreationDialog(wxWindow* parent, wxWindowID id, const Columns& cols);

    [[nodiscard]] inline const Record& GetRecord() const { return m_Record; }

private:
    void OnCreate(wxCommandEvent& event);

DECLARE_EVENT_TABLE()
};


