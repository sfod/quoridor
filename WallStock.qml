import QtQuick 2.0

Rectangle {
    color: "sienna"

    property bool isActive: false
    property int actorId: -1

    MouseArea {
        id: pawnWallSwitcherMouseArea
        anchors.fill: parent
        onClicked: {
            if (isActive) {
                console.log("clicked switch " + actorId);
                board.switchPlayerActivity();
            }
        }
    }
}
