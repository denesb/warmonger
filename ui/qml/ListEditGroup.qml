import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

Rectangle {
    id: listEditGroup

    height: 40

    property string label
    property var model
    property int currentIndex

    signal currentItemChanged(int index)

    function find(val) {
        return editField.find(val);
    }

    Label {
        id: labelField
        height: 15
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        color: "black"
        text: listEditGroup.label
    }

    ComboBox {
        id: editField
        height: 25
        anchors {
            top: labelField.bottom
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

        model: listEditGroup.model;
        textRole: "displayName"

        currentIndex: listEditGroup.currentIndex

        onActivated: currentItemChanged(index)
    }
}
