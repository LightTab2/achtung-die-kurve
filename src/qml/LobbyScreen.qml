import QtQuick 6.5
import QtQuick.Controls 6.5
   
Rectangle {
    id: rectangle
    width: parent.width
    height: parent.height
    opacity: 0

    Rectangle {
        id: logo
        width: parent.width * 0.95
        height: parent.height * 0.15
        color: "#a20000"
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: parent.height * 0.012
        Image {
            id: image
            width: parent.width
            height: parent.height
            source: "qrc:/qt/qml/achtung-die-kurve-qml/icon/icon_256x256.png"
        }
    }

    Text {
        id: title
        text: qsTr("List of games")
        anchors.centerIn: parent
        font.family: "Segoe UI"
        font.pixelSize: 16
        anchors.verticalCenterOffset: -370
        anchors.horizontalCenterOffset: 0
    }

    Button {
        id: button1
        x: 378
        y: 607
        text: qsTr("Join")
    }

    ListView {
        id: listView
        x: 366
        y: 215
        width: 346
        height: 644
        model: ListModel {
            ListElement {
                name: "Grey"
                colorCode: "grey"
            }

            ListElement {
                name: "Red"
                colorCode: "red"
            }

            ListElement {
                name: "Blue"
                colorCode: "blue"
            }

            ListElement {
                name: "Green"
                colorCode: "green"
            }
        }
        delegate: Item {
            x: 5
            width: 80
            height: 40
            Row {
                id: row1
                Rectangle {
                    width: 40
                    height: 40
                    color: colorCode
                }

                Text {
                    text: name
                    anchors.verticalCenter: parent.verticalCenter
                    font.bold: true
                }
                spacing: 10
            }
        }
    }
}