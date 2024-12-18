#include "chat_management_api_stub.h"
#include "msg_api_stub.h"
#include "server_data.hpp"
ServerData& serverData = ServerData::instance();

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

    // D-Bus 데몬이, 모든 이름 변화에 대해 발행하는 NameOwnerChanged 신호로 프로그램 종료 또는 D-Bus 연결 끊김을 감지 ~
        // org.freedesktop.DBus 인터페이스용 프록시 생성 ( DBus 시스템(또는 세션) 버스에 대한 프록시 ) ~
        auto dbusProxy = Gio::DBus::Proxy::create_for_bus_sync(
            Gio::DBus::BusType::BUS_TYPE_SESSION,
            "org.freedesktop.DBus",
            "/org/freedesktop/DBus",
            "org.freedesktop.DBus"
        );
        // ~ org.freedesktop.DBus 인터페이스용 프록시 생성 ( DBus 시스템(또는 세션) 버스에 대한 프록시 )

        // NameOwnerChanged 시그널 수신(org.freedesktop.DBus 인터페이스를 통해 /org/freedesktop/DBus 오브젝트 경로에서 발생) ~
        dbusProxy->signal_signal().connect(
            [&](const Glib::ustring &sender_name,
            const Glib::ustring &signal_name,
            const Glib::VariantContainerBase &parameters) {
                if (signal_name == "NameOwnerChanged") {
                    Glib::Variant<Glib::ustring> var_name;
                    Glib::Variant<Glib::ustring> var_old_owner;
                    Glib::Variant<Glib::ustring> var_new_owner;

                    parameters.get_child(var_name, 0);
                    parameters.get_child(var_old_owner, 1);
                    parameters.get_child(var_new_owner, 2);

                    Glib::ustring name = var_name.get();
                    Glib::ustring old_owner = var_old_owner.get();
                    Glib::ustring new_owner = var_new_owner.get();

                    if (!old_owner.empty() && new_owner.empty()) {
                        // std::cout << "Client with bus name " << name << " disconnected." << std::endl;
                        auto it = serverData.user_map.find(name.raw());
                        if (it != serverData.user_map.end()) {
                                                                    // 서버 Log 출력 ~
                                                                    std::cout << "Current user_map before removal:" << std::endl;
                                                                    for (const auto &pair : serverData.user_map) {
                                                                        const std::string &bus_name = pair.first;
                                                                        const auto &user_info = pair.second;
                                                                        std::cout << "  bus_name: " << bus_name 
                                                                                << ", user_name: " << std::get<0>(user_info) 
                                                                                << ", chat_title: " << std::get<1>(user_info) << std::endl;
                                                                    }
                                                                    // ~ 서버 Log 출력

                            user_manager.LeaveChat_impl(name.raw());
                            serverData.user_map.erase(it);
                                                                    // 서버 Log 출력 ~
                                                                    std::cout << "Current user_map after removal:" << std::endl;
                                                                    for (const auto &pair : serverData.user_map) {
                                                                        const std::string &bus_name = pair.first;
                                                                        const auto &user_info = pair.second;
                                                                        std::cout << "  bus_name: " << bus_name 
                                                                                << ", user_name: " << std::get<0>(user_info) 
                                                                                << ", chat_title: " << std::get<1>(user_info) << std::endl;
                                                                    }
                                                                    // ~ 서버 Log 출력
                        }
                    }
                }
            }
        );
        // ~ NameOwnerChanged 시그널 수신(org.freedesktop.DBus 인터페이스를 통해 /org/freedesktop/DBus 오브젝트 경로에서 발생)
    // ~ D-Bus 데몬이, 모든 이름 변화에 대해 발행하는 NameOwnerChanged 신호로 프로그램 종료 또는 D-Bus 연결 끊김을 감지

    std::cout << "Server is running..." << std::endl;

    Glib::RefPtr<Glib::MainLoop> loop = Glib::MainLoop::create();
    loop->run();
}