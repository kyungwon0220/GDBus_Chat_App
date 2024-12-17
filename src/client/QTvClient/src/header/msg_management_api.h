#ifndef MSG_MANAGEMENT_API_H
#define MSG_MANAGEMENT_API_H
#include "../../../../../interfaces/msg_api/msg_api_proxy.h"
#include <QString>
#include <QObject>
using namespace App::ChatMessenger::interface;
class MsgManagementAPI : public QObject {
    Q_OBJECT
public:
    MsgManagementAPI(QObject *parent = nullptr);
    // 비동기 메서드 호출 (DBus를 통한 메시지 전송)
    Q_INVOKABLE void sendMessage(const QString &message);
    Q_INVOKABLE void notifyTyping();
signals:
    // 비동기 호출 완료 시 시그널 (성공 또는 실패)
    void sendMessageFinished();
    void sendMessageFailed(const QString &error_message);
    void notifyTypingFinished();
    void notifyTypingFailed(const QString &error_message);
    // 서버 발행 시그널 수신 후 Qt 시그널로 변환
    void newMessageReceived(const QString &user_name, const QString &message);
    void userTypingSignal(const QString &user_name);
private:
    Glib::RefPtr<MessageProxy> proxy;
    // 비동기 호출 완료 시 호출될 콜백 함수
    void onSendMessageFinish(const Glib::RefPtr<Gio::AsyncResult> &result);
    void onNotifyTypingFinish(const Glib::RefPtr<Gio::AsyncResult> &result);
    // 서버 측 시그널 핸들러
    void onNewMsgReceived(const Glib::ustring &user_name, const Glib::ustring &message);
    void onUserTyping(const Glib::ustring &user_name);
};
#endif // MSG_MANAGEMENT_API_H