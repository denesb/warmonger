import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

Item {
    id: root

    width: 800
    height: 600

    Rectangle {
        id: main_menu
        anchors.fill: parent

        Rectangle {
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 100

            Text {
                anchors.top: parent.top
                
                text: "Warmonger, game engine v" + engine.version
            }

            Text {
                text: "1) " + engine.worlds[0].displayName
            }
        }
    }
}
