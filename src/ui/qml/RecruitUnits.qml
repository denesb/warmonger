import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import Warmonger 1.0

Rectangle {
    id: root

    property var settlement
    signal finished()

    width: 600
    height: 600

    Rectangle {
        id: titleBar
        height: 24

        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
        }

        Text {
            text: "Recruit units"
        }
    }

    Rectangle {
        anchors {
            top: titleBar.bottom
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        UnitTypeSelector {
            id: unitTypeSelector

            width: 300

            anchors {
                top: parent.top
                bottom: parent.bottom
                left: parent.left
            }

            unitTypes: settlement.recruits
        }

        Rectangle {
            anchors {
                top: parent.top
                bottom: parent.bottom
                left: unitTypeSelector.right
                right: parent.right
            }

            ColumnLayout {
                anchors.fill: parent

                Rectangle {
                    id: unitRecruitWrapper

                    Layout.preferredHeight: W.world.surface.tileSize.height * 3/2
                    Layout.fillWidth: true

                    border {
                        width: 1
                        color: "black"
                    }

                    UnitRecruit {
                        id: unitRecruit

                        anchors.fill: parent
                        anchors.margins: 1

                        game: W.game
                        settlement: root.settlement
                        unitType: unitTypeSelector.unitType
                    }
                }

                UnitTypeDetails {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    unitType: unitTypeSelector.unitType
                }

                Row {
                    height: 30

                    Button {
                        text: "Recruit"

                        enabled: unitRecruit.canRecruit

                        Connections {
                            onClicked: unitRecruit.recruitUnit()
                        }
                    }

                    Button {
                        text: "Finish"

                        onClicked: {
                            root.finished()
                        }
                    }
                }
            }
        }
    }
}
