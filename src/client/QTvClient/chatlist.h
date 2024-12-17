#ifndef CHATLIST_H
#define CHATLIST_H
#include "../../../interfaces/chat_management_api/chat_management_api_proxy.h"
#include <QObject>
#include <QStringList>
using namespace App::ChatMessenger::interface;
class ChatListManager : public QObject {
    Q_OBJECT
public:
    ChatListManager(QObject *parent = nullptr);
public slots:
    void fetchChatList();                       // 서버의 GetChatList() 메서드 (비동기)
    void createChat(const QString &chatTitle);  // 서버의 CreateChat() 메서드 (비동기)
signals:
    void chatListFetched(const QStringList &chatList);  // onFetchChatListFinish() 성공시 시그널 (To. ChatList.qml)
    void createChatFailed(const QString &errorMessage); // onCreateChatFinish() 실패시 시그널 (To. ChatList.qml)
    void chatJoined(const QString &chatTitle);          // onCreateChatFinish() 성공시 시그널 (To. ChatList.qml)
private:
    Glib::RefPtr<ManagementProxy> proxy;
    std::vector<Glib::ustring> chatList;                                        // onFetchChatListFinish() 내에서, ' chatList ' 수신을 위해 선언
    void onFetchChatListFinish(const Glib::RefPtr<Gio::AsyncResult> &result);   // fetchChatList() 완료시
    void onCreateChatFinish(const Glib::RefPtr<Gio::AsyncResult> &result);      // createChat(const QString &chatTitle) 완료시
    void onChatListUpdated(std::vector<Glib::ustring> updatedChatList);         // 서버에서 송출하는 ' ChatListUpdated ' Signals 수신시
};
#endif // CHATLIST_H