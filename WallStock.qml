import QtQuick 2.0

Rectangle {
    color: "sienna"

    property bool isActive: false

    MouseArea {
        id: pawnWallSwitcherMouseArea
        anchors.fill: parent
        onClicked: {
            console.log("pawn/wall switcher clicked");
            board.switchPlayerActivity();
        }
    }
}
