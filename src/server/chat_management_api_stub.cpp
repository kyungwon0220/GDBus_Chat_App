#include "chat_management_api_stub.h"

void ManagementStub::RegisterUser(const Glib::ustring &input_user_name, MethodInvocation &invocation) {
    // 대화명 중복 처리 ~
    for (const auto &entry : serverData.user_map) {
        const auto &existing_user_name = std::get<0>(entry.second); // 클라이언트의 대화명 ( 닉네임 )
        if (existing_user_name == input_user_name.raw()) {          // 이미 서버에 등록된 대화명일 경우
            invocation.getMessage()->return_dbus_error("App.ChatMessenger.Error", "User name already exists");
            return;
        }
    }
    // ~ 대화명 중복 처리
    
    auto bus_name = invocation.getMessage()->get_sender();                      // 클라이언트의 버스 이름을 얻음
    serverData.user_map[bus_name] = std::make_tuple(input_user_name.raw(), ""); // user_map에 등록 (대화명과, 접속중인 채팅방명 초기값)

    // 서버 Log 출력 ~
    std::cout << "========== 현재 접속중인 클라이언트 ===========" << std::endl;
    for (const auto &it : serverData.user_map) {
        std::cout << "chat_id " << it.first << "(대화명: " << std::get<0>(it.second) << " )" << std::endl;
    } std::cout << "==========" << std::endl << std::endl << std::endl;
    // ~ 서버 Log 출력

    invocation.ret();
}

void ManagementStub::GetChatList(MethodInvocation &invocation) {}

void ManagementStub::GetUserList(const Glib::ustring &chat_title, MethodInvocation &invocation) {
    std::vector<Glib::ustring> user_list;

    for (const auto &entry : serverData.user_map) {                 // user_map 검색
        const auto &user_name = std::get<0>(entry.second);          // 클라이언트 대화명
        const auto &online_chat_title = std::get<1>(entry.second);  // 클라이언트가, 현재 접속중인 채팅방명

        if (online_chat_title == chat_title.raw()) {
            user_list.push_back(user_name);
        }
    }
        
    invocation.ret(user_list);
}

void ManagementStub::CreateChat(const Glib::ustring &chat_title, MethodInvocation &invocation) {                                // 클라이언트가 생성하고자는 채팅방명 == chat_title
        // 채팅방명 중복 처리 ~
        if (std::find(serverData.chat_list.begin(), serverData.chat_list.end(), chat_title) != serverData.chat_list.end()) {    // 이미 서버에 생성된 채팅방명일 경우
            invocation.getMessage()->return_dbus_error("App.ChatMessenger.Error", "Chat title already exists");
            return;
        }
        // ~ 채팅방명 중복 처리

        serverData.chat_list.push_back(chat_title);

        std::vector<Glib::ustring> empty_destinations;          // 모든 클라이언트에게 신호 송출하기 위해, 빈 벡터를 생성
        this->ChatListUpdated_emitter(empty_destinations, {});  // 첫 인자로 빈 벡터를 전달시, 연결된 모든 클라이언트들에게 신호 송출 ( {} == 추가로 전송할 데이터 없음을 의미 )

        // 서버 Log 출력 ~
        std::cout << "새로운 채팅방 생성: " << chat_title << std::endl;
        std::cout << "========== 현존하는 채팅방 목록 ==========" << std::endl;
        for (const auto &chat : serverData.chat_list) { std::cout << chat << std::endl; }
        std::cout << "==========" << std::endl << std::endl << std::endl;
        // ~ 서버 Log 출력
    
        invocation.ret(chat_title);
}

void ManagementStub::JoinChat(const Glib::ustring &chat_title, MethodInvocation &invocation) {
    auto bus_name = invocation.getMessage()->get_sender();              // 클라이언트의 버스 이름을 얻음
    const auto &user_name = std::get<0>(serverData.user_map[bus_name]); // 본 함수를 호출한 클라이언트의 대화명 ( 닉네임 )
    
    if (serverData.user_map.find(bus_name) != serverData.user_map.end()) {
        std::get<1>(serverData.user_map[bus_name]) = chat_title.raw();  // user_map 업데이트( 클라이언트가 참여하고자는 채팅방명 )
        std::cout << "' " << user_name << " ' 클라이언트가 ' " << chat_title.raw() << " ' 채팅방에 입장." << std::endl; // 서버 Log 출력


        // 입장한 채팅방에 접속중인 클라이언트들에게, 'UserJoined' 신호 송출 ~
        std::vector<Glib::ustring> destinations;
        for (const auto &entry : serverData.user_map) {
            if (std::get<1>(entry.second) == chat_title.raw() || std::get<1>(entry.second) == "") { // 아무 채팅방에도 접속중이지 않은 클라이언트들에게도 송출 ( ChatRoom.qml 실시간 최신화 )
                destinations.push_back(Glib::ustring(entry.first));
            }
        } this->UserJoined_emitter(destinations, user_name, chat_title);
        // ~ 입장한 채팅방에 접속중인 클라이언트들에게, 'UserJoined' 신호 송출
    } else {
        invocation.getMessage()->return_dbus_error("App.ChatMessenger.Error", "User not registered");
        return;
    }

    invocation.ret();
}

