import QtQuick 2.2

Rectangle {
    id: root

    property var points
    property var maxPoints
    property var color

    border.width: 1
    border.color: "black"

    Rectangle {
        id: inner

        anchors {
            fill: parent
            margins: 1
        }

        Text {
            z: 1
            anchors {
                verticalCenter: parent.verticalCenter
                horizontalCenter: parent.horizontalCenter
            }

            text: {
                root.points + "/" + root.maxPoints;
            }
        }

        Rectangle {
            id: bar

            anchors {
                left: parent.left
                top: parent.top
                bottom: parent.bottom
            }
            width: {
                var proportion = root.points/root.maxPoints;

                if (proportion < 0) proportion = 0;
                if (proportion > 1) proportion = 1;

                inner.width * proportion;
            }
            color: root.color
        }
    }
}
