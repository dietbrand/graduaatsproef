import QtQuick

Item {
    id: root
    width: 100
    height: 100
    property color color: "#ffffff"
    property string title: "A button"
    property alias area: buttonMouseArea

    Rectangle {
        anchors.fill: parent
        color: "#00d4ff"
        radius: 10

        Text {
            id: display
            text: root.title
            anchors.centerIn: parent
            color: "#ffffff"
            fontSizeMode: Text.Fit
            font.pixelSize: root.height * 0.7
        }

        MouseArea {
            id: buttonMouseArea
            anchors.fill: parent
            hoverEnabled: true
            onEntered: parent.color =  "#2397d3"
            onExited: parent.color = "#00d4ff"
        }
    }
}
