#ifndef MANAGEMENT_STUB_H
#define MANAGEMENT_STUB_H
#include "../../interfaces/chat_management_api/chat_management_api_stub.h"
#include "server_data.hpp"
class ManagementStub : public App::ChatMessenger::interface::ManagementStub {
public:
    void RegisterUser(const Glib::ustring &user_name, MethodInvocation &invocation) override;
    void GetUserList(const Glib::ustring &chat_title, MethodInvocation &invocation) override;
    void CreateChat(const Glib::ustring &chat_title, MethodInvocation &invocation) override;
    void JoinChat(const Glib::ustring &chat_title, MethodInvocation &invocation) override;
    void LeaveChat(MethodInvocation &invocation) override;
};
#endif