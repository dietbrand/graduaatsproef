import QtQuick
import QtQuick.Window

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    Rectangle {
        width: 100
        height: 65
        color: "#f0f0f0"
        anchors.centerIn: parent
        Text {
            anchors.centerIn: parent
            text: "Hello QML"
        }
    }
}
