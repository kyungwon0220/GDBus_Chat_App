static const char interfaceXml0[] = R"XML_DELIMITER(<?xml version="1.0" encoding="UTF-8"?>
<node>
  <interface name="App.ChatMessenger.interface.Management">
    <!-- Methods -->
    <method name="RegisterUser">
      <arg name="user_name" type="s" direction="in"/>
      <arg name="chat_list" type="as" direction="out"/> <!-- 반환값: 채팅방 목록 -->
    </method>
    <method name="CreateChat">
      <arg name="chat_title" type="s" direction="in"/>
      <arg name="chat_title" type="s" direction="out"/> <!-- 반환값: 생성된 채팅방의 제목 -->
      <arg name="chat_list" type="as" direction="out"/> <!-- 반환값: 채팅방 목록 -->
    </method>
    <method name="JoinChat">
      <arg name="chat_title" type="s" direction="in"/>
    </method>
    <method name="LeaveChat">
      <arg name="chat_list" type="as" direction="out"/> <!-- 반환값: 채팅방 목록 -->
    </method>
    <method name="GetUserList">
      <arg name="chat_title" type="s" direction="in"/>
      <arg name="user_list" type="as" direction="out"/> <!-- 반환값: 수신받은 chat_title 채팅방에, 접속중인 클라이언트 목록 -->
    </method>
    <!-- Signals -->
    <signal name="ChatListUpdated">
      <arg name="chat_list" type="as"/>
    </signal>
    <signal name="UserJoined">
      <arg name="user_name" type="s"/>  <!-- 접속한 클라이언트 대화명(닉네임) -->
      <arg name="chat_title" type="s"/> <!-- 접속한 채팅방명 -->
    </signal>
    <signal name="UserLeft">
      <arg name="user_name" type="s"/>  <!-- 퇴장한 클라이언트 대화명(닉네임) -->
      <arg name="chat_title" type="s"/> <!-- 퇴장한 채팅방명 -->
    </signal>
  </interface>
</node>)XML_DELIMITER";

#include "chat_management_api_stub.h"

template<class T>
inline T specialGetter(Glib::Variant<T> variant)
{
    return variant.get();
}

template<>
inline std::string specialGetter(Glib::Variant<std::string> variant)
{
    // String is not guaranteed to be null-terminated, so don't use ::get()
    gsize n_elem;
    gsize elem_size = sizeof(char);
    char* data = (char*)g_variant_get_fixed_array(variant.gobj(), &n_elem, elem_size);

    return std::string(data, n_elem);
}

App::ChatMessenger::interface::ManagementStub::ManagementStub():
    m_interfaceName("App.ChatMessenger.interface.Management")
{
    ChatListUpdated_signal.connect(sigc::bind<0>(sigc::mem_fun(this, &ManagementStub::ChatListUpdated_emitter),
            std::vector<Glib::ustring>({""})) );
    ChatListUpdated_selectiveSignal.connect(sigc::mem_fun(this, &ManagementStub::ChatListUpdated_emitter));
    UserJoined_signal.connect(sigc::bind<0>(sigc::mem_fun(this, &ManagementStub::UserJoined_emitter),
            std::vector<Glib::ustring>({""})) );
    UserJoined_selectiveSignal.connect(sigc::mem_fun(this, &ManagementStub::UserJoined_emitter));
    UserLeft_signal.connect(sigc::bind<0>(sigc::mem_fun(this, &ManagementStub::UserLeft_emitter),
            std::vector<Glib::ustring>({""})) );
    UserLeft_selectiveSignal.connect(sigc::mem_fun(this, &ManagementStub::UserLeft_emitter));
}

App::ChatMessenger::interface::ManagementStub::~ManagementStub()
{
    unregister_object();
}

