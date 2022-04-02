#pragma once

namespace ID
{
    enum ID
    {
        NEW_DB = wxID_NEW,
        OPEN_DB = wxID_OPEN,
        DROP_DB = wxID_DELETE,
        DB_LIST_VIEW = wxID_VIEW_LIST,
        NEW_TABLE = wxID_HIGHEST + 1,
        OPEN_TABLE = NEW_TABLE + 1,
        TABLE_LIST_VIEW = OPEN_TABLE + 1,
        DROP_TABLE = TABLE_LIST_VIEW + 1,
        ADD_ROW = DROP_TABLE + 1
    };
}