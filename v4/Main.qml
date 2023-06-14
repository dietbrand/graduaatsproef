import QtQuick
import QtQuick.Window
import QtQuick.Layouts

Window {
    id: root
    width: 800
    height: 600
    visible: true
    title: qsTr("Stolos - Qt frontend")
    color: "#fefefe"

    ColumnLayout {
        spacing: 20
        implicitWidth: root.width
        anchors.horizontalCenter: parent.horizontalCenter
        Row {
            id: topMenu
            Layout.alignment: Qt.AlignHCenter
            height: root.height * 0.2
            CustomMenuItem {
                height: 20
                title: "Home"
                area.onClicked: {
                    heroSection.visible = true;
                    driverSection.visible = false;
                    vehicleSection.visible = false;
                }
            }
            CustomMenuItem {
                height: 20
                title: "Bestuurders"

                area.onClicked: {
                    heroSection.visible = false;
                    driverSection.visible = true;
                    vehicleSection.visible = false;
                    driverList.fetchDrivers();
                }
            }
            CustomMenuItem {
                height: 20
                title: "Voertuigen"
                area.onClicked: {
                    heroSection.visible = false;
                    driverSection.visible = false;
                    vehicleSection.visible = true;
                    vehicleList.fetchVehicles();
                }
            }
            CustomMenuItem {
                height: 20
                title: "Tankkaarten"
            }
        }
        Rectangle {
            visible: true
            id: heroSection
            implicitWidth: root.width * 0.8
            implicitHeight: root.height * 0.5

            color: "#fafafa"
            Layout.alignment: Qt.AlignHCenter
            radius: 10

            RowLayout {
                Image {
                    Layout.margins: 10
                    id: heroLogo
                    Layout.preferredHeight: heroSection.width * 0.2
                    Layout.preferredWidth: heroSection.width * 0.2
                    source: "logoshort.png"
                    RotationAnimation on rotation {
                        loops: Animation.Infinite
                        from: 0
                        to: 360
                        duration: 10000
                    }
                }
                ColumnLayout {

                    Rectangle {
                        Layout.preferredHeight: heroSection.height * 0.2
                        Layout.preferredWidth: heroSection.width * 0.7
                        Layout.margins: 5
                        color: heroSection.color
                        Text {
                            anchors.fill: parent
                            id: heroSectionTitle
                            text: qsTr("AllPhi Fleet Management")
                            font.pixelSize: 40
                            horizontalAlignment: Text.AlignRight
                        }
                    }
                    Rectangle {
                        Layout.preferredHeight: heroSection.height * 0.2
                        Layout.preferredWidth: heroSection.width * 0.7
                        Layout.margins: 5
                        color: heroSection.color
                        Text {
                            anchors.fill: parent
                            id: heroSectionSubTitle
                            text: qsTr("Klik op een knop om te starten")
                            font.pixelSize: 32
                            horizontalAlignment: Text.AlignRight
                        }
                    }
                }
            }
        }
        Rectangle {
            id: driverSection
            visible: false
            implicitWidth: root.width * 0.8
            implicitHeight: root.height * 0.5

            color: "#fafafa"
            Layout.alignment: Qt.AlignHCenter
            radius: 10

            DriverList{}

        }
        Rectangle {
            id: vehicleSection
            visible: false
            implicitWidth: root.width * 0.8
            implicitHeight: root.height * 0.5

            color: "#fafafa"
            Layout.alignment: Qt.AlignHCenter
            radius: 10

            VehicleList{}

        }
        Row {
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
            height: root.height * 0.2
            id: bottomMenu
            spacing: 6
            CustomButton {
                height: 40
                width: 150
                title: "Home"
                area.onClicked: {
                    heroSection.visible = true;
                    driverSection.visible = false;
                }
            }
            CustomButton {
                height: 40
                width: 150
                title: "Bestuurders"
                area.onClicked: {
                    heroSection.visible = false;
                    driverSection.visible = true;
                    driverList.fetchDriverData()
                }
            }
            CustomButton {
                height: 40
                width: 150
                title: "Voertuigen"
            }
            CustomButton {
                height: 40
                width: 150
                title: "Tankkaarten"
            }
        }
    }
}


