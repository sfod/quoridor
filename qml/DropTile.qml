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
        Image {
            id: cell
            anchors.fill: parent
            source: "qrc:/resources/cell_empty.png"
        }

        states: [
            State {
                when: dragTarget.containsDrag && drag.source.allowed(index)
                PropertyChanges {
                    target: cell
                    source: "qrc:/resources/cell_hovered_empty.png"
                }
            }
        ]
    }
}
