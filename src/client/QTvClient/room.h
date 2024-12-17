#ifndef ROOM_H
#define ROOM_H
#include "../../../interfaces/msg_api/msg_api_proxy.h"
#include "../../../interfaces/chat_management_api/chat_management_api_proxy.h"
#include <QObject>
using namespace App::ChatMessenger::interface;
class RoomManager : public QObject {
    Q_OBJECT
public:
    RoomManager(QObject *parent = nullptr);
public slots:
    void notifyTyping();  // 서버에게 사용자가 입력 중임을 알리는 요청 (비동기)
    void leaveChat(); // 서버에게 채팅방 퇴장 요청 (비동기)
    void sendMessage(const QString &message); // 서버에게 메시지 전송 요청 (비동기)
signals:
    void userTyping(const QString &userName); // 사용자가 입력 중임을 알리는 시그널
    void newMessageReceived(const QString &user_name, const QString &message); // 새 메시지 수신 시그널
    void userJoined(const QString &userName, const QString &chatTitle);  // 사용자가 채팅방에 입장한 시그널
    void userLeft(const QString &userName, const QString &chatTitle);    // 사용자가 채팅방을 퇴장한 시그널
private:
    Glib::RefPtr<MessageProxy> msgProxy;  // 메시지 관련 서버와 통신할 Proxy
    Glib::RefPtr<ManagementProxy> managementProxy; // 채팅 관리 서버와 통신할 Proxy
    void onNotifyTypingFinish(const Glib::RefPtr<Gio::AsyncResult> &result);  // NotifyTyping() 비동기 호출 완료 후 실행될 콜백 함수
    void onUserTyping(const Glib::ustring &userName); // 서버의 UserTyping 신호를 처리하는 슬롯
    void onLeaveChatFinish(const Glib::RefPtr<Gio::AsyncResult> &result); // LeaveChat 비동기 호출 완료 후 실행될 콜백 함수
    void onSendMessageFinish(const Glib::RefPtr<Gio::AsyncResult> &result);   // SendMessage 비동기 호출 완료 후 실행될 콜백 함수
    void onNewMsgReceived(const Glib::ustring &user_name, const Glib::ustring &message); // 서버의 NewMsgReceived 신호를 처리하는 슬롯
    void onUserJoined(const Glib::ustring &user_name, const Glib::ustring &chat_title);   // 서버의 UserJoined 신호를 처리하는 슬롯
    void onUserLeft(const Glib::ustring &user_name, const Glib::ustring &chat_title);     // 서버의 UserLeft 신호를 처리하는 슬롯
};
#endif // ROOM_H