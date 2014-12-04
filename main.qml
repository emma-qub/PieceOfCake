import QtQuick 2.3
import QtQuick.Window 2.1
import QtQuick.Controls 1.1
import QtQuick.XmlListModel 2.0


Rectangle {
    id: homePage
    visible: true
    width: 1200
    height: 756
    color: "#FFFFFF"
    signal qmlSignal(string msg)
    signal onOpenLevel(string msg)

    Item {
        id: home

        Image {
            id: background
            x: 0
            y: 0
            sourceSize.height: 756
            sourceSize.width: 1200
            source: selectCreditArea.containsMouse ? "resources/images/creditIn.png" : "resources/images/homeBackgroundNaked.png"

            MouseArea {
                id: selectPlayArea
                x: 790
                y: 310
                width: 170
                height: 100
                hoverEnabled: true
                onClicked: stackView.replace(play)

                Image {
                    id: playIn
                    source: "resources/images/playIn.png"
                    visible: selectPlayArea.containsMouse
                }

                Text {
                    id: playText
                    x: selectCreditArea.containsMouse ? 150 : 112
                    y: selectCreditArea.containsMouse ? 20 : 63
                    width: 107
                    height: 60
                    text: qsTr("Play")
                    font.family: homeFont.name
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.WordWrap
                    font.pixelSize: 22
                    color: selectPlayArea.containsMouse ? "#639c45" : "black"
                }
            }

            MouseArea {
                id: selectFollowUsArea
                x: 200
                y: 180
                width: 205
                height: 220
                hoverEnabled: true
                //onClicked: stackView.replace(followUs)
                onClicked: homePage.qmlSignal("FOLLOW US!")

                Image {
                    id: followUsIn
                    source: "resources/images/followUsIn.png"
                    visible: selectFollowUsArea.containsMouse
                }

                Image {
                    id: followUsOut
                    source: "resources/images/followUsOut.png"
                    visible: !selectFollowUsArea.containsMouse
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
                    color: selectFollowUsArea.containsMouse ? "#639c45" : "black"
                }

            }

            MouseArea {
                id: selectCreateLevelsArea
                x: 730
                y: 40
                width: 150
                height: 240
                hoverEnabled: true
                onClicked: homePage.qmlSignal("createLevels")

                Image {
                    id: createLevelsIn
                    source: "resources/images/createLevelsIn.png"
                    visible: selectCreateLevelsArea.containsMouse
                }

                Image {
                    id: createLevelsOut
                    source: "resources/images/createLevelsOut.png"
                    visible: !selectCreateLevelsArea.containsMouse
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
                    color: selectCreateLevelsArea.containsMouse ? "#639c45" : "black"
                }
            }

            MouseArea {
                id: selectOptionsArea
                x: 230
                y: 405
                width: 235
                height: 260
                hoverEnabled: true
                onClicked: stackView.replace(options)

                Image {
                    id: optionsIn
                    source: "resources/images/optionsIn.png"
                    visible: selectOptionsArea.containsMouse
                }

                Image {
                    id: optionsOut
                    source: "resources/images/optionsOut.png"
                    visible: !selectOptionsArea.containsMouse
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
                    color: selectOptionsArea.containsMouse ? "#639c45" : "black"
                }
            }

            MouseArea {
                id: selectCreditArea
                x: 730
                y: 520
                width: 170
                height: 140
                hoverEnabled: true
                onClicked: stackView.replace(credit)

                Text {
                    id: creditText
                    x: selectCreditArea.containsMouse ? 168 : 108
                    y: selectCreditArea.containsMouse ? 16 : 71
                    width: 114
                    height: 40
                    font.family: homeFont.name
                    font.bold: true
                    text: qsTr("Credit")
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 22
                    color: selectCreditArea.containsMouse ? "#639c45" : "black"
                }
            }
        }
    }

    Item {
        id: play

        Rectangle {
            width: 1200
            height: 756

            XmlListModel {
                id: levelsModel
                source: "../PieceOfCake/resources/levels/levels.xml"
                query: "/levels/level"

                XmlRole { name: "stars"; query: "stars/string()" }
                XmlRole { name: "image"; query: "image/string()" }
                XmlRole { name: "name"; query: "name/string()" }
            }

            Image {
                id: selectLevels
                x: 0
                y: 0
                sourceSize.height: 756
                sourceSize.width: 1200
                source: "resources/images/selectLevels.png"
            }

            GridView {
                header: Text {
                    width: parent.width
                    height: 100
                    font.family: homeFont.name
                    font.bold: true
                    text: qsTr("Pack 1: Cut polygons")
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignTop
                    font.pixelSize: 22
                    color:"#333333"
                }

                id: levelsView
                width: 375
                height: 600

                x: 200
                y: 100

                cellHeight: 90
                cellWidth: 90

                model: levelsModel

                delegate: Column {
                    height: 80
                    MouseArea {
                        id: level
                        width: 60
                        height: 60
                        hoverEnabled: true
                        onClicked: onOpenLevel(name)

                        Rectangle {
                            width: 60
                            height: 60
                            color: "#639c45"
                            radius: 5
                            border.width: level.containsMouse ? 2 : 0

                            Image {
                                x: 5
                                y: 5
                                height: 50
                                width: 50
                                source: image;
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                        }
                    }

                    Text {
                        y: 10
                        function starsToString(stars) {
                            var k;
                            var starsStr = "";
                            if (parseInt(stars) > 0)
                                starsStr += "<b><font color='#F70'>"
                            else
                                starsStr += "<font color='#AAA'>"
                            for (k = 0; k < 5; k += 1) {
                                starsStr += "*";
                                if (k === parseInt(stars) && k > 0)
                                   starsStr += "</font></b><font color='#AAA'>";
                            }
                            starsStr += "</font>";
                            return starsStr;
                        }
                        text: starsToString(stars)
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }
            }
        }
    }

    Item {
        id: createLevels

        Rectangle {
            width: 1200
            height: 756

            MouseArea {
                id: createLevelsArea
                x: 230
                y: 275
                width: 231
                height: 114
                anchors.fill: parent
                onClicked: stackView.replace(home)

                Text {
                    id: createLevelsBackText
                    x: 381
                    y: 289
                    width: 160
                    height: 47
                    text: qsTr("Go back to menu")
                    font.family: "Pacifico"
                    font.pixelSize: 22
                }
            }

            Text {
                id: createLevelsTitleText
                x: 225
                y: 148
                width: 197
                height: 84
                text: qsTr("Create Levels")
                font.family: "Pacifico"
                font.pixelSize: 22
            }
        }
    }

    Item {
        id: credit

        Rectangle {
            width: 1200
            height: 756

            MouseArea {
                id: creditArea
                x: 230
                y: 275
                width: 231
                height: 114
                anchors.fill: parent
                onClicked: stackView.replace(home)

                Text {
                    id: creditBackText
                    x: 381
                    y: 289
                    width: 160
                    height: 47
                    text: qsTr("Go back to menu")
                    font.family: "Pacifico"
                    font.pixelSize: 22
                }
            }

            Text {
                id: creditTitleText
                x: 225
                y: 148
                width: 197
                height: 84
                text: qsTr("Credits")
                font.family: "Pacifico"
                font.pixelSize: 22
            }
        }
    }

    Item {
        id: options

        Rectangle {
            width: 1200
            height: 756

            MouseArea {
                id: optionsArea
                x: 230
                y: 275
                width: 231
                height: 114
                anchors.fill: parent
                onClicked: stackView.replace(home)

                Text {
                    id: optionsBackText
                    x: 381
                    y: 289
                    width: 160
                    height: 47
                    text: qsTr("Go back to menu")
                    font.family: "Pacifico"
                    font.pixelSize: 22
                }
            }

            Text {
                id: optionsTitleText
                x: 225
                y: 148
                width: 197
                height: 84
                text: qsTr("Option")
                font.family: "Pacifico"
                font.pixelSize: 22
            }
        }
    }

    Item {
        id: followUs

        Rectangle {
            width: 1200
            height: 756

            MouseArea {
                id: followUsArea
                x: 230
                y: 275
                width: 231
                height: 114
                anchors.fill: parent
                onClicked: stackView.replace(home)

                Text {
                    id: followUsBackText
                    x: 381
                    y: 289
                    width: 160
                    height: 47
                    text: qsTr("Go back to menu")
                    font.family: "Pacifico"
                    font.pixelSize: 22
                }
            }

            Text {
                id: followUsTitleText
                x: 225
                y: 148
                width: 197
                height: 84
                text: qsTr("Follow Us")
                font.family: "Pacifico"
                font.pixelSize: 22
            }
        }
    }

    FontLoader {
        id: homeFont
        name: "Pacifico"
        source: "resources/fonts/Pacifico.ttf"
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: home

        delegate: StackViewDelegate {
            function transitionFinished(properties)
            {
                properties.exitItem.opacity = 1
            }

            pushTransition: StackViewTransition {
                PropertyAnimation {
                    target: enterItem
                    property: "opacity"
                    from: 0
                    to: 1
                }
                PropertyAnimation {
                    target: exitItem
                    property: "opacity"
                    from: 1
                    to: 0
                }
            }
        }
    }
}

////    StateGroup {
////        id: stateGroup
////        states: [
////            State {
////                name: "PlayHovered"
////                PropertyChanges {
////                    target: playText
////                }
////            }
////        ]
////    }
