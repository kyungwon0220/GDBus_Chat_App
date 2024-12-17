import QtQuick 2.15
import QtQuick.Controls 2.15
// import QtQuick.Dialogs 1.3 // 삭제 예정
Window {
    id: mainWindow              // 최초의, user_name 등록 창
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello Client")
    property string errMsg: ""  // 서버의 'RegisterUser()' 실패시, ERR Message
        onClosing: { // 창이 닫힐 때의 이벤트 처리
            // ChatList.qml 창이 열려 있다면 닫기 ~
            if (windowManager.getGlobalChatListWindow() !== null) {
                console.log("\tUserList.qml 프로그램 종료 버튼 클릭 ( ChatList.qml 창 닫기 )");
                var existingWindow = windowManager.getGlobalChatListWindow();
                existingWindow.close(); // UserList.qml 창 닫기
                existingWindow.destroy();   // 메모리 정리
                windowManager.setGlobalChatListWindow(null);    // 변수 초기화
            }
            // ~ ChatList.qml 창이 열려 있다면 닫기
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
        function onRegisterUserFinished(qmlChatList) { // onRegisterUserFinish() emit registerUserFinished(qmlChatList);
            registrationFailedDialog.close();
            mainWindow.close();
            // ChatList.qml 창 띄우기 ( Main.qml 역할 종료 ) ~
            if (windowManager.getGlobalChatListWindow() === null) { // globalChatListWindow가 비어 있는 경우 새로 생성
                console.log("if (windowManager.getGlobalChatListWindow() === null)");
                var chatListComponent = Qt.createComponent("ChatList.qml"); // createObject() 사용 위해, QML 파일을 로드하여 컴포넌트(템플릿)를 생성
                var chatListWindow = chatListComponent.createObject(null, {"chatListModel": qmlChatList});  // 속성으로 채팅방 목록 전달하면서, 객체 생성
                if (!chatListWindow) {
                    console.error("Failed to create ChatListWindow.");
                } else {
                    console.log("ChatListWindow created successfully:", chatListWindow);
                    windowManager.setGlobalChatListWindow(chatListWindow);  // 창 객체 저장
                }
            }
            // ~ ChatList.qml 창 띄우기 ( Main.qml 역할 종료 )
        }
        function onRegisterUserFailed(errorMessage) {   // onRegisterUserFinish() emit registerUserFailed(QString::fromStdString(ex.what()));
            errMsg = errorMessage;                      // Window 속성 ( property string errMsg ) 최신화
            registrationFailedDialog.open();            // Dialog 사용하여, 경고창 표시
        }
    }
    TextField { // TextField: 사용하고자 하는 대화명 입력 필드
        id: userNameInput
        width: parent.width * 0.8                           // 가로 사이즈
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter   // 가로로 가운데 정렬
        placeholderText: "사용을 원하는 대화명 입력"
        horizontalAlignment: Text.AlignHCenter              // placeholderText 가로로 중앙 정렬
    }
    Button {
        contentItem: Text {
            text: "대화명 사용하기"
            horizontalAlignment: Text.AlignHCenter  // Text 가로로 중앙 정렬
            color: "black"
        }
        width: parent.width * 0.4
        anchors.top: userNameInput.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 10
        onClicked: {
            if (userNameInput.text.length > 0) {
                chatManagementAPI.registerUser(userNameInput.text);
            }
        }
    }
    Dialog {    // 등록 실패 시 경고창 Dialog ( function onRegisterUserFailed(errorMessage) 사용 )
        id: registrationFailedDialog
        width: 300
        height: 150
        title: "Error"
        standardButtons: Dialog.Ok
        anchors.centerIn: parent
        contentItem: Text {
            text: errMsg + "\n대화명 등록 실패."
        }
        onAccepted: {
            registrationFailedDialog.close();
            userNameInput.text = "";
            errMsg = "";
        }
    }
}
