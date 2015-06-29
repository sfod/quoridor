import QtQuick 2.0

Rectangle {
    id: boardFrame
    objectName: "boardFrame"

    property var playerList: ({})
    property int activeActorId: -1

    function addPlayer(actorId) {
        board.addPawn(actorId);
    }

    function setPawnPos(actorId, idx, possibleMoves) {
    }

    function setPawnPossibleMoves(actorId, possibleMoves) {
    }

    function setWall(actorId, orientation, row, col) {
    }

    function setActivePlayer(actorId) {
    }

    function switchPlayerActivity() {
    }

    function finishGame() {
    }

    function endGame() {
    }

    Board {
        id: board
        objectName: "board"
        width: boardFrame.width - 80
        height: boardFrame.height - 80
        anchors.horizontalCenter: boardFrame.horizontalCenter
        anchors.verticalCenter: boardFrame.verticalCenter
    }

    WallStock {
        id: wallStock_1
        actorId: 1
        width: board.width / 2
        height: 30
        anchors.top: board.bottom
        anchors.horizontalCenter: board.horizontalCenter
    }

    WallStock {
        id: wallStock_2
        actorId: 2
        width: board.width / 2
        height: 30
        anchors.bottom: board.top
        anchors.horizontalCenter: board.horizontalCenter
    }

    WallStock {
        id: wallStock_3
        actorId: 3
        width: 30
        height: board.height / 2
        anchors.right: board.left
        anchors.verticalCenter: board.verticalCenter
    }

    WallStock {
        id: wallStock_4
        actorId: 4
        width: 30
        height: board.height / 2
        anchors.left: board.right
        anchors.verticalCenter: board.verticalCenter
    }
}

