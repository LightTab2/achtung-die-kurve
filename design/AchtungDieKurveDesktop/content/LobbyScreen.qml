import QtQuick 6.5
import QtQuick.Controls 6.5
import QtCharts
import QtQuick.Layouts
//import LobbyTableModel

Rectangle {
    id: rectangle
    width: 1920 //parent.width
    height: 1080 //parent.height

    Image {
        id: logo
        width: parent.height * 0.15
        height: parent.height * 0.15
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: parent.height * 0.012
        source: "qrc:/qt/qml/achtung-die-kurve-qml/icon/icon_256x256.png"
        fillMode: Image.PreserveAspectFit
    }

    TextField {
        id: name
        anchors.top: logo.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: parent.height * 0.012
        placeholderText: qsTr("Text Field")
    }

    Text {
        id: lobbyTitle
        anchors.top: logo.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: parent.height * 0.012
        text: qsTr("Lobbies:")
        font.family: "Segoe UI"
        font.pixelSize: 48
        rotation: 0
    }

    TableView {
        id: gridView
        anchors.top: lobbyTitle.bottom
        contentHeight: 70
        pixelAligned: true
        maximumFlickVelocity: 1200
        flickDeceleration: 600
        boundsBehavior: Flickable.StopAtBounds
        clip: true
        transformOrigin: Item.Center
        rotation: 0
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: parent.height * 0.012
        width: 200
        height: 200
        activeFocusOnTab: true
        focus: true
        //model: TableModel {}

        delegate: Rectangle {
            implicitWidth: 100
            implicitHeight: 50
            Text {
                text: display
            }
        }
        /*delegate: Item {
            id: item1
            height: 40
            Row {
                id: row
                Rectangle {
                    id: coulor
                    width: 40
                    height: 40
                    color: colorCode
                    z: 1
                }

                Text {
                    id: title
                    y: (parent.height - this.height)/2
                    text: name
                    font.bold: true
                }

                Button {
                    id: join
                    text: qsTr("Join")
                }
                spacing: 5
            }
        }*/
    }
}
