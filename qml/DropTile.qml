import QtQuick 2.0

DropArea {
    id: dragTarget

    property alias dropProxy: dragTarget
    property int rIdx: index

    width: (parent.width - (parent.columns - 1) * parent.spacing) / parent.columns
    height: (parent.height - (parent.rows - 1) * parent.spacing) / parent.rows

    onDropped: {
        boardFrame.pawnDropped(drag.source.actorId, index)
    }

    Rectangle {
        id: dropRectangle

        anchors.fill: parent
        color: "#FFCE9E"

        states: [
            State {
                when: dragTarget.containsDrag && drag.source.allowed(index)
                PropertyChanges {
                    target: dropRectangle
                    color: "lightblue"
                }
            }
        ]
    }
}
