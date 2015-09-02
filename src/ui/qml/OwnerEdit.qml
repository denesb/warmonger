import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

Rectangle {
    id: ownerEdit

    height: 40

    property string entityName
    property var ownersModel
    property int ownerIndex

    signal ownerEdited(int index)

    function find(val) {
        return ownerEditField.find(val);
    }

    Label {
        id: ownerLabel
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        color: "black"
        text: entityName + " Owner:"
    }

    ComboBox {
        id: ownerEditField
        height: 25
        anchors {
            top: ownerLabel.bottom
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

        model: ownersModel;
        textRole: "displayName"

        currentIndex: ownerIndex

        onActivated: ownerEdited(ownerEditField.currentIndex)
    }
}
