#ifndef MAIN_H
#define MAIN_H
#include "../../../interfaces/chat_management_api/chat_management_api_proxy.h"
#include <QObject>
#include <QString>
#include <QQmlApplicationEngine>
using namespace App::ChatMessenger::interface;
class ChatClient : public QObject {
    Q_OBJECT
public:
    ChatClient(QObject *parent = nullptr);
public slots:
    void registerUser(const QString &user_name);                                // Main.qml 창에서, "대화명 사용하기" 버튼 클릭시, 비동기적으로 서버의 RegisterUser()
signals:
    void registrationSucceeded();                                               // 서버의 ' RegisterUser() ' 성공시 발행
    void registrationFailed(const QString &errorMessage);                       // 서버의 ' RegisterUser() ' 실패시 발행
private:
    Glib::RefPtr<ManagementProxy> proxy;
    void onRegisterUserFinish(const Glib::RefPtr<Gio::AsyncResult> &result);    // registerUser 슬롯의, 비동기 호출이 완료되면 호출될 콜백
public:
    QQmlApplicationEngine *engine;  // QQmlApplicationEngine 포인터를 멤버로 추가
    QObject *mainWindow;            // Main.qml 창을 관리하기 위한 포인터
    QObject *chatListWindow;        // ChatList.qml 창을 관리하기 위한 포인터
};
#endif // MAIN_H