guint App::ChatMessenger::interface::ManagementStub::register_object(
    const Glib::RefPtr<Gio::DBus::Connection> &connection,
    const Glib::ustring &object_path)
{
    if (!introspection_data) {
        try {
            introspection_data = Gio::DBus::NodeInfo::create_for_xml(interfaceXml0);
        } catch(const Glib::Error& ex) {
            g_warning("Unable to create introspection data for %s: %s", object_path.c_str(), ex.what().c_str());
            return 0;
        }
    }

    Gio::DBus::InterfaceVTable *interface_vtable =
        new Gio::DBus::InterfaceVTable(
            sigc::mem_fun(this, &ManagementStub::on_method_call),
            sigc::mem_fun(this, &ManagementStub::on_interface_get_property),
            sigc::mem_fun(this, &ManagementStub::on_interface_set_property));

    guint registration_id;
    try {
        registration_id = connection->register_object(object_path,
            introspection_data->lookup_interface("App.ChatMessenger.interface.Management"),
            *interface_vtable);
    } catch(const Glib::Error &ex) {
        g_warning("Registration of object %s failed: %s", object_path.c_str(), ex.what().c_str());
        return 0;
    }

    m_registered_objects.emplace_back(RegisteredObject {
        registration_id,
        connection,
        object_path
    });

    return registration_id;
}

void App::ChatMessenger::interface::ManagementStub::unregister_object()
{
    for (const RegisteredObject &obj: m_registered_objects) {
        obj.connection->unregister_object(obj.id);
    }
    m_registered_objects.clear();
}

void App::ChatMessenger::interface::ManagementStub::on_method_call(
    const Glib::RefPtr<Gio::DBus::Connection> &/* connection */,
    const Glib::ustring &/* sender */,
    const Glib::ustring &/* object_path */,
    const Glib::ustring &/* interface_name */,
    const Glib::ustring &method_name,
    const Glib::VariantContainerBase &parameters,
    const Glib::RefPtr<Gio::DBus::MethodInvocation> &invocation)
{
    static_cast<void>(method_name); // maybe unused
    static_cast<void>(parameters); // maybe unused
    static_cast<void>(invocation); // maybe unused

    if (method_name.compare("RegisterUser") == 0) {
        Glib::Variant<Glib::ustring> base_user_name;
        parameters.get_child(base_user_name, 0);
        Glib::ustring p_user_name = specialGetter(base_user_name);

        MethodInvocation methodInvocation(invocation);
        RegisterUser(
            (p_user_name),
            methodInvocation);
    }

    if (method_name.compare("CreateChat") == 0) {
        Glib::Variant<Glib::ustring> base_chat_title;
        parameters.get_child(base_chat_title, 0);
        Glib::ustring p_chat_title = specialGetter(base_chat_title);

        MethodInvocation methodInvocation(invocation);
        CreateChat(
            (p_chat_title),
            methodInvocation);
    }

    if (method_name.compare("JoinChat") == 0) {
        Glib::Variant<Glib::ustring> base_chat_title;
        parameters.get_child(base_chat_title, 0);
        Glib::ustring p_chat_title = specialGetter(base_chat_title);

        MethodInvocation methodInvocation(invocation);
        JoinChat(
            (p_chat_title),
            methodInvocation);
    }

    if (method_name.compare("LeaveChat") == 0) {
        MethodInvocation methodInvocation(invocation);
        LeaveChat(
            methodInvocation);
    }

    if (method_name.compare("GetUserList") == 0) {
        Glib::Variant<Glib::ustring> base_chat_title;
        parameters.get_child(base_chat_title, 0);
        Glib::ustring p_chat_title = specialGetter(base_chat_title);

        MethodInvocation methodInvocation(invocation);
        GetUserList(
            (p_chat_title),
            methodInvocation);
    }

}

void App::ChatMessenger::interface::ManagementStub::on_interface_get_property(
    Glib::VariantBase &property,
    const Glib::RefPtr<Gio::DBus::Connection> &/* connection */,
    const Glib::ustring &/* sender */,
    const Glib::ustring &/* object_path */,
    const Glib::ustring &/* interface_name */,
    const Glib::ustring &property_name)
{
    static_cast<void>(property); // maybe unused
    static_cast<void>(property_name); // maybe unused

}

bool App::ChatMessenger::interface::ManagementStub::on_interface_set_property(
    const Glib::RefPtr<Gio::DBus::Connection> &/* connection */,
    const Glib::ustring &/* sender */,
    const Glib::ustring &/* object_path */,
    const Glib::ustring &/* interface_name */,
    const Glib::ustring &property_name,
    const Glib::VariantBase &value)
{
    static_cast<void>(property_name); // maybe unused
    static_cast<void>(value); // maybe unused

    return true;
}

