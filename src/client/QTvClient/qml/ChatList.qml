import QtQuick 2.15
import QtQuick.Controls 2.15
Window {
    width: 640
    height: 720
    visible: true
    title: qsTr("Chat List")
    property var chatListModel: []  // Main.qml에서 전달받을 채팅방 목록
    property string errMsg: ""      // 서버의 'CreateChat()' 실패 시, ERR Message
    Connections {
        target: chatManagementAPI
        function onChatListUpdated(updatedChatList) {
            if(windowManager.getGlobalChatListWindow() !== null && windowManager.getGlobalChatListWindow().visible === true) {
                if (updatedChatList.length === 0) {
                    chatListModel = [];
                } else {
                    chatListModel = updatedChatList;
                }
            }
        }
        function onCreateChatFinished(createdChatTitle) {   // onCreateChatFinish() emit emit createChatFinished(QString::fromStdString(createdChatTitle.raw()));
            chatManagementAPI.getUserList(createdChatTitle);
            chatManagementAPI.joinChat(createdChatTitle);
        }
        function onCreateChatFailed(errorMessage) {       // onCreateChatFinish() emit createChatFailed(QString::fromStdString(ex.what()));
            errMsg = errorMessage;                        // Window 속성 ( property string errMsg ) 최신화
            createChatFailedDialog.open();                // Dialog 사용하여, 경고창 표시
        }
        function onGetUserListFinished(chatTitle, qmlUserList) { // onGetUserListFinish() emit getUserListFinished(QString::fromStdString(chatTitle.raw()), qmlUserList);
            if (windowManager.getGlobalUserListWindow() !== null) {
                if (qmlUserList.length === 0) {
                    // UserList.qml 창이 열려 있다면 닫기 ~
                    if (windowManager.getGlobalUserListWindow() !== null) {
                        console.log("onGetUserListFinished() (windowManager.getGlobalUserListWindow() === null) if (qmlUserList.length === 0)");
                        var existingUserListWindow = windowManager.getGlobalUserListWindow();
                        existingUserListWindow.close(); // UserList.qml 창 닫기
                        existingUserListWindow.destroy();   // 메모리 정리
                        windowManager.setGlobalUserListWindow(null);    // 변수 초기화
                    }
                    // ~ UserList.qml 창이 열려 있다면 닫기
                } else {
                    console.log("onGetUserListFinished() if (windowManager.getGlobalUserListWindow() !== null) if (qmlUserList.length !== 0)");
                    var existingWindow = windowManager.getGlobalUserListWindow();
                    existingWindow.chatTitle = chatTitle;
                    existingWindow.userListModel = qmlUserList;
                }
            } else {
                if (qmlUserList.length !== 0) {
                    console.log("onGetUserListFinished() if (windowManager.getGlobalUserListWindow() === null) if (qmlUserList.length !== 0)");
                    // 클릭한 채팅방의, 접속자 목록 창 (UserList.qml) ~
                    var userListComponent = Qt.createComponent("UserList.qml");
                    var userListWindow = userListComponent.createObject(null, {
                        "chatTitle": chatTitle,
                        "userListModel": qmlUserList
                    });
                    if (userListWindow) {
                        windowManager.setGlobalUserListWindow(userListWindow);
                    }
                    // ~ 클릭한 채팅방의, 접속자 목록 창 (UserList.qml)
                }
            }
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
            model: chatListModel
            delegate: Item {
                width: chatListView.width
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
                            createChatFailedDialog.close();
                            chatManagementAPI.getUserList(modelData);
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
                    chatManagementAPI.createChat(chatNameInput.text);
                    chatNameInput.text = "";    // 입력 필드 초기화
                }
            }
        }
        Dialog {    // 채팅방 생성 실패 시 경고창 Dialog
            id: createChatFailedDialog
            width: 300
            height: 150
            title: "Error"
            standardButtons: Dialog.Ok
            anchors.centerIn: parent
            contentItem: Text {
                text: errMsg + "\n채팅방 생성 실패."
            }
            onAccepted: {
                createChatFailedDialog.visible = false;
            }
        }
        Button {
            id: createChatButton
            width: parent.width * 0.4
            anchors.top: chatListView.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 20
            contentItem: Text {
                text: "Create New Chat"
                color: "black"
                font.pixelSize: 16
                anchors.centerIn: parent
                horizontalAlignment: Text.AlignHCenter  // 텍스트를 가로로 중앙 정렬
            }
            onClicked: {
                createChatDialog.open();    // 생성하고자는 채팅방명을 입력받을 Dialog 열기
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
                // ChatList.qml 창 닫기 ~
                if (windowManager.getGlobalChatListWindow() !== null) {
                    console.log("\tChatList.qml 프로그램 종료 버튼 클릭 ( ChatList.qml 창 닫기 )");
                    var existingWindow = windowManager.getGlobalChatListWindow();
                    existingWindow.close(); // UserList.qml 창 닫기
                    existingWindow.destroy();   // 메모리 정리
                    windowManager.setGlobalChatListWindow(null);    // 변수 초기화
                }
                // ~ ChatList.qml 창 닫기
                // UserList.qml 창이 열려 있다면 닫기 ~
                if (windowManager.getGlobalUserListWindow() !== null) {
                    console.log("\tChatList.qml 프로그램 종료 버튼 클릭 ( UserList.qml 창 닫기 )");
                    var existingUserListWindow = windowManager.getGlobalUserListWindow();
                    existingUserListWindow.close(); // UserList.qml 창 닫기
                    existingUserListWindow.destroy();   // 메모리 정리
                    windowManager.setGlobalUserListWindow(null);    // 변수 초기화
                }
                // ~ UserList.qml 창이 열려 있다면 닫기
                // ChatRoom.qml 창이 열려 있다면 닫기 ~
                if (windowManager.getGlobalChatRoomWindow() !== null) {
                    console.log("\tChatList.qml 프로그램 종료 버튼 클릭 ( ChatRoom.qml 창 닫기 )");
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
