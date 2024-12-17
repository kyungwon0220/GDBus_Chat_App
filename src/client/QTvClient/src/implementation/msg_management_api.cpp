#include "../header/msg_management_api.h"

MsgManagementAPI::MsgManagementAPI(QObject *parent) : QObject(parent) {
    try {
        proxy = MessageProxy::createForBus_sync(
            Gio::DBus::BusType::BUS_TYPE_SESSION,
            Gio::DBus::ProxyFlags::PROXY_FLAGS_NONE,
            "App.ChatMessenger.Server",
            "/App/ChatMessenger/MessageManager");

        if (!proxy) {
            std::cerr << "Failed to create message proxy." << std::endl;
        } else {
            // 서버에서 발행하는 신호와 Qt 시그널 핸들러를 연결
            proxy->NewMsgReceived_signal.connect(sigc::mem_fun(*this, &MsgManagementAPI::onNewMsgReceived));
            proxy->UserTyping_signal.connect(sigc::mem_fun(*this, &MsgManagementAPI::onUserTyping));
            proxy->UserStoppedTyping_signal.connect(sigc::mem_fun(*this, &MsgManagementAPI::onUserStoppedTyping));
        }
    } catch (const Glib::Error &ex) {
        std::cerr << "Error occurred while creating message proxy: " << ex.what() << std::endl;
    }
}

// 서버 측 시그널 수신 ~
void MsgManagementAPI::onNewMsgReceived(const Glib::ustring &user_name, const Glib::ustring &message) {
    emit newMessageReceived(QString::fromStdString(user_name.raw()), QString::fromStdString(message.raw()));
}

void MsgManagementAPI::onUserTyping(const Glib::ustring &user_name) {
    emit userTypingSignal(QString::fromStdString(user_name.raw()));
}

void MsgManagementAPI::onUserStoppedTyping(const Glib::ustring &user_name) {
    emit userStoppedTypingSignal(QString::fromStdString(user_name.raw()));
}
// ~ 서버 측 시그널 수신




void MsgManagementAPI::sendMessage(const QString &message) {
    if (proxy) {    // 비동기 호출이 완료되면 호출될 콜백 등록 ( onSendMessageFinish() )
        proxy->SendMsg(message.toStdString(), sigc::mem_fun(*this, &MsgManagementAPI::onSendMessageFinish));
    } else {
        emit sendMessageFailed("Message proxy is not initialized.");
    }
}

void MsgManagementAPI::onSendMessageFinish(const Glib::RefPtr<Gio::AsyncResult> &result) {  // 서버에서 데이터를 반환하면 이 콜백이 실행
    try {
        // SendMsg에는 서버에서 별다른 반환값을 주지 않으므로 finish 호출 후 별도 결과 처리 필요 없음
        proxy->SendMsg_finish(result);
        emit sendMessageFinished();
    } catch (const Glib::Error &ex) {
        emit sendMessageFailed(QString::fromStdString(ex.what()));
    }
}

void MsgManagementAPI::notifyTyping() {
    if (proxy) {    // 비동기 호출이 완료되면 호출될 콜백 등록 ( onNotifyTypingFinish() )
        proxy->NotifyTyping(sigc::mem_fun(*this, &MsgManagementAPI::onNotifyTypingFinish));
    } else {
        emit notifyTypingFailed("Message proxy is not initialized.");
    }
}

void MsgManagementAPI::onNotifyTypingFinish(const Glib::RefPtr<Gio::AsyncResult> &result) { // 서버에서 데이터를 반환하면 이 콜백이 실행
    try {
        // NotifyTyping도 반환값이 없으므로 finish 호출 후 결과 예외 처리만 수행
        proxy->NotifyTyping_finish(result);
        emit notifyTypingFinished();
    } catch (const Glib::Error &ex) {
        emit notifyTypingFailed(QString::fromStdString(ex.what()));
    }
}

void MsgManagementAPI::userStopTyping() {
    if (proxy) {    // 비동기 호출이 완료되면 호출될 콜백 등록 ( onUserStopTypingFinish() )
        proxy->UserStopTyping(sigc::mem_fun(*this, &MsgManagementAPI::onUserStopTypingFinish));
    } else {
        emit userStopTypingFailed("Message proxy is not initialized.");
    }
}

void MsgManagementAPI::onUserStopTypingFinish(const Glib::RefPtr<Gio::AsyncResult> &result) {
    try {
        proxy->UserStopTyping_finish(result);
        emit userStopTypingFinished();
    } catch (const Glib::Error &ex) {
        emit userStopTypingFailed(QString::fromStdString(ex.what()));
    }
}
