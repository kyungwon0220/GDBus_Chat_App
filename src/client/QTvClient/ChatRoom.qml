import QtQuick 2.15
import QtQuick.Controls 2.15
import App.ChatMessenger
Window {
    id: chatRoomWindow
    width: 320
    height: 720
    visible: true
    title: qsTr("Chat Room")
    property string chatTitle: ""                                       // ChatList.qml에서 전달받을 채팅방 제목
    property var userListModel: []                                      // 외부에서 모델을 받을 수 있도록 속성을 정의
    property var roomWindow: null                                       // 전역 변수로 Room 창 참조를 저장 ( Room.qml 창의 열고 닫는 등의 조작을 위해 )
    ChatRoomManager { id: chatRoomManager }
    Component.onCompleted: { chatRoomManager.fetchUserList(chatTitle) } // 최초에, 창이 켜지면, 서버에게 userList 요청
    Connections {
        target: chatRoomManager
        function onUserListFetched(userList) {  // emit userListFetched(qmlUserList) (chatroom.cpp)
            userListModel = userList;           // 서버로부터 수신한, userList
        }
        function onUserJoined(chatTitle) {      // fetchUserList(QString::fromStdString(chatTitle.raw())) (chatroom.cpp)
            chatRoomManager.fetchUserList(chatTitle);
        }
        function onUserLeft(chatTitle) {        // fetchUserList(QString::fromStdString(chatTitle.raw())) (chatroom.cpp)
            chatRoomManager.fetchUserList(chatTitle);
        }
    }
    Rectangle {
        anchors.fill: parent
        color: "#f0f0f0"
        Text {  // 상단에, 제목 텍스트 추가
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
            anchors.topMargin: 50   // 제목 텍스트 아래로 여백 추가
            anchors.left: parent.left
            anchors.right: parent.right
            height: parent.height * 0.8
            model: userListModel
            delegate: Text {
                text: modelData
                font.pixelSize: 20
                anchors.left: parent.left
                anchors.leftMargin: 10
                color: "black"
            }
        }
        Button {
            id: joinButton
            text: "Join"
            anchors.top: userListView.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 10
            onClicked: {
                chatRoomManager.joinChat(chatTitle);
                // 채팅창 띄우기 ( Room.qml ) ~
                var roomComponent = Qt.createComponent("Room.qml");
                roomWindow = roomComponent.createObject(null, {"chatRoomWindow": chatRoomWindow});
                roomWindow.visible = true;
                // ~ 채팅창 띄우기 ( Room.qml )
            }
        }
        Button {
            text: "채팅 나가기"
            anchors.top: joinButton.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 10
            onClicked: {
                chatRoomManager.leaveChat();
                chatRoomWindow.close();
                if (typeof roomWindow !== 'undefined' && roomWindow !== null) { roomWindow.close(); roomWindow = null; } // Room.qml 창 닫기 (이미 열려 있다면)
                chatListWindow.visible = true;
            }
        }
    }
}