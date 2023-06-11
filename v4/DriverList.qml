import QtQuick
import QtQuick.Controls

import Driver 1.0

GridView {
    id: driversGrid
    anchors.fill: parent
    clip: true
    model: DriverModel {
        list: driverList
    }
    cellWidth: parent.width * 0.2
    cellHeight: 20

    property string fullName : ""

    Dialog {
        id: dialog
        modal: true
        anchors.centerIn: parent
        standardButtons: Dialog.Open | Dialog.Close
        Column {
            Row {
                Label {
                    text: "Volledige naam: "
                }
                Label {
                    id: modalDriverName
                    text: fullName
                }
            }
            Row {
                Label {
                    text: "Geboortedatum: "
                }
                Label {
                    id: modalDriverBirthdate
                }
            }
            Row {
                Label {
                    text: "Rijksregisternummer: "
                }
                Label {
                    id: modalDriverRRN
                }
            }
        }
        onAccepted: editDialog.open()
    }

    Dialog {
        id: editDialog
        modal: true
        anchors.centerIn: parent
        standardButtons: Dialog.Save | Dialog.Cancel
        Column {
            Label {
                text: "Bewerk de gegevens"
            }
            Row {
                Label {
                    text: "Volledige naam: "
                }
                TextField {
                    id: editDialogfirstName
                    text: fullName
                }
            }
        }
    }

    delegate: Item {
        implicitWidth: driversGrid.cellWidth
        height: driversGrid.cellHeight
        Rectangle {
            anchors.fill: parent
            color: "#fafafa"
            Text {
                id: driverName
                text: model.fullname
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: driverName.color =  "#2397d3"
                onExited: driverName.color = "#000000"
                onClicked: {
                    dialog.open();
                    dialog.title = model.fullname;
                    fullName = model.fullname;
                    modalDriverBirthdate.text = Qt.formatDateTime(model.birthDate,"dd-MMMM-yyyy");
                    modalDriverRRN.text = model.rrn;
                }
            }
        }
    }
}
