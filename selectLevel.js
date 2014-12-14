var selectedPack = 0;
var selectedLevel = -1;

var titles = [
  "Cut Polygons",
  "Adhesive Tapes",
  "Mirrors",
  "Portals",
  "Filters"
];

function setSelectedLevel(levelNumber)  {
  selectedLevel = levelNumber;
}

function isFirstLevel() {
  return selectedLevel === 0;
}

function isLastLevel() {
  return selectedLevel === 19;
}

function setSelectedPack(packNumber) {
  selectedPack = packNumber;

  var k;
  for (k = 0; k < pageRepeater.count; k += 1) {
//    selectLevels.children[k].x = computeXTab(k);
//    selectLevels.children[k].width = 60+computeShiftTab(k);
//    selectLevels.children[k].children[0].width = 60+computeShiftTab(k);
//    selectLevels.children[k].children[0].children[0].x = 6+computeShiftTab(k)

    if (selectedPack === k) {
      pageRepeater.itemAt(k).children[0].children[0].width = 14;
      pageRepeater.itemAt(k).children[0].children[0].height = 14;
      pageRepeater.itemAt(k).children[0].children[0].radius = 7;
      pageRepeater.itemAt(k).children[0].children[0].color = "white";
      pageRepeater.itemAt(k).children[0].children[0].border.width = 3;
    } else {
      pageRepeater.itemAt(k).children[0].children[0].width = 10;
      pageRepeater.itemAt(k).children[0].children[0].height = 10;
      pageRepeater.itemAt(k).children[0].children[0].radius = 5;
      pageRepeater.itemAt(k).children[0].children[0].color = "black";
      pageRepeater.itemAt(k).children[0].children[0].border.width = 0;
    }
  }

  rules1Rectangle.visible = (selectedPack === 0);
  levelsModel.source = "../PieceOfCake/resources/levels/pack"+(packNumber+1).toString()+"/levels.xml";
  packTitleText.text = getSelectedPackTitle();
}

function getSelectedPack() {
  return selectedPack;
}

//function computeXTab(tabIndex) {
//  if (tabIndex < selectedPack)
//    return 80-2*selectedPack+2*tabIndex;
//  else if (tabIndex === selectedPack)
//    return 1040;
//  else
//    return 1065-2*selectedPack+2*tabIndex;
//}

//function computeShiftTab(tabIndex) {
//  if (tabIndex === selectedPack)
//    return 25;
//  else
//    return 0;
//}

function getSelectedPackTitle() {
  return "Pack "+(selectedPack+1).toString()+": "+titles[selectedPack];
}

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
