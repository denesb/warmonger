import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

Rectangle {
    id: gameplay
    anchors.fill: parent

    Rectangle {
        id: status_bar
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: sideBar.left
        height: 20
        color: "green"
    }

    Rectangle {
        id: game_map
        anchors.left: parent.left
        anchors.top: status_bar.bottom
        anchors.right: side_bar.left
        anchors.bottom: parent.bottom
        color: "red"
    }

    Rectangle {
        id: side_bar
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        width: 200
        color: "blue"
    }
}
