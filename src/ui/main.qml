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
            id: buttonHold
            text: qsTr("On Hold")
            Layout.fillWidth: true
        }

        GridLayout {
            columns: 2
            columnSpacing: defaultSpacing
            rowSpacing: defaultSpacing
            Layout.fillWidth: true

            Button {
                id: buttonMute
                text: qsTr("Mute")
                Layout.fillWidth: true
            }

            Button {
                id: buttonSpeaker
                text: qsTr("Speakers On/Off")
                Layout.fillWidth: true
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
    }

    Component.onCompleted: {
        buttonCall.clicked.connect(handler.call)
        buttonAnswer.clicked.connect(handler.answer)
        buttonHold.clicked.connect(handler.hold)
        buttonMute.clicked.connect(handler.mute)
        buttonSpeaker.clicked.connect(handler.speakerOnOff)
        handler.messageLogged.connect(textArea.append)
    }
}
