#ifndef WINDOW_MANAGER_HPP
#define WINDOW_MANAGER_HPP
#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
// #include <QDebug>
class WindowManager : public QObject {
    Q_OBJECT
public:
    explicit WindowManager(QObject *parent = nullptr)
        : QObject(parent), globalChatListWindow(nullptr), globalUserListWindow(nullptr), globalChatRoomWindow(nullptr) {}
    Q_INVOKABLE void setGlobalChatListWindow(QObject *window) {
        if (globalChatListWindow) {
            globalChatListWindow->deleteLater();
        }
        globalChatListWindow = window;
        qDebug() << "Global Chat List Window updated.";
    }
    Q_INVOKABLE QObject *getGlobalChatListWindow() const {
        return globalChatListWindow;
    }
    Q_INVOKABLE void setGlobalUserListWindow(QObject *window) {
        if (globalUserListWindow) {
            globalUserListWindow->deleteLater();
        }
        globalUserListWindow = window;
        qDebug() << "Global User List Window updated.";
    }
    Q_INVOKABLE QObject *getGlobalUserListWindow() const {
        return globalUserListWindow;
    }
    Q_INVOKABLE void setGlobalChatRoomWindow(QObject *window) {
        if (globalChatRoomWindow) {
            globalChatRoomWindow->deleteLater();
        }
        globalChatRoomWindow = window;
        qDebug() << "Global Chat Room Window updated.";
    }
    Q_INVOKABLE QObject *getGlobalChatRoomWindow() const {
        return globalChatRoomWindow;
    }
private:
    QObject *globalChatListWindow;
    QObject *globalUserListWindow;
    QObject *globalChatRoomWindow;
};
#endif // WINDOW_MANAGER_HPP