void ManagementStub::LeaveChat(MethodInvocation &invocation) {
    auto bus_name = invocation.getMessage()->get_sender();                      // 클라이언트의 버스 이름을 얻음
    const auto &user_name = std::get<0>(serverData.user_map[bus_name]);         // 본 함수를 호출한 클라이언트의 대화명
    const std::string chat_title = std::get<1>(serverData.user_map[bus_name]);  // 본 함수를 호출한 클라이언트가, 접속중인 채팅방명
    std::vector<Glib::ustring> destinations;                                    // 같은 채팅방에 참여중인 클라이언트들을, 추출할 벡터 생성

    
    if (serverData.user_map.find(bus_name) != serverData.user_map.end()) {
        if (!chat_title.empty()) {
            // 같은 채팅방에 접속중인 클라이언트들 추출 ~
            for (const auto &entry : serverData.user_map) {
                if ((std::get<1>(entry.second) == chat_title && entry.first != bus_name) || std::get<1>(entry.second) == "") {  // 본 함수를 호출한 본인은, 호출 대상에서 제외 // 아무 채팅방에도 접속중이지 않은 클라이언트들에게도 송출 ( ChatRoom.qml 실시간 최신화 )
                    destinations.push_back(Glib::ustring(entry.first));
                }
            }
            // ~ 같은 채팅방에 접속중인 클라이언트들 추출
            

            std::get<1>(serverData.user_map[bus_name]) = "";                    // 클라이언트가 채팅방을 떠남 (채팅방 정보 초기화)


            // 퇴장하는 채팅방에, 접속중인 클라이언트의 존재 여부 확인 ~
            bool hasOtherClients = false;
            for (const auto &entry : serverData.user_map) {
                if (std::get<1>(entry.second) == chat_title) {
                    hasOtherClients = true;
                    break;
                }
            }
            // ~ 퇴장하는 채팅방에, 접속중인 클라이언트의 존재 여부 확인

            if (!hasOtherClients) {                                             // 접속중인 클라이언트가 없을 경우
                serverData.chat_list.erase(std::remove(serverData.chat_list.begin(), serverData.chat_list.end(), Glib::ustring(chat_title)), serverData.chat_list.end());

                // 서버 Log 출력 ~
                std::cout << "No users remaining in chat ' " << chat_title << " '. Chat room has been deleted." << std::endl;
                std::cout << "========== 현존하는 채팅방 목록 ==========" << std::endl;
                for (const auto &chat : serverData.chat_list) { std::cout << chat << std::endl; }
                std::cout << "==========" << std::endl << std::endl << std::endl;
                // ~ 서버 Log 출력

                std::vector<Glib::ustring> empty_destinations;                  // 모든 클라이언트에게 신호 송출하기 위해, 빈 벡터를 생성
                this->ChatListUpdated_emitter(empty_destinations, {});          // 첫 인자로 빈 벡터를 전달시, 연결된 모든 클라이언트들에게 신호 송출 ( {} == 추가로 전송할 데이터 없음을 의미 )
            } else {                                                            // 접속중인 클라이언트 존재시
                this->UserLeft_emitter(destinations, user_name, Glib::ustring(chat_title));

                std::cout << "' " << std::get<0>(serverData.user_map[bus_name]) << " ' 클라이언트가 ' " << chat_title << " ' 채팅방 퇴장." << std::endl;   // 서버 Log 출력
            }
        }
    } else {
        invocation.getMessage()->return_dbus_error("App.ChatMessenger.Error", "User not registered");   // 클라이언트가 등록되지 않은 경우 오류 반환
        return;
    }

    invocation.ret();
}
