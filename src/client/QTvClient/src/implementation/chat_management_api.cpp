#include "../header/chat_management_api.h"
#include <QQmlApplicationEngine>

ChatManagementAPI::ChatManagementAPI(QObject *parent) : QObject(parent) {
    try {
        proxy = ManagementProxy::createForBus_sync(
            Gio::DBus::BusType::BUS_TYPE_SESSION,
            Gio::DBus::ProxyFlags::PROXY_FLAGS_NONE,
            "App.ChatMessenger.Server",
            "/App/ChatMessenger/UserManager");

        if (!proxy) {
            std::cerr << "Failed to create user proxy." << std::endl;
        } else {
            // 서버에서 발행하는 신호와 연결
            proxy->ChatListUpdated_signal.connect(sigc::mem_fun(*this, &ChatManagementAPI::onChatListUpdated)); // 서버에서 ChatListUpdated_emitter(empty_destinations, serverData.chat_list); 시그널 발행 시
            proxy->UserJoined_signal.connect(sigc::mem_fun(*this, &ChatManagementAPI::onUserJoined));           // 서버에서 UserJoined_emitter(destinations, user_name, chat_title); 시그널 발행 시
            proxy->UserLeft_signal.connect(sigc::mem_fun(*this, &ChatManagementAPI::onUserLeft));               // 서버에서 UserLeft_emitter(destinations, user_name, Glib::ustring(chat_title)); 시그널 발행 시
        }
    } catch (const Glib::Error &ex) {
        std::cerr << "Error occurred: " << ex.what() << std::endl;
    }
}

void ChatManagementAPI::onChatListUpdated(const std::vector<Glib::ustring> &chat_list) {    // 서버에서 ChatListUpdated_emitter(empty_destinations, serverData.chat_list); 시그널 발행 시
    QStringList qmlChatList;
    for (const auto &chat : chat_list) {
        qmlChatList.append(QString::fromStdString(chat.raw()));
    }
    emit chatListUpdated(qmlChatList);
}

void ChatManagementAPI::onUserJoined(const Glib::ustring &user_name, const Glib::ustring &chat_title) { // 서버에서 UserJoined_emitter(destinations, user_name, chat_title); 시그널 발행 시
    emit userJoinedSignal(QString::fromStdString(user_name.raw()), QString::fromStdString(chat_title.raw()));
}

void ChatManagementAPI::onUserLeft(const Glib::ustring &user_name, const Glib::ustring &chat_title) {   // 서버에서 UserLeft_emitter(destinations, user_name, Glib::ustring(chat_title)); 시그널 발행 시
    emit userLeftSignal(QString::fromStdString(user_name.raw()), QString::fromStdString(chat_title.raw()));
}




void ChatManagementAPI::registerUser(const QString &user_name) {    // 비동기적으로, 서버에게 사용자 등록 요청
    if (proxy) {    // 비동기 호출이 완료되면 호출될 콜백 등록 ( onRegisterUserFinish() )
        proxy->RegisterUser(user_name.toStdString(), sigc::mem_fun(*this, &ChatManagementAPI::onRegisterUserFinish));
    }
}
void ChatManagementAPI::onRegisterUserFinish(const Glib::RefPtr<Gio::AsyncResult> &result) {    // 서버에서 데이터를 반환하면 이 콜백이 실행
    try {
        std::vector<Glib::ustring> chatList;
        proxy->RegisterUser_finish(chatList, result);

        QStringList qmlChatList;
        for (const auto &chat : chatList) {
            qmlChatList.append(QString::fromStdString(chat.raw()));
        }
        emit registerUserFinished(qmlChatList);
    } catch (const Glib::Error &ex) {
        emit registerUserFailed(QString::fromStdString(ex.what()));
    }
}

void ChatManagementAPI::createChat(const QString &chat_title) {
    if (proxy) {    // 비동기 호출이 완료되면 호출될 콜백 등록 ( onCreateChatFinish() )
        proxy->CreateChat(chat_title.toStdString(), sigc::mem_fun(*this, &ChatManagementAPI::onCreateChatFinish));
    }
}
void ChatManagementAPI::onCreateChatFinish(const Glib::RefPtr<Gio::AsyncResult> &result) {  // 서버에서 데이터를 반환하면 이 콜백이 실행
    try {
        Glib::ustring createdChatTitle;
        proxy->CreateChat_finish(createdChatTitle, result);

        emit createChatFinished(QString::fromStdString(createdChatTitle.raw()));
    } catch (const Glib::Error &ex) {
        emit createChatFailed(QString::fromStdString(ex.what()));
    }
}

void ChatManagementAPI::getUserList(const QString &chat_title) {    // 비동기적으로, 서버에게 userList 요청
    if (proxy) {    // 비동기 호출이 완료되면 호출될 콜백 등록 ( onGetUserListFinish() )
        proxy->GetUserList(chat_title.toStdString(), sigc::mem_fun(*this, &ChatManagementAPI::onGetUserListFinish));
    }
}
void ChatManagementAPI::onGetUserListFinish(const Glib::RefPtr<Gio::AsyncResult> &result) { // 서버에서 데이터를 반환하면 이 콜백이 실행
    try {
        Glib::ustring chatTitle;
        std::vector<Glib::ustring> userList;
        proxy->GetUserList_finish(chatTitle, userList, result);

        QStringList qmlUserList;
        for (const auto &user : userList) {
            qmlUserList.append(QString::fromStdString(user.raw()));
        }
        emit getUserListFinished(QString::fromStdString(chatTitle.raw()), qmlUserList);
    } catch (const Glib::Error &ex) {
        // emit getUserListFailed(QString::fromStdString(ex.what()));
    }
}

void ChatManagementAPI::joinChat(const QString &chat_title) {
    if (proxy) {    // 비동기 호출이 완료되면 호출될 콜백 등록 ( onJoinChatFinish() )
        proxy->JoinChat(chat_title.toStdString(), sigc::mem_fun(*this, &ChatManagementAPI::onJoinChatFinish));
    }
}
void ChatManagementAPI::onJoinChatFinish(const Glib::RefPtr<Gio::AsyncResult> &result) {    // 서버에서 데이터를 반환하면 이 콜백이 실행
    try {
        std::vector<Glib::ustring> userList;
        proxy->JoinChat_finish(userList, result);

        // QStringList로 변환 ~
        QStringList qmlUserList;
        for (const auto &user : userList) {
            qmlUserList.append(QString::fromStdString(user.raw()));
        }
        // ~ QStringList로 변환

        emit joinChatFinished(qmlUserList);
    } catch (const Glib::Error &ex) {
        // emit joinChatFailed(QString::fromStdString(ex.what()));
    }
}

void ChatManagementAPI::leaveChat() {
    if (proxy) {    // 비동기 호출이 완료되면 호출될 콜백 등록 ( onLeaveChatFinish() )
        proxy->LeaveChat(sigc::mem_fun(*this, &ChatManagementAPI::onLeaveChatFinish));
    }
}
void ChatManagementAPI::onLeaveChatFinish(const Glib::RefPtr<Gio::AsyncResult> &result) {   // 서버에서 데이터를 반환하면 이 콜백이 실행
    try {
        std::vector<Glib::ustring> chatList;
        proxy->LeaveChat_finish(chatList, result);

        QStringList qmlChatList;
        for (const auto &chat : chatList) {
            qmlChatList.append(QString::fromStdString(chat.raw()));
        }
        emit leaveChatFinished(qmlChatList);
    } catch (const Glib::Error &ex) {
        // emit leaveChatFailed(QString::fromStdString(ex.what()));
    }
}
