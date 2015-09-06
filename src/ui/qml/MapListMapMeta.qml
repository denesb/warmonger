import QtQuick 2.2

Rectangle {
    width: 500
    height: 48
    border {
        width: 1
        color: ListView.isCurrentItem ? "orange" : "black"
    }
    anchors {
        verticalCenter: parent.verticalCenter
        horizontalCenter: parent.horizontalCenter
    }

    Text {
        anchors {
            left: parent.left
            margins: 10
            verticalCenter: parent.verticalCenter
        }
        color: ListView.isCurrentItem ? "orange" : "black"
        text: {
            model.modelData.displayName + " (" + model.modelData.world.displayName + ")";
        }
    }

    Text {
        anchors {
            right: parent.right
            margins: 10
            verticalCenter: parent.verticalCenter
        }
        text: {
            var text = "";
            text += model.modelData.mapNodes.length + " nodes, ";
            text += model.modelData.players.length + " players";
            text;
        }
    }
}
