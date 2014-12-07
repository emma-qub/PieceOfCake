import QtQuick 2.3
import QtQuick.Window 2.1
import QtQuick.Controls 1.1
import QtQuick.XmlListModel 2.0

import "selectLevel.js" as SelectJS

Rectangle {
  id: homePage
  visible: true
  width: 1200
  height: 756
  color: "#FFFFFF"
  signal levelOpenRequested(string fileName)

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
        onClicked: stackView.replace(selectLevelItem)

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

  FontLoader {
    id: homeFont
    name: "Pacifico"
    source: "resources/fonts/Pacifico.ttf"
  }

  XmlListModel {
    id: levelsModel
    source: "../PieceOfCake/resources/levels/pack1/levels.xml"
    query: "/levels/level"

    XmlRole { name: "stars"; query: "stars/string()" }
    XmlRole { name: "image"; query: "image/string()" }
    XmlRole { name: "name"; query: "name/string()" }
  }

  Item {
    id: selectLevelItem

    Image {
      id: selectLevels
      x: 0
      y: 0
      sourceSize.height: 756
      sourceSize.width: 1200
      source: "resources/images/selectLevels.png"

//      MouseArea {
//        id: pack1Area
//        x: SelectJS.computeXTab(0)
//        y: 120
//        width: 60+SelectJS.computeShiftTab(0)
//        height: 40
//        hoverEnabled: true
//        onClicked: SelectJS.setSelectedPack(0)

//        Rectangle {
//          id: pack1Rectangle
//          x: 0
//          y: 0
//          width: 60+SelectJS.computeShiftTab(0)
//          height: 40
//          color: "#639c45"
//          border.width: 2
//          border.color: "#333"

//          Text {
//            id: pack1Text
//            x: 6+SelectJS.computeShiftTab(0)
//            y: 5
//            width: 49
//            font.family: homeFont.name
//            text: qsTr("Pack 1")
//            font.bold: true
//            font.pixelSize: 18
//          }
//        }
//      }

//      MouseArea {
//        id: pack2Area
//        x: SelectJS.computeXTab(1)
//        y: 200
//        width: 60+SelectJS.computeShiftTab(1)
//        height: 40
//        hoverEnabled: true
//        acceptedButtons: Qt.LeftButton
//        onClicked: SelectJS.setSelectedPack(1)

//        Rectangle {
//          id: pack2Rectangle
//          x: 0
//          y: 0
//          width: 60+SelectJS.computeShiftTab(1)
//          height: 40
//          color: "#fff080"
//          border.width: 2
//          border.color: "#333"

//          Text {
//            id: pack2Text
//            x: 6+SelectJS.computeShiftTab(1)
//            y: 5
//            font.family: homeFont.name
//            text: qsTr("Pack 2")
//            font.bold: true
//            font.pixelSize: 18
//          }
//        }
//      }

//      MouseArea {
//        id: pack3Area
//        x: SelectJS.computeXTab(2)
//        y: 280
//        width: 60+SelectJS.computeShiftTab(2)
//        height: 40
//        hoverEnabled: true
//        acceptedButtons: Qt.LeftButton
//        onClicked: SelectJS.setSelectedPack(2)

//        Rectangle {
//          id: pack3Rectangle
//          x: 0
//          y: 0
//          width: 60+SelectJS.computeShiftTab(2)
//          height: 40
//          color: "steelblue"
//          border.width: 2
//          border.color: "#333"

//          Text {
//            id: pack3Text
//            x: 6+SelectJS.computeShiftTab(2)
//            y: 5
//            font.family: homeFont.name
//            text: qsTr("Pack 3")
//            font.bold: true
//            font.pixelSize: 18
//          }
//        }
//      }

//      MouseArea {
//        id: pack4Area
//        x: SelectJS.computeXTab(3)
//        y: 360
//        width: 60+SelectJS.computeShiftTab(3)
//        height: 40
//        hoverEnabled: true
//        acceptedButtons: Qt.LeftButton
//        onClicked: SelectJS.setSelectedPack(3)

//        Rectangle {
//          id: pack4Rectangle
//          x: 0
//          y: 0
//          width: 60+SelectJS.computeShiftTab(3)
//          height: 40
//          color: "#f07835"
//          border.width: 2
//          border.color: "#333"

//          Text {
//            id: pack4Text
//            x: 6+SelectJS.computeShiftTab(3)
//            y: 5
//            font.family: homeFont.name
//            text: qsTr("Pack 4")
//            font.bold: true
//            font.pixelSize: 18
//          }
//        }
//      }

//      MouseArea {
//        id: pack5Area
//        x: SelectJS.computeXTab(4)
//        y: 440
//        width: 60+SelectJS.computeShiftTab(4)
//        height: 40
//        hoverEnabled: true
//        acceptedButtons: Qt.LeftButton
//        onClicked: SelectJS.setSelectedPack(4)

//        Rectangle {
//          id: pack5Rectangle
//          x: 0
//          y: 0
//          width: 60+SelectJS.computeShiftTab(4)
//          height: 40
//          color: "#60a0ff"
//          border.width: 2
//          border.color: "#333"

//          Text {
//            id: pack5Text
//            x: 6+SelectJS.computeShiftTab(4)
//            y: 5
//            font.family: homeFont.name
//            text: qsTr("Pack 5")
//            font.bold: true
//            font.pixelSize: 18
//          }
//        }
//      }
    }

    Rectangle {
      id: titleRectangle
      width: 375
      height: 100
      x: 200
      y: 100

      Text {
        id: packTitleText
        font.family: homeFont.name
        font.bold: true
        text: "Pack 1: Cut Polygons"
        font.pixelSize: 24
        color:"#333333"
        anchors.horizontalCenter: titleRectangle.horizontalCenter
      }

    }

    GridView {
      id: levelsView
      width: 375
      height: 400

      x: 200
      y: 200

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
          onClicked: levelOpenRequested(name)

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

          text: SelectJS.starsToString(stars)
          anchors.horizontalCenter: parent.horizontalCenter
        }
      }
    }

    Rectangle {
      id: homeRectangle
      width: 375
      height: 50
      x: 200
      y: 600

      MouseArea {
        id: homeArea
        width: 40
        height: 40
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        onClicked: stackView.replace(home)

        Rectangle {
          height: 50
          width: 50
          x: -5
          y: -5
          radius: 5
          color: homeArea.containsMouse ? "#CCCCCC" : "#FFFFFF"
        }

        Image {
          height: 40
          width: 40
          source: "resources/images/home.png";
        }
      }
    }

    Rectangle {
      id: goalRectangle
      x: 650
      y: 150
      width: 375
      height: 150
//      color: "steelblue"

      Text {
        id: goalTitleText
        text: qsTr("Objective of the game")
        font.family: homeFont.name
        font.bold: true
        font.pixelSize: 24
        color:"#333333"
      }

      Text {
        id: goalText
        x: 10
        y: 40
        width: 300
        text: qsTr("  Cut polygons and try to get pieces of equal area.")
        wrapMode: Text.Wrap
        font.family: homeFont.name
        font.pixelSize: 20
        color:"#333333"
      }
    }
    Rectangle {
      id: rules1Rectangle
      x: 650
      y: 300
      width: 375
      height: 250
      visible: SelectJS.getSelectedPack() === 0

      Text {
        id: rulesTitle
        text: qsTr("Rules")
        font.family: homeFont.name
        font.bold: true
        font.pixelSize: 24
        color:"#333333"
      }

      Text {
        id: rule1Text
        y: 40
        text: qsTr("  - Cut through the whole polygon")
        font.family: homeFont.name
        font.pixelSize: 20
        color:"#333333"
      }

      Image {
        height: 100
        width: 100
        x: 0
        y: 80
        source: "resources/images/rule1Pack1.png";
      }
      Image {
        height: 100
        width: 100
        x: 125
        y: 80
        source: "resources/images/rule2Pack1.png";
      }
      Image {
        height: 100
        width: 100
        x: 250
        y: 80
        source: "resources/images/rule3Pack1.png";
      }

      Text {
        id: rule2Text
        y: 200
        text: qsTr("  - Respect number of lines and pieces")
        font.family: homeFont.name
        font.pixelSize: 20
        color:"#333333"
      }
    }

    Rectangle {
      id: navigationRectangle
      x: 650
      y: 600
      width: 375
      height: 40
//      color: "lightblue"

      Row {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        Repeater {
          id: pageRepeater
          model: 5
          MouseArea {
            width: 40
            height: 40
            onClicked: SelectJS.setSelectedPack(index)
            Rectangle {
              width: 40
              height: 40
              Rectangle {
                width: SelectJS.getSelectedPack() === index ? 14 : 10
                height: SelectJS.getSelectedPack() === index ? 14 : 10
                radius: SelectJS.getSelectedPack() === index ? 7 : 5
                color: SelectJS.getSelectedPack() === index ? "white" : "black"
                border.width: SelectJS.getSelectedPack() === index ? 3 : 0
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
              }
            }
          }
        }
      }
    }
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
