import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

ApplicationWindow {
    id: windowApp

    visible: true
    width: 500
    height: 600
    title: qsTr("Quoridor")

    Rectangle {
        id: windowMain
        state: "stateMainMenu"

        width: windowApp.width
        height: windowApp.height
        x: 0
        y: 0

        // main menu window
        Item {
            id: windowMainMenu
            objectName: "mainMenu"

            x: 20
            y: 20
            width: parent.width - 40
            height: parent.height - 40

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

            x: 20
            y: 20
            width: parent.width - 40
            height: parent.height - 40

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

            x: 20
            y: 20
            width: parent.width - 40
            height: parent.height - 40

            Board {
                id: board
                objectName: "board"

                width: parent.width
                height: parent.height - 60
                x: 0
                y: 0
            }

            Rectangle {
                id: pawnWallSwitcher

                width: parent.width / 4
                height: 20
                anchors.top: board.bottom
                anchors.horizontalCenter: parent.horizontalCenter

                color: "sienna"

                MouseArea {
                    id: pawnWallSwitcherMouseArea
                    anchors.fill: parent
                    onClicked: {
                        console.log("pawn/wall switcher clicked");
                        board.switchPlayerActivity();
                    }
                }
            }

            Item {
                id: buttonsArea

                width: parent.width / 2
                anchors.top: pawnWallSwitcher.bottom
                anchors.bottom: parent.bottom
                anchors.right: parent.right

                Button {
                    id: buttonBackToOptions
                    objectName: "buttonBackToOptions"
                    text: "Back"

                    anchors.centerIn: parent

                    // @fixme change state from C++
                    onClicked: {
                        board.endGame()
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
