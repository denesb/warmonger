import QtQuick 2.2

Rectangle {
    id: root

    property var unitTypes;

    property var unitType : unitTypes[0];

    Component {
        id: unitDelegate

        Rectangle {
            id: unit
            height: 48
            anchors.left: parent.left
            anchors.right: parent.right

            color: unit.ListView.isCurrentItem ? "orange" : "white"

            MouseArea {
                id: mouseArea
                anchors.fill: parent

                onClicked: {
                    unit.ListView.view.currentIndex = index
                }
            }

            Row {
                anchors.fill: parent
                anchors.margins: 3

                Image {
                    width: 48
                    height: 48
                    source: {
                        var imageName = model.modelData.objectName;
                        var surface = W.world.surface;
                        surface.imagePaths[imageName];
                    }

                    fillMode: Image.PreserveAspectFit
                }

                Text {
                    id: unitText

                    anchors {
                        verticalCenter: parent.verticalCenter
                    }

                    text: {
                        var text = "";

                        text += model.modelData.displayName;
                        text += " [" + model.modelData.klass.displayName + "]";
                        text += "\n";
                        text += "Level " + model.modelData.level.index;
                        text += " (" + model.modelData.level.displayName + ")";
                        text;
                    }
                }
            }
        }
    }

    ListView {
        anchors.fill: parent

        currentIndex: 0

        model: root.unitTypes
        delegate: unitDelegate

        onCurrentIndexChanged: {
            root.unitType = root.unitTypes[currentIndex];
        }
    }

}
