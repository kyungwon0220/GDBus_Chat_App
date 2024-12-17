#ifndef CHATROOM_H
#define CHATROOM_H
#include "../../../interfaces/chat_management_api/chat_management_api_proxy.h"
#include <QObject>
#include <QStringList>
using namespace App::ChatMessenger::interface;
class ChatRoomManager : public QObject {
    Q_OBJECT
public:
    ChatRoomManager(QObject *parent = nullptr);
public slots:
    void fetchUserList(const QString &chatTitle);   // 서버의 GetUserList() 메서드 (비동기)
    void joinChat(const QString &chatTitle);        // 서버의 JoinChat() 메서드 (비동기)
    void leaveChat();                               // 서버의 LeaveChat() 메서드 (비동기)
signals:
    void userListFetched(const QStringList &userList);  // 서버의 ' user_list ' 가져오기 완료 시그널 (To. ChatRoom.qml)
    void userJoined(const QString &userName, const QString &chatTitle);          // 서버에서 송출하는 UserJoined 신호에 연결
    void userLeft(const QString &userName, const QString &chatTitle);            // 서버에서 발행한 UserLeft 신호 수신 시그널
private:
    Glib::RefPtr<ManagementProxy> userProxy;
    void onFetchUserListFinish(const Glib::RefPtr<Gio::AsyncResult> &result);// fetchUserList() 완료시
    void onJoinChatFinish(const Glib::RefPtr<Gio::AsyncResult> &result); // joinChat() 완료시
    void onLeaveChatFinish(const Glib::RefPtr<Gio::AsyncResult> &result); // leaveChat() 완료시
    void onUserJoined(const Glib::ustring &user_name, const Glib::ustring &chatTitle); // 서버에서 송출하는 ' UserJoined ' Signals 수신시
    void onUserLeft(const Glib::ustring &user_name, const Glib::ustring &chatTitle);    // 서버에서 송출하는 ' UserLeft ' Signals 수신시
};
#endif // CHATROOM_H