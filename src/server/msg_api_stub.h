#ifndef MESSAGE_STUB_H
#define MESSAGE_STUB_H
#include "../../interfaces/msg_api/msg_api_stub.h"
#include "server_data.hpp"
class MessageStub : public App::ChatMessenger::interface::MessageStub {
public:
    void NotifyTyping(MethodInvocation &invocation) override;
    void UserStopTyping(MethodInvocation &invocation) override;
    void SendMsg(const Glib::ustring &message, MethodInvocation &invocation) override;
};
#endif