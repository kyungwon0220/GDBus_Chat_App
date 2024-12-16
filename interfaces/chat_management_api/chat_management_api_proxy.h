#pragma once
#include <string>
#include <tuple>
#include <vector>
#include <glibmm.h>
#include <giomm.h>
#include "chat_management_api_common.h"

namespace App {
namespace ChatMessenger {
namespace interface {

class ManagementProxy : public Glib::ObjectBase {
public:
    static void createForBus(Gio::DBus::BusType busType,
                             Gio::DBus::ProxyFlags proxyFlags,
                             const std::string &name,
                             const std::string &objectPath,
                             const Gio::SlotAsyncReady &slot,
                             const Glib::RefPtr<Gio::Cancellable> &cancellable = {});

    static Glib::RefPtr<ManagementProxy> createForBusFinish (const Glib::RefPtr<Gio::AsyncResult> &result);

    static Glib::RefPtr<ManagementProxy> createForBus_sync(
        Gio::DBus::BusType busType,
        Gio::DBus::ProxyFlags proxyFlags,
        const std::string &name,
        const std::string &objectPath,
        const Glib::RefPtr<Gio::Cancellable> &cancellable = {});

    Glib::RefPtr<Gio::DBus::Proxy> dbusProxy() const { return m_proxy; }

    void RegisterUser(
        const Glib::ustring & user_name,
        const Gio::SlotAsyncReady &slot,
        const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    void RegisterUser_finish (
        const Glib::RefPtr<Gio::AsyncResult> &res);

    void
    RegisterUser_sync(
        const Glib::ustring & user_name,const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    void GetChatList(
        const Gio::SlotAsyncReady &slot,
        const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    void GetChatList_finish (
        std::vector<Glib::ustring> &chat_list,
        const Glib::RefPtr<Gio::AsyncResult> &res);

    std::vector<Glib::ustring>
    GetChatList_sync(
const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    void CreateChat(
        const Glib::ustring & chat_title,
        const Gio::SlotAsyncReady &slot,
        const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    void CreateChat_finish (
        Glib::ustring &chat_title,
        const Glib::RefPtr<Gio::AsyncResult> &res);

    Glib::ustring
    CreateChat_sync(
        const Glib::ustring & chat_title,const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    void JoinChat(
        const Glib::ustring & chat_title,
        const Gio::SlotAsyncReady &slot,
        const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    void JoinChat_finish (
        const Glib::RefPtr<Gio::AsyncResult> &res);

    void
    JoinChat_sync(
        const Glib::ustring & chat_title,const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    void LeaveChat(
        const Gio::SlotAsyncReady &slot,
        const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    void LeaveChat_finish (
        const Glib::RefPtr<Gio::AsyncResult> &res);

    void
    LeaveChat_sync(
const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    void GetUserList(
        const Glib::ustring & chat_title,
        const Gio::SlotAsyncReady &slot,
        const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    void GetUserList_finish (
        std::vector<Glib::ustring> &user_list,
        const Glib::RefPtr<Gio::AsyncResult> &res);

    std::vector<Glib::ustring>
    GetUserList_sync(
        const Glib::ustring & chat_title,const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    sigc::signal<void, std::vector<Glib::ustring> > ChatListUpdated_signal;
    sigc::signal<void, Glib::ustring, Glib::ustring > UserJoined_signal;
    sigc::signal<void, Glib::ustring, Glib::ustring > UserLeft_signal;

    void reference() const override {}
    void unreference() const override {}

protected:
    Glib::RefPtr<Gio::DBus::Proxy> m_proxy;

private:
    ManagementProxy(const Glib::RefPtr<Gio::DBus::Proxy> &proxy);

    void handle_signal(const Glib::ustring &sender_name,
                       const Glib::ustring &signal_name,
                       const Glib::VariantContainerBase &parameters);

    void handle_properties_changed(const Gio::DBus::Proxy::MapChangedProperties &changed_properties,
                                   const std::vector<Glib::ustring> &invalidated_properties);

};

}// interface
}// ChatMessenger
}// App

