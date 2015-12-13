import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

Rectangle {
    id: root

    property var settlement
    property var stack

    Button {
        id: backButton

        height: 24
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
        }

        text: "<<"

        Connections {
            onClicked: root.stack.pop()
        }
    }

    Rectangle {
        id: settlementInfo

        height: 24
        anchors {
            left: parent.left
            right: parent.right
            top: backButton.bottom
        }

        Text {
            anchors.verticalCenter: parent.verticalCenter

            text: {
                var dn = root.settlement.displayName;
                if (dn != "")
                    text += dn + " ";

                text += root.settlement.type.displayName;
                text += " - " + root.settlement.owner.displayName;
                text;
            }
        }
    }

    Component {
        id: unitDelegate

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
                        surface.prefix + surface.imagePaths[imageName];
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
        id: recruits

        anchors {
            left: parent.left
            right: parent.right
            top: settlementInfo.bottom
            bottom: parent.bottom
        }

        model: root.settlement.type.recruits
        delegate: unitDelegate
    }
}
