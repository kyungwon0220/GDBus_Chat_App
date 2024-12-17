import QtQuick 2.15
import QtQuick.Controls 2.15
import App.ChatMessenger
Window {
    id: roomWindow
    width: 640
    height: 720
    visible: true
    title: qsTr("Chat Room")
    property string chatTitle: ""  // ChatRoom.qml에서 전달받을 채팅방 제목
    property ListModel messageModel: ListModel {}
    property var chatRoomWindow: null // 전역 변수로 Room 창 참조를 저장 ( ChatRoom.qml 창의 열고 닫는 등의 조작을 위해 )
    RoomManager { id: roomManager }  // C++ 객체 연결 (필수)
    Connections {
        target: roomManager
        function onNewMessageReceived(userName, message) {
            messageModel.append({ sender: userName, text: message });
        }
        function onUserTyping(userName) {
            console.log(userName + " is typing...")
            typingIndicator.text = userName + " is typing..."
            typingTimer.restart()  // 타이머를 재시작하여 2초 후에 문구를 비움
        }
        function onUserJoined(userName, chatTitle) {
            console.log(userName + " has joined the chat.");
            messageModel.append({ sender: "System", text: userName + " has joined the chat." });
        }
        function onUserLeft(userName, chatTitle) {
            console.log(userName + " has left the chat.");
            messageModel.append({ sender: "System", text: userName + " has left the chat." });
        }
    }
    Timer {
            id: typingTimer
            interval: 2000  // 2초 후에 실행
            repeat: false
            onTriggered: {
                typingIndicator.text = ""  // 문구를 비움
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
            height: parent.height * 0.8
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
            anchors.top: messageDisplay.bottom
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.topMargin: 10
            TextField {
                id: messageInput
                placeholderText: "Enter your message"
                width: parent.width * 0.8
                onTextChanged: {
                    if (messageInput.text.length > 0) {
                        roomManager.notifyTyping()  // 서버에 입력 중임을 알림
                    }
                }
            }
            Button {
                text: "Send"
                width: parent.width * 0.15
                onClicked: {
                    if (messageInput.text.length > 0) {
                        roomManager.sendMessage(messageInput.text) // 서버에 메시지 전송 요청
                        messageInput.text = ""
                        typingIndicator.text = "" // 메시지를 보낸 후 입력 상태 표시 초기화
                    }
                }
            }
        }
        // 방 나가기 버튼
        Button {
            id: exitButton
            text: "채팅 나가기"
            width: parent.width * 0.2
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: 10
            onClicked: {
                roomManager.leaveChat();  // 서버에게 LeaveChat 요청
                roomWindow.close();                                                                                                         // 현재 Room.qml 창 닫기
                if (typeof chatRoomWindow !== 'undefined' && chatRoomWindow !== null) { chatRoomWindow.close(); chatRoomWindow = null; }    // ChatRoom.qml 창 닫기 (이미 열려 있다면)
                // ChatList.qml 창 다시 표시
                var chatListComponent = Qt.createComponent("ChatList.qml");
                var chatListWindow = chatListComponent.createObject(null);
                chatListWindow.visible = true;
            }
        }
    }
}