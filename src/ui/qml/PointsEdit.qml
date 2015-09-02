import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

Rectangle {
    id: pointsEdit

    height: 40

    property string pointsName
    property string maxPoints
    property string points

    signal pointsEdited(string val)

    Label {
        id: pointsNameLabel
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        color: "black"
        text: pointsName + "(/" + maxPoints + "): "
    }

    TextField {
        id: pointsEditField
        height: 25
        anchors {
            top: pointsNameLabel.bottom
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

        text: points

        onEditingFinished: pointsEdit.pointsChanged(pointsEditField.text)
    }
}

