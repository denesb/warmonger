import QtQuick 2.2

Rectangle {
    id: root

    anchors.fill: parent

    Rectangle {
        id: menuBar

        color: "red"

        height: 40
        anchors {
            top: parent.top
            left: parent.left
            right: sideBar.left
        }
    }

    Rectangle {
        id: sideBar

        color: "yellow"

        width: 300
        anchors {
            top: parent.top
            bottom: parent.bottom
            right: parent.right
        }
    }

    Rectangle {
        id: statusBar

        color: "green"

        height: 20
        anchors {
            bottom: parent.bottom
            left: parent.left
            right: sideBar.left
        }
    }

    Rectangle {
        id: mainWidget

        color: "blue"

        anchors {
            top: menuBar.bottom
            bottom: statusBar.top
            left: parent.left
            right: sideBar.left
        }
    }
}
