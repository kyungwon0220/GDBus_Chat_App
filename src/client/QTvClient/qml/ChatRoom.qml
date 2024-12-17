import QtQuick 2.15
import QtQuick.Controls 2.15
Window {
    width: 640
    height: 720
    visible: true
    title: qsTr(chatTitle + "Chat Room")
    property string chatTitle: ""                   // 전달받을 채팅방 제목
    property ListModel messageModel: ListModel {}   // 메시지 모델
    Connections {
        target: chatManagementAPI
        function onLeaveChatFinished(updatedChatList) {
            // UserList.qml 창이 열려 있다면 닫기 ~
            if (windowManager.getGlobalUserListWindow() !== null) {
                console.log("\tChatRoom.qml 프로그램 종료 버튼 클릭 ( UserList.qml 창 닫기 )");
                var existingUserListWindow = windowManager.getGlobalUserListWindow();
                existingUserListWindow.close(); // UserList.qml 창 닫기
                existingUserListWindow.destroy();   // 메모리 정리
                windowManager.setGlobalUserListWindow(null);    // 변수 초기화
            }
            // ~ UserList.qml 창이 열려 있다면 닫기
            // ChatRoom.qml 창 닫기 ~
            if (windowManager.getGlobalChatRoomWindow() !== null) {
                console.log("\tChatRoom.qml 프로그램 종료 버튼 클릭 ( ChatRoom.qml 창 닫기 )");
                var existingChatRoomWindow = windowManager.getGlobalChatRoomWindow();
                existingChatRoomWindow.close(); // UserList.qml 창 닫기
                existingChatRoomWindow.destroy();   // 메모리 정리
                windowManager.setGlobalChatRoomWindow(null);    // 변수 초기화
            }
            // ~ ChatRoom.qml 창 닫기
            windowManager.getGlobalChatListWindow().chatListModel = updatedChatList;
            windowManager.getGlobalChatListWindow().visible = true;
        }
    //     function onNewMessageReceived(userName, message) {
    //         messageModel.append({ sender: userName, text: message });
    //     }
    //     function onUserTyping(userName) {
    //         console.log(userName + " is typing...");
    //         typingIndicator.text = userName + " is typing...";
    //         typingTimer.restart(); // 타이머를 재시작하여 2초 후에 문구를 비움
    //     }
    }
    Timer {
        id: typingTimer
        interval: 2000 // 2초 후에 실행
        repeat: false
        onTriggered: {
            typingIndicator.text = ""; // 문구를 비움
        }
    }
    Rectangle {
        anchors.fill: parent
        color: "#f0f0f0"
        // 메시지 표시 영역
        Rectangle {
            id: messageDisplay
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: parent.height * 0.75
            color: "#ffffff"
            border.color: "#000000"
            border.width: 1
            ListView {
                id: messageListView
                anchors.fill: parent
                model: messageModel
                delegate: Column {
                    width: parent.width
                    Text {
                        text: model.sender + ": " + model.text
                        font.pixelSize: 16
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        color: "black"
                    }
                }
            }
        }
        // 사용자 입력 상태 표시
        Text {
            id: typingIndicator
            text: ""
            anchors.top: messageDisplay.bottom
            anchors.left: parent.left
            anchors.leftMargin: 10
            font.pixelSize: 14
            color: "gray"
            height: 20
        }
        // 메시지 입력 및 전송 버튼
        Row {
            id: messageInputRow
            width: parent.width
            height: 40
            anchors.top: typingIndicator.bottom // messageDisplay.bottom
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.topMargin: 50
            TextField {
                id: messageInput
                placeholderText: "Enter your message"
                width: parent.width * 0.8
                onTextChanged: {
                    if (messageInput.text.length > 0) {
                        roomManager.notifyTyping(); // 서버에 입력 중임을 알림
                    }
                }
            }
            Button {
                text: "Send"
                width: parent.width * 0.15
                onClicked: {
                    if (messageInput.text.length > 0) {
                        roomManager.sendMessage(messageInput.text); // 서버에 메시지 전송 요청
                        messageInput.text = "";
                        typingIndicator.text = ""; // 메시지를 보낸 후 입력 상태 표시 초기화
                    }
                }
            }
        }
        Button {
            text: "Exit Chat Room"
            width: parent.width * 0.4 // 0.2
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: 10
            onClicked: {
                chatManagementAPI.leaveChat();
            }
        }
    }
}
