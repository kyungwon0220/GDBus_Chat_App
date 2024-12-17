#pragma once
#include <string>
#include <vector>
#include <glibmm.h>
#include <giomm.h>
#include "chat_management_api_common.h"

namespace App {
namespace ChatMessenger {
namespace interface {

class ManagementStub : public sigc::trackable {
public:
    ManagementStub();
    virtual ~ManagementStub();

    ManagementStub(const ManagementStub &other) = delete;
    ManagementStub(ManagementStub &&other) = delete;
    ManagementStub &operator=(const ManagementStub &other) = delete;
    ManagementStub &operator=(ManagementStub &&other) = delete;

    guint register_object(const Glib::RefPtr<Gio::DBus::Connection> &connection,
                          const Glib::ustring &object_path);
    void unregister_object();

    unsigned int usage_count() const {
        return static_cast<unsigned int>(m_registered_objects.size());
    }

    class MethodInvocation;

protected:
    virtual void RegisterUser(
        const Glib::ustring & user_name,
        MethodInvocation &invocation) = 0;
    virtual void CreateChat(
        const Glib::ustring & chat_title,
        MethodInvocation &invocation) = 0;
    virtual void JoinChat(
        const Glib::ustring & chat_title,
        MethodInvocation &invocation) = 0;
    virtual void LeaveChat(
        MethodInvocation &invocation) = 0;
    virtual void GetUserList(
        const Glib::ustring & chat_title,
        MethodInvocation &invocation) = 0;

    void ChatListUpdated_emitter(const std::vector<Glib::ustring> &destination_bus_names, const std::vector<Glib::ustring> &);
    sigc::signal<void, const std::vector<Glib::ustring> &> ChatListUpdated_signal;
    sigc::signal<void, const std::vector<Glib::ustring>&, const std::vector<Glib::ustring> &> ChatListUpdated_selectiveSignal;

    void UserJoined_emitter(const std::vector<Glib::ustring> &destination_bus_names, const Glib::ustring &, const Glib::ustring &);
    sigc::signal<void, const Glib::ustring &, const Glib::ustring &> UserJoined_signal;
    sigc::signal<void, const std::vector<Glib::ustring>&, const Glib::ustring &, const Glib::ustring &> UserJoined_selectiveSignal;

    void UserLeft_emitter(const std::vector<Glib::ustring> &destination_bus_names, const Glib::ustring &, const Glib::ustring &);
    sigc::signal<void, const Glib::ustring &, const Glib::ustring &> UserLeft_signal;
    sigc::signal<void, const std::vector<Glib::ustring>&, const Glib::ustring &, const Glib::ustring &> UserLeft_selectiveSignal;

    void on_method_call(const Glib::RefPtr<Gio::DBus::Connection> &connection,
                        const Glib::ustring &sender,
                        const Glib::ustring &object_path,
                        const Glib::ustring &interface_name,
                        const Glib::ustring &method_name,
                        const Glib::VariantContainerBase &parameters,
                        const Glib::RefPtr<Gio::DBus::MethodInvocation> &invocation);

    void on_interface_get_property(Glib::VariantBase& property,
                                   const Glib::RefPtr<Gio::DBus::Connection> &connection,
                                   const Glib::ustring &sender,
                                   const Glib::ustring &object_path,
                                   const Glib::ustring &interface_name,
                                   const Glib::ustring &property_name);

    bool on_interface_set_property(
        const Glib::RefPtr<Gio::DBus::Connection> &connection,
        const Glib::ustring &sender,
        const Glib::ustring &object_path,
        const Glib::ustring &interface_name,
        const Glib::ustring &property_name,
        const Glib::VariantBase &value);

private:
    bool emitSignal(const std::string &propName, Glib::VariantBase &value);

    struct RegisteredObject {
        guint id;
        Glib::RefPtr<Gio::DBus::Connection> connection;
        std::string object_path;
    };

    Glib::RefPtr<Gio::DBus::NodeInfo> introspection_data;
    std::vector<RegisteredObject> m_registered_objects;
    std::string m_interfaceName;
};

class ManagementStub::MethodInvocation {
public:
    MethodInvocation(const Glib::RefPtr<Gio::DBus::MethodInvocation> &msg):
        m_message(msg) {}

    const Glib::RefPtr<Gio::DBus::MethodInvocation> getMessage() {
        return m_message;
    }

    void ret(Glib::Error error) {
        m_message->return_error(error);
    }

    void returnError(const Glib::ustring &domain, int code, const Glib::ustring &message) {
        m_message->return_error(domain, code, message);
    }

    void ret(const std::vector<Glib::ustring> & p0) {
        std::vector<Glib::VariantBase> vlist;
        Glib::Variant<std::vector<Glib::ustring>> var0 =
            Glib::Variant<std::vector<Glib::ustring>>::create(p0);
        vlist.push_back(var0);

        m_message->return_value(Glib::Variant<Glib::VariantBase>::create_tuple(vlist));
    }

    void ret(const Glib::ustring & p0, const std::vector<Glib::ustring> & p1) {
        std::vector<Glib::VariantBase> vlist;
        Glib::Variant<Glib::ustring> var0 =
            Glib::Variant<Glib::ustring>::create(p0);
        vlist.push_back(var0);
        Glib::Variant<std::vector<Glib::ustring>> var1 =
            Glib::Variant<std::vector<Glib::ustring>>::create(p1);
        vlist.push_back(var1);

        m_message->return_value(Glib::Variant<Glib::VariantBase>::create_tuple(vlist));
    }

    void ret() {
        std::vector<Glib::VariantBase> vlist;

        m_message->return_value(Glib::Variant<Glib::VariantBase>::create_tuple(vlist));
    }

private:
    Glib::RefPtr<Gio::DBus::MethodInvocation> m_message;
};

} // interface
} // ChatMessenger
} // App

