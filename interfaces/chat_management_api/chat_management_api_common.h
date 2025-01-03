#pragma once
#include <iostream>
#include <vector>
#include "glibmm.h"
#include "giomm.h"

namespace App {
namespace ChatMessenger {
namespace interface {

class ManagementTypeWrap {
public:
    template<typename T>
    static void unwrapList(std::vector<T> &list, const Glib::VariantContainerBase &wrapped) {
        for (uint i = 0; i < wrapped.get_n_children(); i++) {
            Glib::Variant<T> item;
            wrapped.get_child(item, i);
            list.push_back(item.get());
        }
    }

    static std::vector<Glib::ustring> stdStringVecToGlibStringVec(const std::vector<std::string> &strv) {
        std::vector<Glib::ustring> newStrv;
        for (uint i = 0; i < strv.size(); i++) {
            newStrv.push_back(strv[i]);
        }

        return newStrv;
    }

    static std::vector<std::string> glibStringVecToStdStringVec(const std::vector<Glib::ustring> &strv) {
        std::vector<std::string> newStrv;
        for (uint i = 0; i < strv.size(); i++) {
            newStrv.push_back(strv[i]);
        }

        return newStrv;
    }

    static Glib::VariantContainerBase RegisterUser_pack(
        const Glib::ustring & arg_user_name) {
        Glib::VariantContainerBase base;
        Glib::Variant<Glib::ustring> params =
            Glib::Variant<Glib::ustring>::create(arg_user_name);
        return Glib::VariantContainerBase::create_tuple(params);
    }

    static Glib::VariantContainerBase CreateChat_pack(
        const Glib::ustring & arg_chat_title) {
        Glib::VariantContainerBase base;
        Glib::Variant<Glib::ustring> params =
            Glib::Variant<Glib::ustring>::create(arg_chat_title);
        return Glib::VariantContainerBase::create_tuple(params);
    }

    static Glib::VariantContainerBase GetUserList_pack(
        const Glib::ustring & arg_chat_title) {
        Glib::VariantContainerBase base;
        Glib::Variant<Glib::ustring> params =
            Glib::Variant<Glib::ustring>::create(arg_chat_title);
        return Glib::VariantContainerBase::create_tuple(params);
    }

    static Glib::VariantContainerBase JoinChat_pack(
        const Glib::ustring & arg_chat_title) {
        Glib::VariantContainerBase base;
        Glib::Variant<Glib::ustring> params =
            Glib::Variant<Glib::ustring>::create(arg_chat_title);
        return Glib::VariantContainerBase::create_tuple(params);
    }
};

} // interface
} // ChatMessenger
} // App


