import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

Rectangle {
    id: mapNodeEdit
    property var mapNode

    onMapNodeChanged: {
        var index = -1;
        if (mapNodeEdit.mapNode) {
            index = terrainTypeEdit.find(mapNodeEdit.mapNode.terrainType.displayName);
        }
        terrainTypeEdit.currentIndex = index;
    }

    Image {
        id: picture
        width: 64
        height: {
            var tileSize = W.map.world.surface.tileSize;
            64 * tileSize.height/tileSize.width;
        }
        anchors {
            top: parent.top
            left: parent.left
            margins: 2
        }
    
        source: {
            var surface = W.map.world.surface;
            if (mapNodeEdit.mapNode) {
                var terrainTypeName = mapNodeEdit.mapNode.terrainType.objectName;
                surface.prefix + surface.bigMap[terrainTypeName];
            } else {
                "";
            }
        }
    }

    Rectangle {
        anchors {
            top: parent.top
            bottom: parent.bottom
            left: picture.right
            right: parent.right

            leftMargin: 5
            rightMargin: 5
        }

        Label {
            id: objectNameLabel
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
                topMargin: 10
            }
            color: "black"
            text: {
                var label = "Map Node ";
                if (mapNodeEdit.mapNode) {
                    label += mapNodeEdit.mapNode.objectName;
                }
                label;
            }
        }

        Label {
            id: displayNameLabel
            anchors {
                top: objectNameLabel.bottom
                left: parent.left
                right: parent.right
                topMargin: 10
            }
            color: "black"
            text: "Display Name:"
        }

        TextField {
            id: displayNameEdit
            height: 25
            anchors {
                top: displayNameLabel.bottom
                left: parent.left
                right: parent.right
            }
            style: TextFieldStyle {
                textColor: "black"
                background: Rectangle {
                    radius: 5
                    border.color: "#333"
                    border.width: 1
                }
            }

            text: {
                if (mapNodeEdit.mapNode) {
                    mapNodeEdit.mapNode.displayName
                } else {
                    ''
                }
            }

            onEditingFinished: {
                if (mapNodeEdit.mapNode) {
                    mapNodeEdit.mapNode.displayName = displayNameEdit.text;
                }
            }
        }

        Label {
            id: terrainTypeLabel
            anchors {
                top: displayNameEdit.bottom
                left: parent.left
                right: parent.right
                topMargin: 10
            }
            color: "black"
            text: "Terrain Type:"
        }

        ComboBox {
            id: terrainTypeEdit
            height: 25
            anchors {
                top: terrainTypeLabel.bottom
                left: parent.left
                right: parent.right
            }
            style: ComboBoxStyle {
                background: Rectangle {
                    radius: 5
                    border.color: "#333"
                    border.width: 1
                }
            }

            model: W.map.world.terrainTypes
            textRole: "displayName"

            onActivated: {
                if (mapNodeEdit.mapNode) {
                    var terrainTypes = W.map.world.terrainTypes;
                    var terrainType = terrainTypes[index];

                    mapNodeEdit.mapNode.terrainType = terrainType;
                }
            }
        }
    }
}
