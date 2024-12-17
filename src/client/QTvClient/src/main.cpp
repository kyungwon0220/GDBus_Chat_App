#include "../src/header/chat_management_api.h"
#include "../src/header/msg_management_api.h"
#include "../src/header/window_manager.hpp"
#include <QGuiApplication>

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    Glib::init(); Gio::init();  // 서버가 발행하는 신호를 정상적으로 수신하기 위해, Glib/Gio 라이브러리의 내부 초기화

    // C++에서 ChatManagementAPI 인스턴스를 생성하고 QML에 등록 ~
    ChatManagementAPI chatManagementAPI;
    engine.rootContext()->setContextProperty("chatManagementAPI", &chatManagementAPI);
    // ~ C++에서 ChatManagementAPI 인스턴스를 생성하고 QML에 등록

    // MsgManagementAPI 인스턴스 생성 및 QML에 등록 ~
    MsgManagementAPI messageAPI;
    engine.rootContext()->setContextProperty("messageAPI", &messageAPI);
    // ~ MsgManagementAPI 인스턴스 생성 및 QML에 등록

    WindowManager windowManager;
    engine.rootContext()->setContextProperty("windowManager", &windowManager);


    engine.load(QUrl(QStringLiteral("qrc:/QTvClient/qml/Main.qml")));   // Main.qml 파일을 로드하여 표시
    if (engine.rootObjects().isEmpty()) {
        return -1;  // QML 창 로드 실패 시 종료
    }

    return app.exec();
}