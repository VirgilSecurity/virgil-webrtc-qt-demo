import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

ApplicationWindow {
    id: root
    visible: true
    width: 480
    height: 640
    title: qsTr("WebRTC Qt Demo")

    readonly property int defaultSpacing: 10

    ColumnLayout {
        spacing: defaultSpacing
        anchors {
            fill: parent
            margins: defaultSpacing
        }

        Rectangle {
            color: "#ffffff"
            radius: 5
            border.color: "lightgray"
            border.width: 1
            Layout.fillWidth: true
            Layout.fillHeight: true

            ScrollView {
                anchors.fill: parent

                TextArea {
                    id: textArea
                    readOnly: true
                    color: "#000000"
                    placeholderText: "Log"
                    padding: defaultSpacing
                }
            }
        }

        Button {
            id: buttonCall
            text: qsTr("Call")
            Layout.fillWidth: true
        }

        Button {
            id: buttonAnswer
            text: qsTr("Answer")
            Layout.fillWidth: true
        }
    }

    Component.onCompleted: {
        buttonCall.clicked.connect(handler.call)
        buttonAnswer.clicked.connect(handler.answer)
        handler.messageLogged.connect(textArea.append)
    }
}
