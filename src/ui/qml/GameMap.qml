import QtQuick 2.2
import Warmonger 1.0

Rectangle {
    id: root

    property var game

    GameMap {
        id: map
        anchors.fill: parent

        game: root.game
    }
}
