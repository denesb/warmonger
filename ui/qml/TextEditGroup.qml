import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

Rectangle {
    id: textEditGroup

    height: 40

    property string label
    property string value

    signal valueEdited(string val)

    Label {
        id: labelField
        height: 15
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        color: "black"
        text: textEditGroup.label
    }

    TextField {
        id: editField
        height: 25
        anchors {
            top: labelField.bottom
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

        text: textEditGroup.value

        onEditingFinished: textEditGroup.valueEdited(editField.text)
    }
}

