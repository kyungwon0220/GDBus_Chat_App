#include "chat_management_api_stub.h"
#include "msg_api_stub.h"

int main(/*int argc, char *argv[]*/) {
    Gio::Application::create("App.ChatMessenger.Server");
    auto connection = Gio::DBus::Connection::get_sync(Gio::DBus::BusType::BUS_TYPE_SESSION);
    Gio::DBus::own_name(Gio::DBus::BusType::BUS_TYPE_SESSION, "App.ChatMessenger.Server"); // Register D-Bus name // own_name() == D-Bus 버스 상에 서비스 이름을 등록하는 함수

    // ManagementStub 등록 ( chat_management_api )
    ManagementStub user_manager;
    user_manager.register_object(connection, "/App/ChatMessenger/UserManager");

    // MessageStub 등록 ( msg_api )
    MessageStub message_manager;
    message_manager.register_object(connection, "/App/ChatMessenger/MessageManager");

    std::cout << "Server is running..." << std::endl;

    Glib::RefPtr<Glib::MainLoop> loop = Glib::MainLoop::create();
    loop->run();
}