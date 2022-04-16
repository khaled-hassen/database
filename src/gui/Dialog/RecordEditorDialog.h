#pragma once

#include "gui/wxwidgets.h"
#include "Utils/types.h"
#include <unordered_map>
#include <string>

enum class Mode
{
    Create, Edit
};

class RecordEditorDialog : public wxDialog
{
private:
    std::unordered_map<std::string, wxTextCtrl*> m_DataCtrl;
    Record m_Record;
    Mode m_Mode;

public:
    RecordEditorDialog(wxWindow* parent, const Columns& cols, Mode mode = Mode::Create);

    void AddDefaultValues(const Record& record);

    [[nodiscard]] inline const Record& GetRecord() const { return m_Record; }

private:
    void OnCreate(wxCommandEvent& event);

DECLARE_EVENT_TABLE()
};


