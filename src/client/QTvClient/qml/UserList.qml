import QtQuick 2.15
import QtQuick.Controls 2.15
Window {
    width: 320
    height: 720
    visible: true
    title: qsTr(chatTitle + "\tClient List")
    property string chatTitle: ""           // ChatList.qml에서 전달받을 채팅방 제목
    property var userListModel: []          // 서버에서 받은 chatTitle 채팅방의 클라이언트들 목록
    onClosing: { // 창이 닫힐 때의 이벤트 처리
        // UserList.qml 창이 열려 있다면 닫기 ~
        if (windowManager.getGlobalUserListWindow() !== null) {
            console.log("\tUserList.qml 프로그램 종료 버튼 클릭 ( UserList.qml 창 닫기 )");
            var existingUserListWindow = windowManager.getGlobalUserListWindow();
            existingUserListWindow.close(); // UserList.qml 창 닫기
            existingUserListWindow.destroy();   // 메모리 정리
            windowManager.setGlobalUserListWindow(null);    // 변수 초기화
        }
        // ~ UserList.qml 창이 열려 있다면 닫기
        // ChatRoom.qml 창이 열려 있다면 닫기 ~
        if (windowManager.getGlobalChatRoomWindow() !== null) {
            console.log("\tUserList.qml 프로그램 종료 버튼 클릭 ( ChatRoom.qml 창 닫기 )");
            var existingChatRoomWindow = windowManager.getGlobalChatRoomWindow();
            existingChatRoomWindow.close(); // UserList.qml 창 닫기
            existingChatRoomWindow.destroy();   // 메모리 정리
            windowManager.setGlobalChatRoomWindow(null);    // 변수 초기화
        }
        // ~ ChatRoom.qml 창이 열려 있다면 닫기
    }

    Connections {
        target: chatManagementAPI
        function onUserJoinedSignal(user_name, chat_title) { // 서버에서 UserJoined_emitter(destinations, user_name, chat_title); 시그널 발행 시
            console.log("function onUserJoinedSignal(user_name, chat_title)");
            chatManagementAPI.getUserList(chat_title); // 차후에, 시스템 메시지 표시 처리 예정
        }
        function onUserLeftSignal(userName, chat_title) { // 서버에서 UserLeft_emitter(destinations, user_name, Glib::ustring(chat_title)); 시그널 발행 시
            chatManagementAPI.getUserList(chat_title); // 차후에, 시스템 메시지 표시 처리 예정
            console.log("function onUserLeftSignal(userName, chat_title)");
        }
        function onJoinChatFinished(userList) {  // onJoinChatFinish() emit joinChatFinished(qmlUserList);
            userListModel = userList;
            console.log(chatTitle + "onJoinChatFinished");
            // 이미 열려 있는 ChatList.qml 창이 있다면 닫기 ~
            if (windowManager.getGlobalChatListWindow() !== null) {
                var existingWindow = windowManager.getGlobalChatListWindow();
                // existingWindow.close(); // UserList.qml 창 닫기
                // existingWindow.destroy();   // 메모리 정리
                // windowManager.setGlobalChatListWindow(null);    // 변수 초기화
                existingWindow.visible = false; // 창을 닫지 않고, 숨기기
            }
            // ~ 이미 열려 있는 ChatList.qml 창이 있다면 닫기
            // 이미 열려 있는 ChatRoom.qml 창이 있다면 닫기 ~
            if (windowManager.getGlobalChatRoomWindow() !== null) {
                console.log("if (windowManager.getGlobalChatRoomWindow() !== null)@@@@@@@@@");
                var existingChatRoomWindow = windowManager.getGlobalChatRoomWindow();
                existingChatRoomWindow.close(); // UserList.qml 창 닫기
                existingChatRoomWindow.destroy();   // 메모리 정리
                windowManager.setGlobalChatRoomWindow(null);    // 변수 초기화
            }
            // ~ 이미 열려 있는 ChatRoom.qml 창이 있다면 닫기
            // 클릭한 채팅방의, 채팅창 (ChatRoom.qml) 열기 ~
            var chatRoomComponent = Qt.createComponent("ChatRoom.qml");
            var chatRoomWindow = chatRoomComponent.createObject(null, {"chatTitle": chatTitle});
            windowManager.setGlobalChatRoomWindow(chatRoomWindow)
            // ~ 클릭한 채팅방의, 채팅창 (ChatRoom.qml)
        }
    }
    Rectangle {
        anchors.fill: parent
        color: "#f0f0f0"
        Text { // 상단에, 제목 텍스트 추가
            text: "현재 접속중인 클라이언트 목록"
            font.pixelSize: 24
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            color: "black"
            anchors.topMargin: 10
        }
        ListView {
            id: userListView
            anchors.top: parent.top
            anchors.topMargin: 50
            anchors.left: parent.left
            anchors.right: parent.right
            height: parent.height * 0.8
            model: userListModel
            delegate: Item {
                width: userListView.width
                height: 40
                Rectangle {
                    width: parent.width
                    height: parent.height
                    color: "#ffffff"
                    Text {
                        text: modelData
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                    }
                }
            }
        }
        Button {
                id: joinButton
                text: "Join"
                width: parent.width * 0.4
                anchors.top: userListView.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: 10
                onClicked: {
                    if (chatTitle.length > 0) {
                        chatManagementAPI.joinChat(chatTitle);
                    }
                }
            }
        Button {
            text: "프로그램 종료"
            width: parent.width * 0.4
            anchors.top: joinButton.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 20
            onClicked: {
                chatManagementAPI.leaveChat();
                // ChatList.qml 창 닫기 ~
                if (windowManager.getGlobalChatListWindow() !== null) {
                    console.log("\tUserList.qml 프로그램 종료 버튼 클릭 ( ChatList.qml 창 닫기 )");
                    var existingWindow = windowManager.getGlobalChatListWindow();
                    existingWindow.close(); // UserList.qml 창 닫기
                    existingWindow.destroy();   // 메모리 정리
                    windowManager.setGlobalChatListWindow(null);    // 변수 초기화
                }
                // ~ ChatList.qml 창 닫기
                // UserList.qml 창이 열려 있다면 닫기 ~
                if (windowManager.getGlobalUserListWindow() !== null) {
                    console.log("\tUserList.qml 프로그램 종료 버튼 클릭 ( UserList.qml 창 닫기 )");
                    var existingUserListWindow = windowManager.getGlobalUserListWindow();
                    existingUserListWindow.close(); // UserList.qml 창 닫기
                    existingUserListWindow.destroy();   // 메모리 정리
                    windowManager.setGlobalUserListWindow(null);    // 변수 초기화
                }
                // ~ UserList.qml 창이 열려 있다면 닫기
                // ChatRoom.qml 창이 열려 있다면 닫기 ~
                if (windowManager.getGlobalChatRoomWindow() !== null) {
                    console.log("\tUserList.qml 프로그램 종료 버튼 클릭 ( ChatRoom.qml 창 닫기 )");
                    var existingChatRoomWindow = windowManager.getGlobalChatRoomWindow();
                    existingChatRoomWindow.close(); // UserList.qml 창 닫기
                    existingChatRoomWindow.destroy();   // 메모리 정리
                    windowManager.setGlobalChatRoomWindow(null);    // 변수 초기화
                }
                // ~ ChatRoom.qml 창이 열려 있다면 닫기
            }
        }
    }
}
