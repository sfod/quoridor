import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

ApplicationWindow {
    id: windowApp

    visible: true
    width: 800
    height: 600
    title: qsTr("Quoridor")

    Rectangle {
        id: windowMain
        state: "stateMainMenu"

        anchors.fill: parent

        // main menu window
        Item {
            id: windowMainMenu
            objectName: "mainMenu"

            anchors.fill: parent

            RowLayout {
                height: parent.height / 2
                width: parent.width
                anchors.bottom: parent.bottom

                Item {
                    anchors.left: parent.left
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Button {
                        id: buttonNewGame
                        objectName: "buttonNewGame"
                        text: "New Game"
                        anchors.centerIn: parent

                        // @fixme change state from C++
                        onClicked: {
                            windowMain.state = "stateOptions"
                        }
                    }
                }

                Item {
                    anchors.right: parent.right
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Button {
                        id: buttonQuit
                        objectName: "buttonQuit"
                        text: "Quit"
                        anchors.centerIn: parent
                    }
                }
            }
        }

        // options window
        Item {
            id: windowOptions

            anchors.fill: parent

            Options {
                id: options
                objectName: "options"
            }

            RowLayout {
                height: parent.height / 2
                width: parent.width
                anchors.bottom: parent.bottom

                Item {
                    anchors.left: parent.left
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Button {
                        id: buttonStartGame
                        objectName: "buttonStartGame"
                        text: "Start Game"
                        anchors.centerIn: parent

                        // @fixme change state from C++
                        onClicked: {
                            windowMain.state = "stateGame"
                        }
                    }
                }

                Item {
                    anchors.right: parent.right
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Button {
                        id: buttonBackToMainMenu
                        objectName: "buttonBackToMainMenu"
                        text: "Back"
                        anchors.centerIn: parent

                        // @fixme change state from C++
                        onClicked: {
                            windowMain.state = "stateMainMenu"
                        }
                    }
                }
            }
        }

        // game window
        Item {
            id: windowGame

            anchors.fill: parent

            BoardFrame {
                id: boardFrame
                objectName: "boardFrame"

                width: windowApp.width - 300
                height: windowApp.height - 60
                x: 0
                y: 0
            }

            TextArea {
                id: movesRecorder
                objectName: "moveRecorder"

                anchors.left: boardFrame.right

                text: ""
                readOnly: true

                function init() {
                    movesRecorder.text = "";
                }

                function addMessage(message) {
                    movesRecorder.append(message);
                }
            }

            Item {
                id: buttonsArea

                width: parent.width / 2
                anchors.top: movesRecorder.bottom
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.left: boardFrame.right

                Button {
                    id: buttonBackToOptions
                    objectName: "buttonBackToOptions"
                    text: "Back"

                    anchors.verticalCenter: parent.verticalCenter

                    // @fixme change state from C++
                    onClicked: {
                        boardFrame.endGame()
                        windowMain.state = "stateOptions"
                    }
                }
            }
        }

        states: [
            State {
                name: "stateMainMenu"
                PropertyChanges {
                    target: windowMainMenu
                    visible: true
                }
                PropertyChanges {
                    target: windowOptions
                    visible: false
                }
                PropertyChanges {
                    target: windowGame
                    visible: false
                }
            },
            State {
                name: "stateOptions"
                PropertyChanges {
                    target: windowMainMenu
                    visible: false
                }
                PropertyChanges {
                    target: windowOptions
                    visible: true
                }
                PropertyChanges {
                    target: windowGame
                    visible: false
                }
            },
            State {
                name: "stateGame"
                PropertyChanges {
                    target: windowMainMenu
                    visible: false
                }
                PropertyChanges {
                    target: windowOptions
                    visible: false
                }
                PropertyChanges {
                    target: windowGame
                    visible: true
                }
            }
        ]
    }
}
