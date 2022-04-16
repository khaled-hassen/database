cmake_minimum_required(VERSION 3.21)
project(database)

set(CMAKE_CXX_STANDARD 23)

add_compile_definitions(wxMSVC_VERSION_ABI_COMPAT WXUSINGDLL UNICODE _UNICODE)
include_directories($ENV{WX_WID}/include/msvc $ENV{WX_WID}/include ${PROJECT_SOURCE_DIR}/src)
link_directories($ENV{WX_WID}/lib/vc14x_x64_dll)
add_executable(${PROJECT_NAME} WIN32 main.cpp src/CSVFile/CSVFile.cpp src/CSVFile/CSVFile.h src/CLI/CLI.cpp src/CLI/CLI.h src/Database/Database.cpp src/Database/Database.h src/Table/Table.cpp src/Table/Table.h src/Utils/Pointer.h src/Utils/Id.h src/Utils/StringUtils.cpp src/Utils/StringUtils.h src/Utils/types.h src/gui/App/App.cpp src/gui/App/App.h src/gui/Window/Window.cpp src/gui/Window/Window.h src/gui/wxwidgets.h src/gui/wxWindowId.h src/gui/Dialog/DbSelectionDialog.cpp src/gui/Dialog/DbSelectionDialog.h src/gui/Dialog/DbCreationDialog.cpp src/gui/Dialog/DbCreationDialog.h src/gui/Panel/TableSelectionPanel.cpp src/gui/Panel/TableSelectionPanel.h src/gui/Panel/RecordsViewPanel.cpp src/gui/Panel/RecordsViewPanel.h src/gui/Dialog/TableCreationDialog.cpp src/gui/Dialog/TableCreationDialog.h src/gui/Event/EventHandler.cpp src/gui/Event/EventHandler.h src/gui/Dialog/RecordEditorDialog.cpp src/gui/Dialog/RecordEditorDialog.h)

