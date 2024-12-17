#include "room.h"
RoomManager::RoomManager(QObject *parent) : QObject(parent) {
    try {
        // msg_api Proxy 생성
        msgProxy = MessageProxy::createForBus_sync(
            Gio::DBus::BusType::BUS_TYPE_SESSION,
            Gio::DBus::ProxyFlags::PROXY_FLAGS_NONE,
            "App.ChatMessenger.Server",
            "/App/ChatMessenger/MessageManager");
        if (!msgProxy) { std::cerr << "Failed to create message proxy." << std::endl; } else {
            msgProxy->UserTyping_signal.connect(sigc::mem_fun(*this, &RoomManager::onUserTyping)); // 서버에서 송출하는 UserTyping 신호에 연결
            msgProxy->NewMsgReceived_signal.connect(sigc::mem_fun(*this, &RoomManager::onNewMsgReceived));
        }
        // chat_management_api Proxy 생성
        managementProxy = ManagementProxy::createForBus_sync(
            Gio::DBus::BusType::BUS_TYPE_SESSION,
            Gio::DBus::ProxyFlags::PROXY_FLAGS_NONE,
            "App.ChatMessenger.Server",
            "/App/ChatMessenger/UserManager");
        if (!managementProxy) { std::cerr << "Failed to create management proxy." << std::endl; } else {
            // UserJoined와 UserLeft 신호를 서버에서 연결
            managementProxy->UserJoined_signal.connect(sigc::mem_fun(*this, &RoomManager::onUserJoined));
            managementProxy->UserLeft_signal.connect(sigc::mem_fun(*this, &RoomManager::onUserLeft));
        }
    } catch (const Glib::Error &ex) {
        std::cerr << "Error occurred: " << ex.what() << std::endl;
    }
}
void RoomManager::onUserTyping(const Glib::ustring &userName) {
    QString qUserName = QString::fromStdString(userName.raw());
    emit userTyping(qUserName);
}
// 메시지 수신 신호를 처리하는 슬롯
void RoomManager::onNewMsgReceived(const Glib::ustring &user_name, const Glib::ustring &message) {
    emit newMessageReceived(QString::fromStdString(user_name.raw()), QString::fromStdString(message.raw()));
}
// UserJoined 신호를 처리하는 슬롯 추가
void RoomManager::onUserJoined(const Glib::ustring &user_name, const Glib::ustring &chat_title) {
    QString qUserName = QString::fromStdString(user_name.raw());
    QString qChatTitle = QString::fromStdString(chat_title.raw());
    emit userJoined(qUserName, qChatTitle);
}
// UserLeft 신호를 처리하는 슬롯 추가
void RoomManager::onUserLeft(const Glib::ustring &user_name, const Glib::ustring &chat_title) {
    QString qUserName = QString::fromStdString(user_name.raw());
    QString qChatTitle = QString::fromStdString(chat_title.raw());
    emit userLeft(qUserName, qChatTitle);
}
void RoomManager::notifyTyping() {
    if (msgProxy) { msgProxy->NotifyTyping(sigc::mem_fun(*this, &RoomManager::onNotifyTypingFinish)); } // 비동기 호출이 완료되면 호출될 콜백 등록 ( onNotifyTypingFinish() )
}
void RoomManager::onNotifyTypingFinish(const Glib::RefPtr<Gio::AsyncResult> &result) {                  // 서버에서 데이터를 반환하면 이 콜백이 실행
    try {
        msgProxy->NotifyTyping_finish(result);
    } catch (const Glib::Error &ex) {
        std::cerr << "Error occurred while notifying typing: " << ex.what() << std::endl;
    }
}
void RoomManager::sendMessage(const QString &message) {
    if (msgProxy) {
        msgProxy->SendMsg(message.toStdString(), sigc::mem_fun(*this, &RoomManager::onSendMessageFinish)); // 비동기 호출이 완료되면 호출될 콜백 등록 ( onSendMessageFinish() )
    }
}
void RoomManager::onSendMessageFinish(const Glib::RefPtr<Gio::AsyncResult> &result) { // 서버에서 데이터를 반환하면 이 콜백이 실행
    try {
        msgProxy->SendMsg_finish(result);
    } catch (const Glib::Error &ex) {
        std::cerr << "Error occurred while sending message: " << ex.what() << std::endl;
    }
}
void RoomManager::leaveChat() { // 비동기적으로, 서버에게 채팅방 퇴장 요청
    if (managementProxy) { managementProxy->LeaveChat(sigc::mem_fun(*this, &RoomManager::onLeaveChatFinish)); }
}
void RoomManager::onLeaveChatFinish(const Glib::RefPtr<Gio::AsyncResult> &result) { // 서버에서 데이터를 반환하면 이 콜백이 실행
    try {
        managementProxy->LeaveChat_finish(result);
        std::cout << "Successfully left the chat room." << std::endl;
    } catch (const Glib::Error &ex) {
        std::cerr << "Error occurred while leaving chat: " << ex.what() << std::endl;
    }
}