import QtQuick 2.0

Rectangle {
    signal pawnDropped(int actorId, int idx)
    signal wallDropped(int actorId, int orientation, int row, int col)

    property list<WallStock> wallStocks

    property var playerList: ({})
    property var wallStockList: ({})
    property int activeActorId: -1
    property int playerCurrentActivity: 0
    property int playerNum: 0
    property bool isRunning: false

    function init() {
        board.clear();
        start();
    }

    function addPlayer(actorId, nodeIdx, possibleMoves, walls) {
        playerList[actorId] = 1;
        wallStockList[actorId] = wallStocks[playerNum];
        playerNum += 1;

        board.addPawn(actorId);
        board.setPawnPos(actorId, nodeIdx, possibleMoves);
        wallStocks[playerNum].actorId = actorId;
    }

    function setPawnPos(actorId, nodeIdx, possibleMoves) {
        board.setPawnPos(actorId, nodeIdx, possibleMoves);
    }

    function setPawnPossibleMoves(actorId, possibleMoves) {
        board.setPawnPossibleMoves(actorId, possibleMoves);
    }

    function setWall(actorId, orientation, row, col) {
        board.setWall(actorId, orientation, row, col);
    }

    function setActivePlayer(actorId) {
        if (!isRunning || !playerList.hasOwnProperty(actorId)) {
            return;
        }
        board.setActivePlayer(actorId);
        wallStockList[actorId].isActive = true;

        activeActorId = actorId;
        playerCurrentActivity = 0;
    }

    function switchPlayerActivity() {
        playerCurrentActivity = 1 - playerCurrentActivity;
        board.switchPlayerActivity(activeActorId, playerCurrentActivity);
    }

    function start() {
        isRunning = true;
    }

    function stop() {
        isRunning = false;
    }

    function endGame() {
        board.clear();
        stop();
    }

    function finishGame() {
        board.finishGame();
    }

    function clear() {
        playerList = {};
        wallStockList = {};
        playerNum = 0;
    }


    Board {
        id: board
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

    Component.onCompleted: {
        wallStocks = [wallStock_1, wallStock_2, wallStock_3, wallStock_4];
    }
}

