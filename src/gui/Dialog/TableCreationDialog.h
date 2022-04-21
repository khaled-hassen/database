#pragma once

#include "gui/wxwidgets.h"
#include "Utils/types.h"

class TableCreationDialog : public wxDialog
{
private:
    static int GAP;
    std::string m_TableName;
    Columns m_Columns;
    wxTextCtrl* m_TableNameTextCtrl = nullptr;
    wxBoxSizer* m_MainSizer = nullptr;
    std::vector<std::array<wxControl*, 2>> m_DataCtrls;

public:
    TableCreationDialog(wxWindow* parent, wxWindowID id);

    [[nodiscard]] inline const std::string& GetTableName() const { return m_TableName; }

    [[nodiscard]] inline const Columns& GetColumns() const { return m_Columns; }

private:
    // add a new row (col name, data type) to the dialog
    void AddNewDataRow();

    void OnAddDataRow(wxCommandEvent& event);

    void OnCreate(wxCommandEvent& event);

DECLARE_EVENT_TABLE()
};


