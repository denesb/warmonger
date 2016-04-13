import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

Rectangle {
    id: mainMenu

    property var stack

    Column {
        anchors.centerIn: parent

        spacing: 4

        Button {
            width: 128
            text: "New Game"
            onClicked: {
                W.loadMaps();
                mainMenu.stack.push({
                    item: Qt.createComponent("NewGame.qml"),
                    properties: {stack: stack}
                });
            }
            enabled: false
        }

        Button {
            width: 128
            text: "Saved Game"
            enabled: false
        }

        Button {
            width: 128
            text: "Map Editor"

            onClicked: {
                mainMenu.stack.push({
                    item: Qt.createComponent("MapEditor.qml"),
                    properties: {stack: stack}
                });
            }
        }

        Button {
            width: 128
            text: "Exit"

            onClicked: Qt.quit()
        }
    }
}
