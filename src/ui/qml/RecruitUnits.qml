import QtQuick 2.2
import QtQuick.Layouts 1.1
import Warmonger 1.0

Rectangle {
    id: root

    property var settlement

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

        Component {
            id: unitDelegate

            UnitDelegate {
            }
        }

        ListView {
            id: recruitList

            width: 300

            anchors {
                top: parent.top
                bottom: parent.bottom
                left: parent.left
            }

            model: settlement.type.recruits
            delegate: unitDelegate
        }

        Rectangle {
            anchors {
                top: parent.top
                bottom: parent.bottom
                left: recruitList.right
                right: parent.right
            }

            Rectangle {
                id: mapFragment
                height: W.world.surface.tileSize.height * 3/2

                anchors {
                    top: parent.top
                    left: parent.left
                    right: parent.right
                }
                border {
                    width: 1
                    color: "black"
                }

                UnitRecruit {
                    anchors.fill: parent
                    anchors.margins: 1

                    game: W.game
                    settlement: root.settlement
                }
            }

            Rectangle {
                id: details

                anchors {
                    top: mapFragment.bottom
                    bottom: parent.bottom
                    left: parent.left
                    right: parent.right
                }
                color: "yellow"
            }
        }
    }
}
