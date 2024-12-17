#ifndef CHAT_MANAGEMENT_API_H
#define CHAT_MANAGEMENT_API_H

#include "../../../../../interfaces/chat_management_api/chat_management_api_proxy.h"
#include <QString>
#include <QStringList>
#include <QObject>

using namespace App::ChatMessenger::interface;

class ChatManagementAPI : public QObject {
    Q_OBJECT

public:
    ChatManagementAPI(QObject *parent = nullptr);

    // 비동기 메서드 호출
    Q_INVOKABLE void registerUser(const QString &user_name);
    Q_INVOKABLE void createChat(const QString &chat_title);
    Q_INVOKABLE void getUserList(const QString &chat_title);
    Q_INVOKABLE void joinChat(const QString &chat_title);
    Q_INVOKABLE void leaveChat();

signals:
    // 비동기 호출 완료 시 시그널
    void registerUserFinished(const QStringList &chat_list);
    void registerUserFailed(const QString &error_message);

    void createChatFinished(const QString &createdChatTitle);
    void createChatFailed(const QString &error_message);

    void getUserListFinished(const QString &chat_title, const QStringList &user_list);
    // void getUserListFailed(const QString &error_message);

    void joinChatFinished(const QStringList &user_list);
    // void joinChatFailed(const QString &error_message);

    void leaveChatFinished(const QStringList &chat_list);
    // void leaveChatFailed(const QString &error_message);

    // 서버가 발행하는 시그널 수신하여 emit
    void chatListUpdated(const QStringList &chat_list);
    void userJoinedSignal(const QString &user_name, const QString &chat_title);
    void userLeftSignal(const QString &user_name, const QString &chat_title);

private:
    Glib::RefPtr<ManagementProxy> proxy;

    // 비동기 호출 완료 시 호출될 콜백 함수
    void onRegisterUserFinish(const Glib::RefPtr<Gio::AsyncResult> &result);
    void onCreateChatFinish(const Glib::RefPtr<Gio::AsyncResult> &result);
    void onGetUserListFinish(const Glib::RefPtr<Gio::AsyncResult> &result);
    void onJoinChatFinish(const Glib::RefPtr<Gio::AsyncResult> &result);
    void onLeaveChatFinish(const Glib::RefPtr<Gio::AsyncResult> &result);

    // 서버가 발행하는 시그널 수신 시
    void onChatListUpdated(const std::vector<Glib::ustring> &chat_list);
    void onUserJoined(const Glib::ustring &user_name, const Glib::ustring &chat_title);
    void onUserLeft(const Glib::ustring &user_name, const Glib::ustring &chat_title);
};

#endif // CHAT_MANAGEMENT_API_H
