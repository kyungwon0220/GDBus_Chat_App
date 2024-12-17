#include "chatlist.h"   // 채팅방 목록 창
#include "chatroom.h"   // 채팅방 접속자 목록 창
#include "room.h"       // 채팅방 창
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
using namespace App::ChatMessenger::interface;
class ChatClient : public QObject {
    Q_OBJECT
public:
    ChatClient(QObject *parent = nullptr) : QObject(parent) {
        try {
            Gio::Application::create("App.ChatMessenger.Client");
            // chat_management_api Proxy 생성 ~
            Proxy = ManagementProxy::createForBus_sync(
                Gio::DBus::BusType::BUS_TYPE_SESSION,
                Gio::DBus::ProxyFlags::PROXY_FLAGS_NONE,
                "App.ChatMessenger.Server",
                "/App/ChatMessenger/UserManager"); if (!Proxy) std::cerr << "Failed to create user proxy." << std::endl;
            // ~ chat_management_api Proxy 생성
        } catch (const Glib::Error &ex) {
            std::cerr << "Error occurred: " << ex.what() << std::endl;
        }
    }
public slots:
    void registerUser(const QString &user_name) {                                                                               // 비동기적으로, 서버에게 사용자 등록 요청
        if (Proxy) { Proxy->RegisterUser( user_name.toStdString(), sigc::mem_fun(*this, &ChatClient::onRegisterUserFinish) ); } // 비동기 호출이 완료되면 호출될 콜백 등록 ( onRegisterUserFinish() )
    }
signals:
    void registrationSucceeded();                           // 서버에 user_name 등록 성공 시그널
    void registrationFailed(const QString &errorMessage);   // 서버에 user_name 등록 실패 시그널
private:
    Glib::RefPtr<ManagementProxy> Proxy;
    void onRegisterUserFinish(const Glib::RefPtr<Gio::AsyncResult> &result) {   // 서버에서 데이터를 반환하면 이 콜백이 실행
        try {
            Proxy->RegisterUser_finish(result);
            emit registrationSucceeded();                                       // 등록 성공 시 시그널 방출
        } catch (const Glib::Error &ex) {
            emit registrationFailed(QString::fromStdString(ex.what()));         // 등록 실패 시 시그널 방출
        }
    }
};
int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv); QQmlApplicationEngine engine;
    ChatClient chatClient; engine.rootContext()->setContextProperty("Client", &chatClient); // ChatClient 인스턴스를 생성하고 QML에 등록
    const QUrl url(QStringLiteral("qrc:/QTvClient/Main.qml"));                          // 사용자 등록을 위한, 최초 시작 창의 QML 파일 로드
    engine.load(url);
    qmlRegisterType<ChatListManager>("App.ChatMessenger", 1, 0, "ChatListManager");         // ChatListManager 클래스를 QML에 등록
    qmlRegisterType<ChatRoomManager>("App.ChatMessenger", 1, 0, "ChatRoomManager");         // ChatRoomManager 클래스를 QML에 등록
    qmlRegisterType<RoomManager>("App.ChatMessenger", 1, 0, "RoomManager");
    return app.exec();
}
#include "main.moc"