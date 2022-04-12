#pragma once

#include "Utils/Pointer/Pointer.h"
#include <functional>

using UpdateUIFn = std::function<void()>;
using UpdateTableUIFn = std::function<void(const std::string&, bool)>;

class EventHandler
{
private:
    class Database* m_Db = nullptr;

    class wxFrame* m_Parent = nullptr;

public:
    EventHandler(wxFrame* parent, Database* db);

    void OpenDatabase(const UpdateUIFn& updateUI) const;

    void OnCreateDB(const UpdateUIFn& updateUI) const;

    void OnDropDb() const;

    void OnCreateTable(const UpdateTableUIFn& updateTableUI) const;

    void OnDropTable(const UpdateUIFn& updateUI) const;

    void OnDeleteRecord(long index) const;
};