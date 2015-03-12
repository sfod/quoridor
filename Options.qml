import QtQuick 2.0
import QtQuick.Layouts 1.1

Item {
    id: options

    property var playerNums: []
    property var playerTypes: []

    property var numsList: []
    property var typesList: []

    property var selectedPlayers: []

    signal playersChanged(variant players);

    function setOptions(num) {
        selectedPlayers = [];
        for (var i = 0; i < num; ++i) {
            var component = Qt.createComponent("Carousel.qml");
            if (component.status === Component.Error) {
                console.error("error creating carousel: " + component.errorString());
            }

            var types = component.createObject(playersGrid, { idx: i, values: playerTypes });
            types.valueChanged.connect(changeType);
            typesList.push(types);

            selectedPlayers.push(playerTypes[0]);
        }
        playersChanged(selectedPlayers);
    }

    function changeNum(idx, n) {
        clearTypes();
        setOptions(n);
    }

    function changeType(idx, type) {
        selectedPlayers[idx] = type;
        playersChanged(selectedPlayers);
    }

    // @todo check for available player types
    function setPlayerNums(nums) {
        if (numsList.length > 0) {
            clearNums();
        }

        playerNums = nums;

        var component = Qt.createComponent("Carousel.qml");
        if (component.status === Component.Error) {
            console.error("error creating carousel: " + component.errorString());
        }

        var n = component.createObject(playersGrid, { values: playerNums });
        numsList.push(n);

        n.valueChanged.connect(changeNum);

        setOptions(playerNums[0]);
    }

    function setPlayerTypes(types) {
        playerTypes = types;
    }

    function clearNums() {
        clearTypes();

        for (var i in numsList) {
            numsList[i].destroy();
        }
        numsList = [];
    }

    function clearTypes() {
        for (var i in typesList) {
            typesList[i].destroy();
        }
        typesList = [];
    }


    ColumnLayout {
        id: playersGrid
        anchors.fill: parent

        spacing: 20
    }
}
