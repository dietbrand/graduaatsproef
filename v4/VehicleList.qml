import QtQuick
import QtQuick.Controls

import Vehicle 1.0

GridView {
    id: vehicleGrid
    anchors.fill: parent
    clip: true
    model: VehicleModel {
        list: vehicleList
    }
    cellWidth: parent.width * 0.2
    cellHeight: 20

    property string vehicleVin
    property string vehicleLP
    property string vehicleBM
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
                    text: "Chassisnummer: "
                }
                Label {
                    id: modalvin
                    text: vehicleVin
                }
            }
            Row {
                Label {
                    text: "Nummerplaat: "
                }
                Label {
                    id: modalLicensePlate
                    text: vehicleLP
                }
            }
            Row {
                Label {
                    text: "Merk en model: "
                }
                Label {
                    id: modalBrandModel
                    text: vehicleBM
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
        implicitWidth: vehicleGrid.cellWidth
        height: vehicleGrid.cellHeight
        Rectangle {
            anchors.fill: parent
            color: "#fafafa"
            Text {
                id: vehicleLicensePlate
                text: model.licensePlate
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: vehicleLicensePlate.color =  "#2397d3"
                onExited: vehicleLicensePlate.color = "#000000"
                onClicked: {
                    dialog.open();
                    dialog.title = model.brandModel + " " + model.licensePlate;
                    vehicleVin = model.vin;
                    vehicleLP = model.licensePlate;
                    vehicleBM = model.brandModel;
                    vehicleDriver = model.driver;
                }
            }
        }
    }
}