void App::ChatMessenger::interface::ManagementStub::ChatListUpdated_emitter(
    const std::vector<Glib::ustring> &destination_bus_names,const std::vector<Glib::ustring> & chat_list)
{
    std::vector<Glib::VariantBase> paramsList;

    paramsList.push_back(Glib::Variant<std::vector<Glib::ustring>>::create((chat_list)));;

    const Glib::VariantContainerBase params =
        Glib::Variant<std::vector<Glib::VariantBase>>::create_tuple(paramsList);
    for (const RegisteredObject &obj: m_registered_objects) {
        for (const auto &bus_name: destination_bus_names) {
            obj.connection->emit_signal(
                    obj.object_path,
                    "App.ChatMessenger.interface.Management",
                    "ChatListUpdated",
                    bus_name,
                    params);
        }
    }
}

void App::ChatMessenger::interface::ManagementStub::UserJoined_emitter(
    const std::vector<Glib::ustring> &destination_bus_names,const Glib::ustring & user_name, const Glib::ustring & chat_title)
{
    std::vector<Glib::VariantBase> paramsList;

    paramsList.push_back(Glib::Variant<Glib::ustring>::create((user_name)));;
    paramsList.push_back(Glib::Variant<Glib::ustring>::create((chat_title)));;

    const Glib::VariantContainerBase params =
        Glib::Variant<std::vector<Glib::VariantBase>>::create_tuple(paramsList);
    for (const RegisteredObject &obj: m_registered_objects) {
        for (const auto &bus_name: destination_bus_names) {
            obj.connection->emit_signal(
                    obj.object_path,
                    "App.ChatMessenger.interface.Management",
                    "UserJoined",
                    bus_name,
                    params);
        }
    }
}

void App::ChatMessenger::interface::ManagementStub::UserLeft_emitter(
    const std::vector<Glib::ustring> &destination_bus_names,const Glib::ustring & user_name, const Glib::ustring & chat_title)
{
    std::vector<Glib::VariantBase> paramsList;

    paramsList.push_back(Glib::Variant<Glib::ustring>::create((user_name)));;
    paramsList.push_back(Glib::Variant<Glib::ustring>::create((chat_title)));;

    const Glib::VariantContainerBase params =
        Glib::Variant<std::vector<Glib::VariantBase>>::create_tuple(paramsList);
    for (const RegisteredObject &obj: m_registered_objects) {
        for (const auto &bus_name: destination_bus_names) {
            obj.connection->emit_signal(
                    obj.object_path,
                    "App.ChatMessenger.interface.Management",
                    "UserLeft",
                    bus_name,
                    params);
        }
    }
}


bool App::ChatMessenger::interface::ManagementStub::emitSignal(
    const std::string &propName,
    Glib::VariantBase &value)
{
    std::map<Glib::ustring, Glib::VariantBase> changedProps;
    std::vector<Glib::ustring> changedPropsNoValue;

    changedProps[propName] = value;

    Glib::Variant<std::map<Glib::ustring, Glib::VariantBase>> changedPropsVar =
        Glib::Variant<std::map<Glib::ustring, Glib::VariantBase>>::create(changedProps);
    Glib::Variant<std::vector<Glib::ustring>> changedPropsNoValueVar =
        Glib::Variant<std::vector<Glib::ustring>>::create(changedPropsNoValue);
    std::vector<Glib::VariantBase> ps;
    ps.push_back(Glib::Variant<Glib::ustring>::create(m_interfaceName));
    ps.push_back(changedPropsVar);
    ps.push_back(changedPropsNoValueVar);
    Glib::VariantContainerBase propertiesChangedVariant =
        Glib::Variant<std::vector<Glib::VariantBase>>::create_tuple(ps);

    for (const RegisteredObject &obj: m_registered_objects) {
        obj.connection->emit_signal(
            obj.object_path,
            "org.freedesktop.DBus.Properties",
            "PropertiesChanged",
            Glib::ustring(),
            propertiesChangedVariant);
    }

    return true;
}
