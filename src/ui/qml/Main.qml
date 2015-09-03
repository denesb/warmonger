import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

Rectangle {
    width: 360
    height: 360

    StackView {
        id: mainStack
        anchors.fill: parent
        initialItem: mainMenu

        Component {
            id: mainMenu
            MainMenu {
                stack: mainStack
            }
        }
    }
}
