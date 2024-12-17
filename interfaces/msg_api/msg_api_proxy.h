#pragma once
#include <string>
#include <tuple>
#include <vector>
#include <glibmm.h>
#include <giomm.h>
#include "msg_api_common.h"

namespace App {
namespace ChatMessenger {
namespace interface {

class MessageProxy : public Glib::ObjectBase {
public:
    static void createForBus(Gio::DBus::BusType busType,
                             Gio::DBus::ProxyFlags proxyFlags,
                             const std::string &name,
                             const std::string &objectPath,
                             const Gio::SlotAsyncReady &slot,
                             const Glib::RefPtr<Gio::Cancellable> &cancellable = {});

    static Glib::RefPtr<MessageProxy> createForBusFinish (const Glib::RefPtr<Gio::AsyncResult> &result);

    static Glib::RefPtr<MessageProxy> createForBus_sync(
        Gio::DBus::BusType busType,
        Gio::DBus::ProxyFlags proxyFlags,
        const std::string &name,
        const std::string &objectPath,
        const Glib::RefPtr<Gio::Cancellable> &cancellable = {});

    Glib::RefPtr<Gio::DBus::Proxy> dbusProxy() const { return m_proxy; }

    void SendMsg(
        const Glib::ustring & message,
        const Gio::SlotAsyncReady &slot,
        const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    void SendMsg_finish (
        const Glib::RefPtr<Gio::AsyncResult> &res);

    void
    SendMsg_sync(
        const Glib::ustring & message,const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    void NotifyTyping(
        const Gio::SlotAsyncReady &slot,
        const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    void NotifyTyping_finish (
        const Glib::RefPtr<Gio::AsyncResult> &res);

    void
    NotifyTyping_sync(
const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    void UserStopTyping(
        const Gio::SlotAsyncReady &slot,
        const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    void UserStopTyping_finish (
        const Glib::RefPtr<Gio::AsyncResult> &res);

    void
    UserStopTyping_sync(
const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    sigc::signal<void, Glib::ustring, Glib::ustring > NewMsgReceived_signal;
    sigc::signal<void, Glib::ustring > UserTyping_signal;
    sigc::signal<void, Glib::ustring > UserStoppedTyping_signal;

    void reference() const override {}
    void unreference() const override {}

protected:
    Glib::RefPtr<Gio::DBus::Proxy> m_proxy;

private:
    MessageProxy(const Glib::RefPtr<Gio::DBus::Proxy> &proxy);

    void handle_signal(const Glib::ustring &sender_name,
                       const Glib::ustring &signal_name,
                       const Glib::VariantContainerBase &parameters);

    void handle_properties_changed(const Gio::DBus::Proxy::MapChangedProperties &changed_properties,
                                   const std::vector<Glib::ustring> &invalidated_properties);

};

}// interface
}// ChatMessenger
}// App

