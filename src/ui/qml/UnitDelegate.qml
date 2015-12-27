import QtQuick 2.2

Rectangle {
    id: unit
    height: 48

    Row {
        anchors.fill: parent

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
