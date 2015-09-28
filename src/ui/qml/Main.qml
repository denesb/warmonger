import QtQuick 2.2
import QtQuick.Controls 1.1

Rectangle {
    width: 1024
    height: 700

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
