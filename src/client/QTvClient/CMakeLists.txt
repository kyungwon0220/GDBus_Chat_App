cmake_minimum_required(VERSION 3.16)

project(QTvClient VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.2 REQUIRED COMPONENTS Quick)
pkg_check_modules(GLIBMM REQUIRED glibmm-2.4 giomm-2.4)


qt_add_executable(appQTvClient
    src/main.cpp
    ../../../interfaces/chat_management_api/chat_management_api_proxy.cpp
    ../../../interfaces/msg_api/msg_api_proxy.cpp
)

qt_add_qml_module(appQTvClient
    URI QTvClient
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        QML_FILES qml/ChatList.qml
        QML_FILES qml/ChatRoom.qml
        QML_FILES qml/UserList.qml
        SOURCES src/header/chat_management_api.h
        SOURCES src/implementation/chat_management_api.cpp
        SOURCES src/header/msg_management_api.h
        SOURCES src/implementation/msg_management_api.cpp
        SOURCES src/header/window_manager.hpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appQTvClient PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appQTvClient
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_include_directories(appQTvClient PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/header
    ${GLIBMM_INCLUDE_DIRS}
)
target_link_libraries(appQTvClient PRIVATE
    ${GLIBMM_LIBRARIES}
    Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appQTvClient
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
