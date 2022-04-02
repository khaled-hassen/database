#pragma once

#include "../../wxwidgets.h"
#include "../../../Utils/types/types.h"
#include <string>
#include <vector>
#include <array>

class TableCreationDialog : public wxDialog
{
private:
    static int GAP;
    std::string tableName;
    Columns columns;
    wxTextCtrl* tableNameTextCtrl = nullptr;
    wxBoxSizer* mainSizer = nullptr;
    std::vector<std::array<wxControl*, 2>> dataCtrls;

public:
    TableCreationDialog(wxWindow* parent, wxWindowID id);

    [[nodiscard]] const std::string& GetTableName() const;

    [[nodiscard]] const Columns& GetColumns() const;

private:
    // add a new row (col name, data type) to the dialog
    void AddNewDataRow();

    void OnAddDataRow(wxCommandEvent& event);

    void OnCreate(wxCommandEvent& event);

DECLARE_EVENT_TABLE()
};


