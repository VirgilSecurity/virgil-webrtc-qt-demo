import QtQuick 2.0
import QtQuick.Controls 2.12

Button {
    id: root
    property var buttonAction: undefined
    visible: buttonAction.visible
    enabled: buttonAction.enabled

    Component.onCompleted: clicked.connect(buttonAction.trigger)
    onCheckedChanged: buttonAction.checked = checked

    Connections {
        target: buttonAction
        function onCheckedChanged(checked) {
            root.checked = checked
        }
    }
}
