import QtQuick 2.15
import QtQuick.Controls 2.15
import App.ChatMessenger
Window {
    id: chatListWindow
    width: 640
    height: 720
    visible: true
    title: qsTr("Chat List")
    ChatListManager { id: chatListManager }
    ChatRoomManager { id: chatRoomManager }                     // 새로운 채팅방 생성시, 자동 입장 위해 선언
    property var chatListModel: []                              // 외부에서 모델을 받을 수 있도록 속성을 정의
    property string errMsg: ""                                  // emit createChatFailed(QString::fromStdString(ex.what())) (chatlist.cpp)
    Component.onCompleted: { chatListManager.fetchChatList() }  // 최초로 창이 띄워질 때, 서버의 GetChatList() 메서드 ( 창이 화면에 완전히 표시되었을 때 )
    Connections {
        target: chatListManager
        function onChatListFetched(chatList) {      // emit chatListFetched(qmlChatList) (chatlist.cpp)
            chatListModel = chatList;               // 서버로부터 수신한, 채팅방 목록 (chatList)
        }
        function onCreateChatFailed(errorMessage) { // emit createChatFailed(QString::fromStdString(ex.what())) (chatlist.cpp)
            errMsg = errorMessage;                  // Window 속성 ( errMsg ) 최신화
            createChatFailedDialog.open();          // 경고창 표시 (Dialog 사용)
        }
        function onChatJoined(chatTitle) {          // emit chatJoined(QString::fromStdString(createdChatTitle.raw())) (chatlist.cpp)
            chatListWindow.close();
            // 새로 생성된 채팅방의, 접속자 목록 창 (ChatRoom.qml) ~
            var chatRoomComponent = Qt.createComponent("ChatRoom.qml");
            var chatRoomWindow = chatRoomComponent.createObject(null, {"chatTitle": chatTitle});
            chatRoomWindow.visible = true;
            // ~ 새로 생성된 채팅방의 접속자 목록 창 (ChatRoom.qml)
            // 새로 생성된 채팅방에 자동 입장 처리 (Room.qml) ~
            chatRoomManager.joinChat(chatTitle);
            var roomComponent = Qt.createComponent("Room.qml");
            var roomWindow = roomComponent.createObject(null, {"chatRoomWindow": null, "chatTitle": chatTitle});
            roomWindow.visible = true;
            // ~ 새로 생성된 채팅방 자동 입장 처리 (Room.qml)
        }
    }
    Rectangle {
        anchors.fill: parent
        color: "#f0f0f0"
        Text {  // 상단에 제목 텍스트
            text: "채팅방 목록"
            font.pixelSize: 24
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            color: "black"
            anchors.topMargin: 10
        }
        ListView {
            id: chatListView
            anchors.top: parent.top
            anchors.topMargin: 50   // 제목 텍스트 아래로 여백 추가
            anchors.left: parent.left
            anchors.right: parent.right
            height: parent.height * 0.8
            model: chatListWindow.chatListModel
            delegate: Item {
                width: chatListView.width
                anchors.topMargin: 5
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
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            createChatFailedDialog.visible = false;
                            chatListWindow.close();
                            // 클릭한 채팅방의, 접속자 목록 창 (ChatRoom.qml) ~
                            var chatRoomComponent = Qt.createComponent("ChatRoom.qml");
                            var chatRoomWindow = chatRoomComponent.createObject(null, {"chatTitle": modelData});
                            chatRoomWindow.visible = true;
                             // ~ 클릭한 채팅방의, 접속자 목록 창 (ChatRoom.qml)
                        }
                    }
                }
            }
        }
        Dialog {    // 생성하고자는 채팅방명을 입력받을 Dialog
            id: createChatDialog
            width: 300
            height: 150
            title: "Create New Chat"
            standardButtons: Dialog.Ok | Dialog.Cancel
            anchors.centerIn: parent
            contentItem: Column {
                TextField {
                    id: chatNameInput
                    placeholderText: "Enter chat title"
                    width: parent.width
                }
            }
            onAccepted: {
                if (chatNameInput.text.length > 0) {
                    chatListManager.createChat(chatNameInput.text);
                }
                chatNameInput.text = ""; // 입력 필드 초기화
            }
        }
        Dialog {    // 경고 창 Dialog
            id: createChatFailedDialog
            width: 300
            height: 150
            title: "Error"
            standardButtons: Dialog.Ok
            anchors.centerIn: parent
            contentItem: Text {
                text: errMsg + "\n채팅방 생성 실패.";
            }
            onAccepted: {
                createChatFailedDialog.visible = false
            }
        }
        Button {
            id: createChatButton
            width: parent.width * 0.4
            anchors.top: chatListView.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 10
            contentItem: Text {
                text: "Create New Chat"
                color: "black"
                font.pixelSize: 16
                anchors.centerIn: parent
                horizontalAlignment: Text.AlignHCenter  // 텍스트를 가로로 중앙 정렬
            }
            onClicked: {
                createChatDialog.open();
            }
        }
        Button {
            contentItem: Text {
                text: "프로그램 종료"
                color: "black"
                font.pixelSize: 16
                anchors.centerIn: parent
                horizontalAlignment: Text.AlignHCenter  // 텍스트를 가로로 중앙 정렬
            }
            width: parent.width * 0.4
            anchors.top: createChatButton.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 10
            onClicked: {
                chatListWindow.close();
            }
        }
    }
}