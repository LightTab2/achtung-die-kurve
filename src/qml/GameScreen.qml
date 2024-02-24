import achtung_die_kurve
import QtQuick
import QtQuick.Layouts

RowLayout 
{
    id: gameLayout
    anchors.fill: parent
    spacing: 6
    GameCanvas 
    {
        id: gameCanvas
        objectName: "gameCanvas"
        Layout.alignment: Qt.AlignHCenter
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.minimumWidth: 256
        Layout.minimumHeight: 256
        Layout.maximumWidth: boardSize
        Layout.maximumHeight: boardSize
    }
    Rectangle 
    {
        id: leaderboardBackground
        color: "lightgray"
        Layout.fillWidth: true
        Layout.fillHeight: true
        ColumnLayout
        {
            width: leaderboardBackground.width
            height: leaderboardBackground.height
            Text {
                id: leaderboardHeaderText
                text: "Leaderboard"
                font.pixelSize: Math.max(28, Math.min(leaderboardBackground.width, leaderboardBackground.height)/6)
                Layout.alignment: Qt.AlignHCenter
            }
            ListView {
                id: leaderboard
                objectName: "leaderboard"
                model: LeaderboardModel
                interactive: false
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: Math.max(12, Math.min(leaderboard.width, leaderboard.height)/16) * 1.2
                delegate: Item
                {
                    required property string name
                    required property string score
                    required property color textColor
                    Row 
                    {
                        Text {
                            text: score + " "
                            id: scoreText
                            font.pixelSize: Math.max(12, Math.min(leaderboard.width, leaderboard.height)/16)
                        }
                        Text {
                            text: name
                            id: nameText
                            font.pixelSize: Math.max(12, Math.min(leaderboard.width, leaderboard.height)/16)
                            color: textColor
                        }
                    }
                }
            }
        }
    }
}
