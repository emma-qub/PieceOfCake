import QtQuick 2.3
import QtQuick.Window 2.1
import QtQuick.Controls 1.1
import QtQuick.XmlListModel 2.0

import "selectLevel.js" as SelectJS
import gameinfo 1.0

Rectangle {
  id: homePage
  visible: true
  width: 1200
  height: 756
  color: "#FFFFFF"
  signal openLevelRequested(string fileName)
  signal createLevelRequested()
  signal homePageRequested()
  signal backToLevelsRequested()
  signal refreshLevelRequested()

  // Home Page
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
        onClicked: {
          levelsModel.reload()
          stackView.replace(selectLevelItem)
        }

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
        onClicked: {
          stackView.replace(createLevelItem)
          createLevelRequested()
        }

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

  // Select Level
  Item {
    id: selectLevelItem

    Image {
      id: selectLevels
      x: 0
      y: 0
      sourceSize.height: 756
      sourceSize.width: 1200
      source: "resources/images/selectLevels.png"
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
          onClicked: {
            SelectJS.setSelectedLevel(id)
            stackView.replace(gameItem)
            openLevelRequested(name)
          }

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

          Rectangle {
            width: 20
            height: 20
            x: -10
            y: -10
            radius: 5
            border.width: 1
            border.color: "#cccccc"
            Text {
              text: id+1
              anchors.horizontalCenter: parent.horizontalCenter
              anchors.verticalCenter: parent.verticalCenter
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
        onClicked: {
          stackView.replace(home)
          homePageRequested()
        }

        Image {
          height: 40
          width: 40
          source: homeArea.containsMouse ? "resources/images/homeIn.png" : "resources/images/homeOut.png"
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
        text: qsTr("Objective")
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
            id: pageArea
            width: 40
            height: 40
            hoverEnabled: true
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

  // Create Level
  Item {
    id: createLevelItem
    width: 1200
    height: 756

    Rectangle {
      id: createLevelRectangle
      width: 1200
      height: 756

      Image {
          source: "../PieceOfCake/resources/images/selectLevels.png"
      }
    }

    Rectangle {
      id: homeCreateLevelRectangle
      width: 350
      height: 50
      x: 200
      y: 600

      MouseArea {
        id: homeCreateLevelArea
        width: 40
        height: 40
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        onClicked: {
          stackView.replace(home)
          homePageRequested()
        }

        Image {
          height: 40
          width: 40
          source: homeCreateLevelArea.containsMouse ? "resources/images/homeIn.png" : "resources/images/homeOut.png"
        }
      }
    }

  }

  // Game
  Item {
    id: gameItem
    width: 1200
    height: 756

    Rectangle {
      id: gameRectangle
      width: 1200
      height: 756

      Image {
          source: "../PieceOfCake/resources/images/selectLevels.png"
      }
    }

    Rectangle {
      id: drawAreaRectangle
      x: 174
      y: 149
      width: 403
      height: 403
      border.width: 1
      border.color: "#ddddff"
    }

    Rectangle {
      id: homeGameRectangle
      width: 350
      height: 50
      x: 200
      y: 600

      MouseArea {
        id: backToLevelsArea
        width: 40
        height: 40
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton
        anchors.horizontalCenter: parent.left
        anchors.verticalCenter: parent.verticalCenter
        onClicked: {
          SelectJS.setSelectedLevel(-1)
          stackView.replace(selectLevelItem)
          backToLevelsRequested()
        }

        Image {
          height: 40
          width: 40
          source: backToLevelsArea.containsMouse ? "resources/images/backToLevelsIn.png" : "resources/images/backToLevelsOut.png"
        }
      }

      MouseArea {
        id: homeGameArea
        width: 40
        height: 40
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        onClicked: {
          stackView.replace(home)
          homePageRequested()
        }

        Image {
          height: 40
          width: 40
          source: homeGameArea.containsMouse ? "resources/images/homeIn.png" : "resources/images/homeOut.png"
        }
      }

      MouseArea {
        id: eraseArea
        width: 40
        height: 40
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton
        anchors.horizontalCenter: parent.right
        anchors.verticalCenter: parent.verticalCenter
        onClicked: {
//          stackView.replace(home)
//          homePageRequested()
        }

        Image {
          height: 40
          width: 40
          source: eraseArea.containsMouse ? "resources/images/eraserIn.png" : "resources/images/eraserOut.png"
        }
      }
    }

    Rectangle {
      id: missionRectangle
      x: 650
      y: 150
      width: 375
      height: 150

      MouseArea {
        id: missionMouseArea
        width: 375
        height: 150
      }

      Text {
        id: missionTitleText
        text: qsTr("Mission")
        font.family: homeFont.name
        font.bold: true
        font.pixelSize: 24
        color:"#333333"
      }

      Image {
        id: lineImage
        x: 10
        y: 40
        width: 40
        height: 40
        source: "resources/images/line.png"
      }

      Text {
        id: linesText
        x: 60
        y: 50
        width: 300
        wrapMode: Text.Wrap
        font.pixelSize: 24
        font.bold: true
        color:"#333333"
        text: {
          var lines = gameInfo.linesCount - gameInfo.linesDrawn;
          return "x "+lines.toString();
        }
      }

      Image {
        id: partsImage
        x: 10
        y: 90
        width: 40
        height: 40
        source: "resources/images/parts.png"
      }

      Text {
        id: partsText
        x: 60
        y: 100
        width: 300
        wrapMode: Text.Wrap
        font.pixelSize: 24
        font.bold: true
        color:"#333333"
        text: gameInfo.partsCut.toString() + "/" + gameInfo.partsCount.toString()
      }
    }

    Rectangle {
      id: starsRectangle
      x: 650
      y: 300
      width: 375
      height: 150

      Text {
        id: starsText
        text: qsTr("Awards")
        font.family: homeFont.name
        font.bold: true
        font.pixelSize: 24
        color:"#333333"
      }

      Row {
        x: 80
        y: 50
        Repeater {
          model: gameInfo.stars < 6 ? gameInfo.stars : 5

          Image {
            id: lastStarsImage
            source: "resources/images/star.png"
          }
        }
        Repeater {
          model: gameInfo.stars > -1 ? 5 - gameInfo.stars : 5

          Image {
            id: lastStarsEmptyImage
            source: "resources/images/emptyStar.png"
          }
        }
      }

      Text {
        id: lastStarsText
        x: 10
        y: 50
        text: qsTr("Current:")
        font.family: homeFont.name
        font.bold: true
        font.pixelSize: 18
        color:"#333333"
      }

      Row {
        x: 80
        y: 90
        Repeater {
          model: gameInfo.starsMax < 6 ? gameInfo.starsMax : 5

          Image {
            id: lastStarsMaxImage
            source: "resources/images/star.png"
          }
        }
        Repeater {
          model: gameInfo.starsMax > -1 ? 5 - gameInfo.starsMax : 5

          Image {
            id: lastStarsEmptyMaxImage
            source: "resources/images/emptyStar.png"
          }
        }
      }

      Text {
        id: bestStarsText
        x: 35
        y: 90
        text: qsTr("Best:")
        font.family: homeFont.name
        font.bold: true
        font.pixelSize: 18
        color:"#333333"
      }

    }

    Rectangle {
      id: switchLevelsGameRectangle
      width: 350
      height: 50
      x: 650
      y: 600

      MouseArea {
        id: previousLevelsArea
        width: 40
        height: 40
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton
        anchors.horizontalCenter: parent.left
        anchors.verticalCenter: parent.verticalCenter
        onClicked: {
          SelectJS.setSelectedLevel(SelectJS.selectedLevel-1)
//          stackView.replace(selectLevelItem)
//          backToLevelsRequested()
        }

        Image {
          height: 40
          width: 40
          source: {
//            if (SelectJS.isFirstLevel()) {
//              return "resources/images/previousLevelDisabled.png";
//            } else {
              if (previousLevelsArea.containsMouse) {
                return "resources/images/previousLevelIn.png";
              } else {
                return "resources/images/previousLevelOut.png";
              }
//            }
          }
        }
      }

      MouseArea {
        id: refreshLevelArea
        width: 40
        height: 40
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        onClicked: {
          refreshLevelRequested()
        }

        Image {
          height: 40
          width: 40
          source: refreshLevelArea.containsMouse ? "resources/images/refreshIn.png" : "resources/images/refreshOut.png"
        }
      }

      MouseArea {
        id: nextLevelArea
        width: 40
        height: 40
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton
        anchors.horizontalCenter: parent.right
        anchors.verticalCenter: parent.verticalCenter
        onClicked: {
//          stackView.replace(home)
//          homePageRequested()
        }

        Image {
          height: 40
          width: 40
          source: nextLevelArea.containsMouse ? "resources/images/nextLevelIn.png" : "resources/images/nextLevelOut.png"
        }
      }
    }

  }

  // Font Loader
  FontLoader {
    id: homeFont
    name: "Pacifico"
    source: "resources/fonts/Pacifico.ttf"
  }

  // XML List Model
  XmlListModel {
    id: levelsModel
    source: "../PieceOfCake/resources/levels/pack1/levels.xml"
    query: "/levels/level"

    XmlRole { name: "id"; query: "@id/number()" }
    XmlRole { name: "stars"; query: "@stars/number()" }
    XmlRole { name: "image"; query: "@image/string()" }
    XmlRole { name: "name"; query: "@name/string()" }
  }

  // Stack View
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

  StateGroup {
    id: stateGroup
    states: [
      State {
        name: "green"
        when: missionMouseArea.pressed === true

        PropertyChanges {
          target: partsText
          color: "#639c45"
          font.pixelSize: 32
        }
      }
    ]
    transitions: [
      Transition {
        reversible: true
        from: "*"; to: "green"
        ParallelAnimation {
          NumberAnimation {
            duration: 1000
            properties: "font.pixelSize"
            easing.type: Easing.InOutQuad
          }
          ColorAnimation {
            duration: 1000
            easing.type: Easing.InOutQuad
          }
        }
      }
    ]
  }

}
