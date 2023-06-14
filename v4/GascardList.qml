import QtQuick
import QtQuick.Controls

import Gascard 1.0

GridView {
    id: gascardGrid
    anchors.fill: parent
    clip: true
    model: GascardModel {
        list: gascardList
    }
    cellWidth: parent.width * 0.2
    cellHeight: 20

    property string gascardNumber
    property bool gascardBlocked: false;
    property int gascardPincode
    property int vehicleDriver
    property string vehicleDriverName

    Dialog {
        id: dialog
        modal: true
        anchors.centerIn: parent
        standardButtons: Dialog.Open | Dialog.Close
        Column {
            Row {
                Label {
                    text: "Kaartnummer: "
                }
                Label {
                    id: modalGascardNumber
                    text: gascardNumber
                }
            }
            Row {
                Label {
                    text: "Geblokkeerd: "
                }
                CheckBox {
                    id: modalBlocked
                    checked: gascardBlocked
                }
            }
            Row {
                Label {
                    text: "Pincode: "
                }
                Label {
                    id: modalpincode
                    text: gascardPincode
                }
            }
        }
        onAccepted: {
            vehicleDriverName = vehicleList.fetchDriverForVehicle(vehicleDriver);
            editDialog.open();
        }
    }

    Dialog {
        id: editDialog
        modal: true
        anchors.centerIn: parent
        standardButtons: Dialog.Save | Dialog.Cancel
        title: "Bestuurder"
        Column {
            Row {
                Label {
                    text: "Bestuurder: "
                }
                TextField {
                    id: editDialogBestuurder
                    text: vehicleDriverName
                }
            }
        }
    }

    delegate: Item {
        implicitWidth: gascardGrid.cellWidth
        height: gascardGrid.cellHeight
        Rectangle {
            anchors.fill: parent
            color: "#fafafa"
            Text {
                id: cardNumber
                text: model.cardNumber
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: cardNumber.color =  "#2397d3"
                onExited: cardNumber.color = "#000000"
                onClicked: {
                    dialog.open();
                    dialog.title = model.cardNumber;
                    gascardNumber = model.cardNumber
                    gascardBlocked = model.blocked;
                    gascardPincode = model.pincode;
                    vehicleDriver = model.driver;
                }
            }
        }
    }
}
