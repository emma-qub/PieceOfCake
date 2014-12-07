var selectedPack = 0;
var titles = [
    "Cut Polygons",
    "Adhesive Tapes",
    "Mirrors",
    "Portals",
    "Filters"
];

function setSelectedPack(packNumber) {
    selectedPack = packNumber;

    var k;
    for (k = 0; k < selectLevels.children.length; k += 1) {
        selectLevels.children[k].x = computeXTab(k);
        selectLevels.children[k].width = 60+computeShiftTab(k);
        selectLevels.children[k].children[0].width = 60+computeShiftTab(k);
        selectLevels.children[k].children[0].children[0].x = 6+computeShiftTab(k)
    }

    levelsModel.source = "../PieceOfCake/resources/levels/pack"+(packNumber+1).toString()+"/levels.xml";
    packTitleText.text = getSelectedPackTitle();
}

function getSelectedPack() {
    return selectedPack;
}

function computeXTab(tabIndex) {
    if (tabIndex < selectedPack)
        return 80-2*selectedPack+2*tabIndex;
    else if (tabIndex === selectedPack)
        return 1040;
    else
        return 1065-2*selectedPack+2*tabIndex;
}

function computeShiftTab(tabIndex) {
    if (tabIndex === selectedPack)
        return 25;
    else
        return 0;
}

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
