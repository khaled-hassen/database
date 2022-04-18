#pragma once

#include "Utils/Pointer.h"
#include <functional>

using VoidFn = std::function<void()>;
using UpdateUIFn = VoidFn;
using UpdateTableUIFn = std::function<void(const std::string&, bool)>;

class Database;

class wxFrame;

class EventHandler
{
private:
    Database* m_Db = nullptr;
    wxFrame* m_Parent = nullptr;

public:
    EventHandler(wxFrame* parent, Database* db);

    void OpenDatabase(const UpdateUIFn& updateUI) const;

    void OnCreateDB(const UpdateUIFn& updateUI) const;

    void OnDropDb() const;

    void OnCreateTable(const UpdateTableUIFn& updateTableUI) const;

    void OnDropTable(const UpdateUIFn& updateUI) const;

    void OnAddRecord() const;

    void OnEditRecord(long index) const;

    void OnDeleteRecord(long index, const VoidFn& callback) const;
};