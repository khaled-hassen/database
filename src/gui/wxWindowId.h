#pragma once

namespace wxWindowId
{
    enum wxWindowId
    {
        NEW_DB = wxID_NEW,
        OPEN_DB = wxID_OPEN,
        DROP_DB = wxID_DELETE,
        DB_LIST_VIEW = wxID_VIEW_LIST,
        NEW_TABLE = wxID_HIGHEST + 1,
        OPEN_TABLE = NEW_TABLE + 1,
        TABLE_LIST_VIEW = OPEN_TABLE + 1,
        DROP_TABLE = TABLE_LIST_VIEW + 1,
        RECORDS_VIEW = DROP_TABLE + 1,
        ADD_ROW = RECORDS_VIEW + 1,
        ADD_RECORD = ADD_ROW + 1,
        DELETE_RECORD = ADD_RECORD + 1,
        EDIT_RECORD = DELETE_RECORD + 1,
        SAVE_TABLE = EDIT_RECORD + 1
    };
}