import achtung_die_kurve
import QtQuick
import QtQuick.Controls

MainWindow {
    visible: true
    width: 1920
    height: 1080
    title: qsTr("Achtung Die Kurve")
    color: "#111111"

    StackView {
        id: stack
        initialItem: gameScreen
        anchors.fill: parent
    }

    GameScreen {
        id: gameScreen
    }

    Dialog
    {
        id: exceptionMessage
        objectName: "exceptionMessage"
        standardButtons: Dialog.Ok
        modal: true

        function showMessage(message: string)
        {
           exceptionMessage.title = message;
           exceptionMessage.open();
        }

        footer: DialogButtonBox {
            alignment: Qt.AlignHCenter
        }
    }
}