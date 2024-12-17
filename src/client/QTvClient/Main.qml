import QtQuick 2.15
import QtQuick.Controls 2.15
// import QtQuick.Dialogs 1.3 // 불필요로 삭제 예정
Window {
    id: mainWindow              // 최초의, user_name 등록 창
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello Client")
    property string errMsg: ""  // emit registrationFailed(QString::fromStdString(ex.what())) (main.cpp)
    Connections {
        target: Client
        function onRegistrationFailed(errorMessage) {   // emit registrationFailed(QString::fromStdString(ex.what())) (main.cpp)
        errMsg = errorMessage;                          // Window 속성 ( errMsg ) 최신화
        registrationFailedDialog.open();                // 경고창 표시 (Dialog 사용)
        }
        function onRegistrationSucceeded() {                            // emit registrationSucceeded() (main.cpp)
            registrationFailedDialog.close();
            mainWindow.close();
            // ChatList.qml 창 띄우기 ( Main.qml 역할 종료 )
            var chatListComponent = Qt.createComponent("ChatList.qml"); // createObject() 사용 위해, QML 파일을 로드하여 컴포넌트(템플릿)를 생성
            var chatListWindow = chatListComponent.createObject(null);  // ' .visible ' 사용해서 창을 띄우기 위해, 객체로 생성
            chatListWindow.visible = true;
        }
    }
    TextField {
        id: userNameInput
        width: parent.width * 0.8
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 20
        placeholderText: "Enter your user name"
    }
    Button {
        id: startButton
        text: "대화명 사용하기"
        width: parent.width * 0.4
        anchors.top: userNameInput.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 10
        onClicked: {
            if (userNameInput.text.length > 0) {
                Client.registerUser(userNameInput.text)
            }
        }
    }
    Dialog {    // 등록 실패시 경고창 Dialog
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
            registrationFailedDialog.visible = false
            userNameInput.text = ""
        }
    }
}