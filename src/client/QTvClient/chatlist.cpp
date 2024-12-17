#include "chatlist.h"
ChatListManager::ChatListManager(QObject *parent) : QObject(parent) {
    try {
        // chat_management_api Proxy 생성 ~
        proxy = ManagementProxy::createForBus_sync(
            Gio::DBus::BusType::BUS_TYPE_SESSION,
            Gio::DBus::ProxyFlags::PROXY_FLAGS_NONE,
            "App.ChatMessenger.Server",
            "/App/ChatMessenger/UserManager");
        if (!proxy) { std::cerr << "Failed to create user proxy." << std::endl; } else {
            proxy->ChatListUpdated_signal.connect( sigc::mem_fun(*this, &ChatListManager::onChatListUpdated));  // 서버에서 송출하는 ' ChatListUpdated ' Signals 연결
        }
        // ~ chat_management_api Proxy 생성
    } catch (const Glib::Error &ex) {
        std::cerr << "Error occurred: " << ex.what() << std::endl;
    }
}
void ChatListManager::onChatListUpdated(std::vector<Glib::ustring> updatedChatList) {   // 서버에서 송출하는 ' ChatListUpdated ' Signals 수신시
    fetchChatList(); // 채팅방 목록이 업데이트되면, 서버로부터 최신화된 채팅방 목록을 요청
}
void ChatListManager::fetchChatList() { // 비동기적으로, 서버에게 chatList 요청
    if (proxy) { proxy->GetChatList(sigc::mem_fun(*this, &ChatListManager::onFetchChatListFinish)); }   // 비동기 호출이 완료되면 호출될 콜백 등록 ( onFetchChatListFinish() )
}
void ChatListManager::onFetchChatListFinish(const Glib::RefPtr<Gio::AsyncResult> &result) {             // 서버에서 데이터를 반환하면 이 콜백이 실행
    try {
        proxy->GetChatList_finish(chatList, result);
        QStringList qmlChatList; for (const auto &chat : chatList) { qmlChatList.append(QString::fromStdString(chat.raw())); }
        emit chatListFetched(qmlChatList);                                                              // chatList 수신 성공 시 시그널 방출
    } catch (const Glib::Error &ex) {
        std::cerr << "Error occurred while getting chat list: " << ex.what() << std::endl;
    }
}
void ChatListManager::createChat(const QString &chat_title) {
    if (proxy) { proxy->CreateChat(chat_title.toStdString(), sigc::mem_fun(*this, &ChatListManager::onCreateChatFinish)); } // 비동기 호출이 완료되면 호출될 콜백 등록 ( onCreateChatFinish() )
}
void ChatListManager::onCreateChatFinish(const Glib::RefPtr<Gio::AsyncResult> &result) {    // 서버에서 데이터를 반환하면 이 콜백이 실행
    try {
        Glib::ustring createdChatTitle;
        proxy->CreateChat_finish(createdChatTitle, result);
        fetchChatList();                                                    // 새로운 채팅방 생성 완료되면, 서버로부터 최신화된 채팅방 목록을 요청
        emit chatJoined(QString::fromStdString(createdChatTitle.raw()));    // 생성된 채팅방에 자동 입장을 위한 시그널 방출
    } catch (const Glib::Error &ex) {
        emit createChatFailed(QString::fromStdString(ex.what()));           // 새로운 채팅방 생성 실패시 시그널 방출
    }
}
#include "chatlist.moc"