#include "chatroom.h"
ChatRoomManager::ChatRoomManager(QObject *parent) : QObject(parent) {
    try {
        userProxy = ManagementProxy::createForBus_sync(
            Gio::DBus::BusType::BUS_TYPE_SESSION,
            Gio::DBus::ProxyFlags::PROXY_FLAGS_NONE,
            "App.ChatMessenger.Server",
            "/App/ChatMessenger/UserManager");
        if (!userProxy) { std::cerr << "Failed to create user proxy." << std::endl; } else {
            userProxy->UserJoined_signal.connect(sigc::mem_fun(*this, &ChatRoomManager::onUserJoined)); // 서버에서 송출하는 UserJoined 신호에 연결
            userProxy->UserLeft_signal.connect(sigc::mem_fun(*this, &ChatRoomManager::onUserLeft));     // 서버에서 송출하는 UserLeft 신호에 연결
        }
    } catch (const Glib::Error &ex) {
        std::cerr << "Error occurred: " << ex.what() << std::endl;
    }
}
void ChatRoomManager::onUserJoined(const Glib::ustring &user_name, const Glib::ustring &chatTitle) {    // 서버에서 송출하는 ' UserJoined ' Signals 수신시
    fetchUserList(QString::fromStdString(chatTitle.raw()));
}
void ChatRoomManager::onUserLeft(const Glib::ustring &user_name, const Glib::ustring &chatTitle) {  // 서버에서 송출하는 ' UserLeft ' Signals 수신시
    fetchUserList(QString::fromStdString(chatTitle.raw()));
}
void ChatRoomManager::fetchUserList(const QString &chatTitle) { // 비동기적으로, 서버에게 userList 요청
    if (userProxy) { userProxy->GetUserList(chatTitle.toStdString(), sigc::mem_fun(*this, &ChatRoomManager::onFetchUserListFinish)); }  // 비동기 호출이 완료되면 호출될 콜백 등록 ( onFetchUserListFinish() )
}
void ChatRoomManager::onFetchUserListFinish(const Glib::RefPtr<Gio::AsyncResult> &result) { // 서버에서 데이터를 반환하면 이 콜백이 실행
    try {
        std::vector<Glib::ustring> userList;
        userProxy->GetUserList_finish(userList, result);
        QStringList qmlUserList;
        for (const auto &user : userList) {
            qmlUserList.append(QString::fromStdString(user.raw()));
        }
        emit userListFetched(qmlUserList);  // userList 수신 성공 시 시그널 방출
    } catch (const Glib::Error &ex) {
        std::cerr << "Error occurred while getting user list: " << ex.what() << std::endl;
    }
}
void ChatRoomManager::joinChat(const QString &chatTitle) { // 비동기적으로, 서버에게 'chatTitle' 채팅방으로 입장 처리 요청
    if (userProxy) { userProxy->JoinChat(chatTitle.toStdString(), sigc::mem_fun(*this, &ChatRoomManager::onJoinChatFinish)); }  // 비동기 호출이 완료되면 호출될 콜백 등록 ( onJoinChatFinish() )
}
void ChatRoomManager::onJoinChatFinish(const Glib::RefPtr<Gio::AsyncResult> &result) { // 서버에서 데이터를 반환하면 이 콜백이 실행
    try {
        userProxy->JoinChat_finish(result);
    } catch (const Glib::Error &ex) {
        std::cerr << "Error occurred while joining chat: " << ex.what() << std::endl;
    }
}
void ChatRoomManager::leaveChat() { // 비동기적으로, 서버에게 채팅방 퇴장 요청
    if (userProxy) {  userProxy->LeaveChat(sigc::mem_fun(*this, &ChatRoomManager::onLeaveChatFinish)); }    // 비동기 호출이 완료되면 호출될 콜백 등록 ( onLeaveChatFinish() )
}
void ChatRoomManager::onLeaveChatFinish(const Glib::RefPtr<Gio::AsyncResult> &result) { // 서버에서 데이터를 반환하면 이 콜백이 실행
    try {
        userProxy->LeaveChat_finish(result);
    } catch (const Glib::Error &ex) {
        std::cerr << "Error occurred while leaving chat: " << ex.what() << std::endl;
    }
}
#include "chatroom.moc"