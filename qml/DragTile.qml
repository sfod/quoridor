import QtQuick 2.0

Item {
    id: root

    property int actorId: -1
    property Item initParent: root

    width: (parent.width - (parent.columns - 1) * parent.spacing) / parent.columns
    height: (parent.height - (parent.rows - 1) * parent.spacing) / parent.rows

    function setParent(p) {
        mouseArea.parent = p
    }

    function setPossibleMoves(moves) {
        pawn.possibleMoves = moves
    }

    function setDragging(b) {
        pawn.isDraggingEnabled = b
    }

    MouseArea {
        id: mouseArea

        width: parent.width
        height: parent.height
        anchors.centerIn: parent
        propagateComposedEvents: true

        drag.target: pawn.isDraggingEnabled ? pawn : null
        onReleased: {
            if (pawn.isDraggingEnabled && (pawn.Drag.target !== null) && pawn.allowed(pawn.Drag.target.rIdx)) {
                console.log("released on drop area " + pawn.Drag.target.rIdx)
                parent = pawn.Drag.target
                pawn.curParent = parent
                pawn.Drag.drop()
            }
        }

        Rectangle {
            id: pawn
            color: "transparent"

            function allowed(idx) {
                return possibleMoves.hasOwnProperty(idx);
            }

            property int actorId: -1
            property var possibleMoves: new Object()
            property bool isDraggingEnabled: true
            property Item curParent: root

            width: parent.width
            height: parent.height
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter

            Drag.active: mouseArea.drag.active
            Drag.hotSpot.x: parent.width / 2
            Drag.hotSpot.y: parent.height / 2

            Image {
                source: "qrc:/resources/pawn.png"
                anchors.fill: parent
            }

            states: State {
                when: mouseArea.drag.active
                ParentChange { target: pawn; parent: root }
                AnchorChanges {target: pawn; anchors.verticalCenter: undefined; anchors.horizontalCenter: undefined}
            }

            Component.onCompleted: {
                mouseArea.parent = root.initParent
                pawn.actorId = root.actorId
            }

            Component.onDestruction: {
                console.log("destroying pawn")
            }
        }
    }
}
