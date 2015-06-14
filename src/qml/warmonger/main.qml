import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

Rectangle {
    width: 1024
    height: 800
    color: "yellow"

    RowLayout {
        anchors.fill: parent

        Rectangle {
            Layout.preferredWidth: parent.width - 200
            Layout.fillHeight: true
            color: "green"

            ColumnLayout {
                anchors.fill: parent

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 20
                    color: "red"
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: parent.height - 20
                    color: "red"
                }
            }
        }

        Rectangle {
            Layout.preferredWidth: 200
            Layout.fillHeight: true
            color: "blue"
        }
    }
}
