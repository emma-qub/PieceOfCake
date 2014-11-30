import QtQuick 2.3
import QtQuick.Window 2.2

Window {
    id: homePage
    visible: true
    width: 1200
    height: 756
    color: "#343434"

    MouseArea {
        anchors.fill: parent
        onClicked: {

        }

        Image {
            id: background
            x: 0
            y: 0
            sourceSize.height: 756
            sourceSize.width: 1200
            source: creditArea.containsMouse ? "resources/images/creditIn.png" : "resources/images/homeBackgroundNaked.png"

            MouseArea {
                id: playArea
                x: 790
                y: 310
                width: 170
                height: 100
                hoverEnabled: true
                //onClicked: screenChanged

                Image {
                    id: playIn
                    source: "resources/images/playIn.png"
                    visible: playArea.containsMouse
                }

                Text {
                    id: playText
                    x: creditArea.containsMouse ? 150 : 112
                    y: creditArea.containsMouse ? 20 : 63
                    width: 107
                    height: 60
                    text: qsTr("Play")
                    font.family: homeFont.name
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.WordWrap
                    font.pixelSize: 22
                    color: playArea.containsMouse ? "#639c45" : "black"
                }
            }

            MouseArea {
                id: followUsArea
                x: 200
                y: 180
                width: 205
                height: 220
                hoverEnabled: true

                Image {
                    id: followUsIn
                    source: "resources/images/followUsIn.png"
                    visible: followUsArea.containsMouse
                }

                Image {
                    id: followUsOut
                    source: "resources/images/followUsOut.png"
                    visible: !followUsArea.containsMouse
                }

                Text {
                    id: followUsText
                    x: 16
                    y: -28
                    width: 173
                    height: 44
                    text: qsTr("Follow us")
                    font.family: homeFont.name
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.WordWrap
                    font.pixelSize: 22
                    color: followUsArea.containsMouse ? "#639c45" : "black"
                }

            }

            MouseArea {
                id: createLevelsArea
                x: 730
                y: 40
                width: 150
                height: 240
                hoverEnabled: true

                Image {
                    id: createLevelsIn
                    source: "resources/images/createLevelsIn.png"
                    visible: createLevelsArea.containsMouse
                }

                Image {
                    id: createLevelsOut
                    source: "resources/images/createLevelsOut.png"
                    visible: !createLevelsArea.containsMouse
                }

                Text {
                    id: createLevelsText
                    x: 137
                    y: 53
                    width: 146
                    height: 47
                    text: qsTr("Create levels")
                    font.family: homeFont.name
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.WordWrap
                    font.pixelSize: 22
                    color: createLevelsArea.containsMouse ? "#639c45" : "black"
                }
            }

            MouseArea {
                id: optionsArea
                x: 230
                y: 405
                width: 235
                height: 260
                hoverEnabled: true

                Image {
                    id: optionsIn
                    source: "resources/images/optionsIn.png"
                    visible: optionsArea.containsMouse
                }

                Image {
                    id: optionsOut
                    source: "resources/images/optionsOut.png"
                    visible: !optionsArea.containsMouse
                }

                Text {
                    id: optionsText
                    x: -29
                    y: -9
                    width: 114
                    height: 40
                    font.family: homeFont.name
                    font.bold: true
                    text: qsTr("Options")
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 22
                    color: optionsArea.containsMouse ? "#639c45" : "black"
                }
            }

            MouseArea {
                id: creditArea
                x: 730
                y: 520
                width: 170
                height: 140
                hoverEnabled: true

                Text {
                    id: creditText
                    x: creditArea.containsMouse ? 168 : 108
                    y: creditArea.containsMouse ? 16 : 71
                    width: 114
                    height: 40
                    font.family: homeFont.name
                    font.bold: true
                    text: qsTr("Credit")
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 22
                    color: creditArea.containsMouse ? "#639c45" : "black"
                }
            }
        }

    }

//    StateGroup {
//        id: stateGroup
//        states: [
//            State {
//                name: "PlayHovered"
//                PropertyChanges {
//                    target: playText
//                }
//            }
//        ]
//    }

    FontLoader {
        id: homeFont
        name: "Pacifico"
        source: "/resources/fonts/Pacifico.ttf"
    }
}
