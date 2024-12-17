#ifndef MANAGEMENT_STUB_H
#define MANAGEMENT_STUB_H
#include "msg_api_stub.h"

void MessageStub::NotifyTyping(MethodInvocation &invocation) {
    auto bus_name = invocation.getMessage()->get_sender();                                  // 클라이언트의 버스 이름을 얻음

    if (serverData.user_map.find(bus_name) != serverData.user_map.end()) {
        const auto &user_name = std::get<0>(serverData.user_map[bus_name]);                 // 본 함수를 호출한 클라이언트의 대화명
        const std::string &chat_title = std::get<1>(serverData.user_map[bus_name]);         // 본 함수를 호출한 클라이언트가, 현재 접속중인 채팅방명
        
        if (!chat_title.empty()) {
            std::vector<Glib::ustring> destinations;
            for (const auto &entry : serverData.user_map) {
                if (entry.first != bus_name && std::get<1>(entry.second) == chat_title) {   // 본 함수를 호출한 클라이언트와, 같은 채팅방에 접속중인 경우 ( 본인 제외 )
                    destinations.push_back(Glib::ustring(entry.first));
                }
            } this->UserTyping_emitter(destinations, Glib::ustring(user_name));             // 본 함수를 호출한 클라이언트의 대화명을 신호로 보냄
        }
    } else {
        invocation.getMessage()->return_dbus_error("App.ChatMessenger.Error", "User not registered");
    }

    invocation.ret();
}

void MessageStub::SendMsg(const Glib::ustring &message, MethodInvocation &invocation) { // 전송하고자는 메시지 == message
    auto bus_name = invocation.getMessage()->get_sender();                      // 클라이언트의 버스 이름을 얻음
    const auto &user_name = std::get<0>(serverData.user_map[bus_name]);         // 클라이언트 대화명
    const std::string &chat_title = std::get<1>(serverData.user_map[bus_name]); // 본 함수를 호출한 클라이언트가, 현재 접속중인 채팅방명

    std::vector<Glib::ustring> destinations;
    for (const auto &entry : serverData.user_map) {
        if (std::get<1>(entry.second) == chat_title) {                          // 본 함수를 호출한 클라이언트와, 같은 채팅방에 접속중인 경우 ( 본인 포함 )
            destinations.push_back(Glib::ustring(entry.first));
        }
    } this->NewMsgReceived_emitter(destinations, user_name, message);           // 본 함수를 호출한 클라이언트 대화명과, 메시지 내용을 신호로 보냄
    
    invocation.ret();
}
#endif