Rectangle {
    anchors.fill: parent
    color: "#646464"

    Rectangle {
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: 10
        width: 100
        radius: 5
        border.width: 1
        border.color: "#353535"

        ListView {
            anchors.fill: parent

            model: worldListModel
            delegate: Rectangle {
                color: red
                radius: 5
                Text {
                    anchors.fill: parent
                    anchors.margins: 5
                    text: displayName
                }
            }
        }
    }
}

