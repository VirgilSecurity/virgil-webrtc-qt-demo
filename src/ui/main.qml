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

        RowLayout {
            spacing: defaultSpacing
            Layout.fillWidth: true

            ActionButton {
                buttonAction: handler.muteAction
                checkable: true
                text: checked ? qsTr("Unmute") : qsTr("Mute")
                Layout.fillWidth: true
            }

            ActionButton {
                buttonAction: handler.muteVoiceAction
                checkable: true
                text: checked ? qsTr("Unmute Voice") : qsTr("Mute Voice")
                Layout.fillWidth: true
            }

            ActionButton {
                buttonAction: handler.speakerAction
                checkable: true
                text: checked ? qsTr("Speaker Off") : qsTr("Speaker On")
                Layout.fillWidth: true
            }

        }

        RowLayout {
            spacing: defaultSpacing
            Layout.fillWidth: true

            ActionButton {
                buttonAction: handler.holdAction
                checkable: true
                text: checked ? qsTr("Resume") : qsTr("On Hold")
                Layout.fillWidth: true
            }
        }


        RowLayout {
            spacing: defaultSpacing
            Layout.fillWidth: true

            ActionButton {
                buttonAction: handler.callAction
                text: qsTr("Call")
                Layout.fillWidth: true
            }

            ActionButton {
                buttonAction: handler.answerAction
                text: qsTr("Answer")
                Layout.fillWidth: true
            }

            ActionButton {
                buttonAction: handler.endAction
                text: qsTr("End")
                Layout.fillWidth: true
            }
        }
    }

    Component.onCompleted: {
        handler.messageLogged.connect(textArea.append)
    }
}
