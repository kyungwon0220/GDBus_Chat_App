#include "../../interfaces/msg_api/msg_api_proxy.h"
using namespace App::ChatMessenger::interface;
int main(int argc, char *argv[]) {
    try {
        Gio::Application::create("App.ChatMessenger.Client"); // GLib와 GIO의 내부 초기화를 수행하는 중요한 역할 ( GIO 시스템 초기화 )
        // Gio::DBus::Connection::get_sync(Gio::DBus::BusType::BUS_TYPE_SESSION); // createForBus_sync() 함수는 내부적으로 D-Bus 연결을 자동으로 생성하여 설정합니다.
        Glib::RefPtr<MessageProxy> proxy = MessageProxy::createForBus_sync( // 서버와의 프록시 생성
            Gio::DBus::BusType::BUS_TYPE_SESSION,
            Gio::DBus::ProxyFlags::PROXY_FLAGS_NONE,
            "App.ChatMessenger.Server",
            "/App/ChatMessenger/MessageManager"); // 구체적인 객체(또는 인터페이스)를 식별 ( 객체 경로 )
        if (!proxy) { std::cerr << "Failed to create proxy." << std::endl; return 1; }
        std::string chat_id = "임의의 클라이언트 ID"; std::string user_name = "임의의 클라이언트 대화명"; std::string message = "Hello from client!"; // Debug
        proxy->SendMsg_sync(message);
        std::cout << "' " << chat_id << " ( " << user_name << " ) ' : " << message << std::endl;
        
    } catch (const Glib::Error &ex) {
        std::cerr << "Error occurred: " << ex.what() << std::endl;
    }
    return 0;  // 클라이언트는 서버와 달리 즉시 종료될 수 있음
}
