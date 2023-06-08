import QtQuick

Item {
    id: root
    width: 100
    height: 100
    property color color: "#ffffff"
    property color originalColor: "#00d4ff"
    property color colorClicked: "#2397d3"
    property string title: "A button"
    property alias area: buttonMouseArea

    Rectangle {
        anchors.fill: parent
        color: color

        Text {
            id: display
            text: root.title
            anchors.centerIn: parent

            color: root.isActive ? "#2397d3" : root.originalColor
            fontSizeMode: Text.Fit
            font.pixelSize: root.height * 0.7
        }

        MouseArea {
            id: buttonMouseArea
            anchors.fill: parent
            onPressed: display.color = root.colorClicked
            onReleased: display.color = root.originalColor
        }
    }
}
