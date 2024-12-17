#include "main.h"
#include "chatlist.h"   // 채팅방 목록 창
#include "chatroom.h"   // 채팅방 접속자 목록 창
#include "room.h"       // 채팅방 창

#include <QGuiApplication>
#include <QQmlContext>

ChatClient::ChatClient(QObject *parent) : QObject(parent) {
    try {
        Gio::Application::create("App.ChatMessenger.Client");

        // chat_management_api Proxy 생성 ~
        proxy = ManagementProxy::createForBus_sync(
            Gio::DBus::BusType::BUS_TYPE_SESSION,
            Gio::DBus::ProxyFlags::PROXY_FLAGS_NONE,
            "App.ChatMessenger.Server",
            "/App/ChatMessenger/UserManager"); if (!proxy) std::cerr << "Failed to create user proxy." << std::endl;
        // ~ chat_management_api Proxy 생성
    } catch (const Glib::Error &ex) {
        std::cerr << "Error occurred: " << ex.what() << std::endl;
    }
}

void ChatClient::registerUser(const QString &user_name) {   // 비동기적으로, 서버에게 사용자 등록 요청
    if (proxy) { proxy->RegisterUser(user_name.toStdString(), sigc::mem_fun(*this, &ChatClient::onRegisterUserFinish)); }   // 비동기 호출이 완료되면 호출될 콜백 등록 ( onRegisterUserFinish() )
}

void ChatClient::onRegisterUserFinish(const Glib::RefPtr<Gio::AsyncResult> &result) {   // 서버에서 데이터를 반환하면 이 콜백이 실행
    try {
        proxy->RegisterUser_finish(result);
        emit registrationSucceeded();                               // 등록 성공 시 시그널 방출

        // Main.qml 창 닫고, ChatList.qml 창 띄우기 ~
        if (mainWindow) { mainWindow->setProperty("visible", false); }

        engine->load(QUrl(QStringLiteral("qrc:/QTvClient/ChatList.qml")));

        if (engine->rootObjects().isEmpty()) { qCritical() << "Failed to load ChatList.qml"; } else {
            QObjectList rootObjects = engine->rootObjects();
            chatListWindow = rootObjects.last();
            chatListWindow->setProperty("visible", true);
        }
        // ~ Main.qml 창 닫고, ChatList.qml 창 띄우기
    } catch (const Glib::Error &ex) {
        emit registrationFailed(QString::fromStdString(ex.what())); // 등록 실패 시 시그널 방출
    }
}

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);                                    // Qt 기반 GUI 애플리케이션을 실행하기 위한 필수적인 진입점
    ChatClient chatClient;
    QQmlApplicationEngine engine;
    chatClient.engine = &engine;
    engine.rootContext()->setContextProperty("Client", &chatClient);    // ChatClient 인스턴스를 생성하고 QML에 등록
    engine.load(QUrl(QStringLiteral("qrc:/QTvClient/Main.qml")));       // 사용자 등록을 위한, 최초 시작 창의 QML 파일 로드
    QObjectList rootObjects = engine.rootObjects();
    chatClient.mainWindow = rootObjects.first();                        // rootObjects의 첫 번째 객체를 mainWindow로 설정

    // 클래스 등록 ( 중앙 집중화 ) ~
    qmlRegisterType<ChatListManager>("App.ChatMessenger", 1, 0, "ChatListManager"); // ChatListManager 클래스를 QML에 등록
    qmlRegisterType<ChatRoomManager>("App.ChatMessenger", 1, 0, "ChatRoomManager"); // ChatRoomManager 클래스를 QML에 등록
    qmlRegisterType<RoomManager>("App.ChatMessenger", 1, 0, "RoomManager");
    // ~ 클래스 등록 ( 중앙 집중화 )

    return app.exec();
}

#include "main.moc"
