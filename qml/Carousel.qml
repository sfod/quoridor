import QtQuick 2.0

Item {
    id: root
    width: 140
    height: 20

    signal valueChanged(var idx, var value);

    property var values: []
    property int idx: -1

    onValuesChanged: {
        carouselModel.clear();
        for (var i in values) {
            carouselModel.append({"name": values[i]});
        }
    }

    Row {
        width: parent.width
        height: parent.height
        spacing: 1

        Rectangle {
            width: parent.height
            height: parent.height

            Text {
                text: "<"
                anchors.fill: parent
                font.pixelSize: parent.height
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    carousel.decrementCurrentIndex();
                }
            }
        }

        Rectangle {
            width: parent.width - parent.height * 2
            height: parent.height

            Component {
                id: delegate
                Item {
                    id: wrapper
                    property string value: nameText.text
                    anchors.fill: parent
                    Text {
                        id: nameText
                        text: name

                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter

                        font.pointSize: 16
                        color: "black"
                        visible: wrapper.PathView.isCurrentItem ? true : false
                    }
                }
            }

            PathView {
                id: carousel
                property var curItem

                onCurrentItemChanged: {
                    if (carousel.currentItem !== curItem) {
                        valueChanged(root.idx, carousel.currentItem.value);
                        curItem = carousel.currentItem;
                    }
                }

                anchors.fill: parent
                model: ListModel {
                    id: carouselModel
                }

                delegate: delegate
                path: Path {
                    startX: 0
                    startY: 0
                    PathLine { relativeX: 0; relativeY: 0 }
                }

                Component.onCompleted: {
                    carousel.curItem = carousel.currentItem;
                }
            }
        }

        Rectangle {
            width: parent.height
            height: parent.height

            Text {
                text: ">"
                anchors.fill: parent
                font.pixelSize: 20
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    carousel.incrementCurrentIndex()
                }
            }
        }
    }
}
