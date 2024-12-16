cmake_minimum_required(VERSION 3.10)
project(ChatServiceServer)
# C++ 표준 설정
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# 패키지 찾기
define_property(GLOBAL PROPERTY PKG_CONFIG_FOUND)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIBMM REQUIRED glibmm-2.4)
pkg_check_modules(GIOMM REQUIRED giomm-2.4)
# 포함 디렉토리 설정
include_directories(
    ${GLIBMM_INCLUDE_DIRS}
    ${GIOMM_INCLUDE_DIRS}
)
# 서버 소스 파일
set(SOURCES
    server.cpp
    ../../interfaces/chat_management_api/chat_management_api_stub.cpp
    ../../interfaces/msg_api/msg_api_stub.cpp
)
# 실행 파일 추가
add_executable(server ${SOURCES})
# 라이브러리 링크
target_link_libraries(server
    ${GLIBMM_LIBRARIES}
    ${GIOMM_LIBRARIES}
)
# 컴파일 옵션 설정
target_compile_options(server PRIVATE ${GLIBMM_CFLAGS_OTHER} ${GIOMM_CFLAGS_OTHER})
# 링커 옵션 설정
target_link_options(server PRIVATE ${GLIBMM_LDFLAGS_OTHER} ${GIOMM_LDFLAGS_OTHER})
