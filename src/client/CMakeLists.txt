# Add the following CMake configuration to build client
cmake_minimum_required(VERSION 3.10)
project(ChatServiceClient)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIBMM REQUIRED glibmm-2.4 giomm-2.4)
# Add source files for client
set(SOURCES client.cpp ../../interfaces/msg_api/msg_api_stub.cpp ../../interfaces/msg_api/msg_api_proxy.cpp)
add_executable(client ${SOURCES})
target_include_directories(client PRIVATE ${GLIBMM_INCLUDE_DIRS})
target_link_libraries(client PRIVATE ${GLIBMM_LIBRARIES})
target_compile_options(client PRIVATE ${GLIBMM_CFLAGS_OTHER})
