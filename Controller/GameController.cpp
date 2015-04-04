#include "ParserXML.h"
#include "GameController.h"

#include <QMessageBox>
#include <QDoubleValidator>
#include <QMap>
#include <QDebug>

#include <algorithm>
#include <cassert>
#include <QTimer>

#define cerro(x) std::cerr << x << std::endl;

GameController::GameController(GameModel* model, QUndoStack* undoStack, QObject* parent):
  AbstractController(model, undoStack, parent),
  _model(model),
  _gameInfo(new GameInfo),
  _orientedAreaTotal(0.0),
  _maxGapToWin(10),
  _fileName(""),
  _levelRunning(false) {

  _polygonListPerTurn = QList<PolygonList>() << _model->getPolygonList();
}

GameController::~GameController(void) {
  delete _gameInfo;
}

GameInfo* GameController::getGameInfo(void) const {
  return _gameInfo;
}

Point2d* GameController::getOtherBound(const Point2d* intersection, const std::vector<std::pair<Point2d*, Point2d*>>& cuttingSegments) const {
  for (const std::pair<Point2d*, Point2d*>& cuttingSegment: cuttingSegments) {
    if (cuttingSegment.first == intersection) {
      return cuttingSegment.second;
    } else if (cuttingSegment.second == intersection) {
      return cuttingSegment.first;
    }
  }

  return nullptr;
}

std::vector<std::pair<Point2d*, Point2d*>> GameController::getCuttingSegments(const std::vector<Point2d*>& intersections) const {
  assert(intersections.size()%2 == 0);

  std::vector<std::pair<Point2d*, Point2d*>> cuttingSegments;
  if (intersections.size() == 0)
    return cuttingSegments;

  for (unsigned int k = 0; k < intersections.size()-1; k += 2) {
    Segment AB(*intersections.at(k), *intersections.at(k+1));
    cuttingSegments.push_back(std::pair<Point2d*, Point2d*>(intersections.at(k), intersections.at((k+1)%intersections.size())));
  }
  return cuttingSegments;
}

bool GameController::stillHasBaseVertices(const std::vector<Point2d*>& globalVertices, const std::vector<Point2d*>& intersections) const {
  for (Point2d* p: globalVertices) {
    if (std::find(intersections.begin(), intersections.end(), p) == intersections.end())
      return true;
  }

  return false;
}

void GameController::cleanIntersections(const Polygon& polygon, std::vector<Point2d*>& intersections) const {
  if (intersections.size() < 2) {
    intersections.clear();
    return;
  }

  std::vector<Point2d*> realIntersection;
  bool inside = false;

  for (unsigned int k = 0; k < intersections.size()-1; ++k) {
    Segment AB(*intersections.at(k), *intersections.at(k+1));
    if (polygon.isPointInside2(AB.getCenter())) {
      if (!inside) {
        realIntersection.push_back(intersections.at(k));
        inside = true;
      }
    } else if (!polygon.isPointInside2(AB.getCenter())) {
      if (inside) {
        realIntersection.push_back(intersections.at(k));
        inside = false;
      }
    }
  }

  // Handle last vertex
  if (inside)
    realIntersection.push_back(intersections.at(intersections.size()-1));

//  for (Point2d* p: realIntersection)
//    std::cerr << *p << " ";
//  std::cerr << std::endl;

  intersections.clear();
  intersections = realIntersection;
}

void GameController::getVerticesAndIntersections(const Segment& line, const Polygon& polygon,
  std::vector<Point2d*>& globalVertices, std::vector<Point2d*>& intersections) const {

  std::vector<Point2d> baseVertices = polygon.getVertices();

  for (unsigned int k = 0; k < baseVertices.size(); ++k) {
    Point2d* A = new Point2d(baseVertices.at(k));
    Point2d* B = new Point2d(baseVertices.at((k+1)%baseVertices.size()));
    Segment AB(*A, *B);

    globalVertices.push_back(A);

    Segment::Intersection intersectionType = AB.computeIntersection(line);
    switch (intersectionType) {
    case Segment::Regular:
    {
      Point2d* intersection = new Point2d(Segment::intersectionPoint(AB, line));
      globalVertices.push_back(intersection);
      intersections.push_back(intersection);
      break;
    }
    case Segment::FirstVertexRegular:
    {
      Point2d* intersection = new Point2d(baseVertices.at(k));
      globalVertices.push_back(intersection);
      intersections.push_back(intersection);
      break;
    }
    default:
      break;
    }
  }

  std::sort(intersections.begin(), intersections.end(), pointCompare);

  cleanIntersections(polygon, intersections);
}

void GameController::sliceIt(const std::vector<Segment>& lines) {
  PolygonList newPolygonList;

  for (const Segment& line: lines) {
//    // Extend line length, to get mirror that are on polygon edge
//    Vector2d AB(currLine.getA(), currLine.getB());
//    AB *= 1.1;
//    AB.setX(qRound(AB.getX()));
//    AB.setY(qRound(AB.getY()));
//    Vector2d BA(-AB);
//    Segment line(currLine.getB()+Point2d(BA.getX(), BA.getY()), currLine.getA()+Point2d(AB.getX(), AB.getY()));
//    std::cerr << currLine << " " << line << " " << Vector2d::areColinear(currLine.getNormal(), line.getNormal()) << std::endl;

    // Browse every polygon and slice it!
    computeNewPolygonList(newPolygonList, line);
    _model->setPolygonList(newPolygonList);

    newPolygonList.clear();
  }

  _polygonListPerTurn << _model->getPolygonList();

  _gameInfo->setPartsCut(_model->getPolygonsCount());
  _gameInfo->setLinesDrawn(_gameInfo->linesDrawn()+1);

  emit update();
}

void GameController::computeNewPolygonList(PolygonList& newPolygonList, const Segment& line) const {
  for (const Polygon& polygon: _model->getPolygonList()) {
    std::vector<Point2d*> globalVertices;
    std::vector<Point2d*> intersections;
    getVerticesAndIntersections(line, polygon, globalVertices, intersections);

    std::vector<Point2d> newVertices;

    std::vector<std::pair<Point2d*, Point2d*>> cuttingSegments = getCuttingSegments(intersections);

    while (stillHasBaseVertices(globalVertices, intersections)) {

      // We really don't want the first point to be an intersection. Trust me.
      Point2d* p = globalVertices.at(0);
      while (std::find(intersections.begin(), intersections.end(), p) != intersections.end()) {
        globalVertices.erase(globalVertices.begin());
        globalVertices.push_back(p);
        p = globalVertices.at(0);
      }
      std::vector<Point2d*> globalVerticesCopy(globalVertices);

      bool lookingForOtherBound = false;
      Point2d* otherBound = nullptr;
      for (Point2d* currVerrtex: globalVerticesCopy) {
        if (lookingForOtherBound) {
          if (otherBound == currVerrtex) {
            newVertices.push_back(*currVerrtex);
            lookingForOtherBound = false;
          }
        } else {
          if (std::find(intersections.begin(), intersections.end(), currVerrtex) != intersections.end()) {
            // If the intersection is not equal to the last point, we add it
            if (newVertices.size() > 0 && (std::find(newVertices.begin(), newVertices.end(), *currVerrtex) == newVertices.end())) {
              newVertices.push_back(*currVerrtex);
            }
            otherBound = getOtherBound(currVerrtex, cuttingSegments);
            lookingForOtherBound = true;
          } else {
            newVertices.push_back(*currVerrtex);
            auto it = std::find(globalVertices.begin(), globalVertices.end(), currVerrtex);
            assert(it != globalVertices.end());
            globalVertices.erase(it);
          }
        }
      }
      globalVerticesCopy.clear();
      globalVerticesCopy = globalVertices;

      Polygon newPolygon(newVertices);
      // Don't add the new polygon if its area is less than 0.1% of the total area.
      // This allows users to draw several lines that pass near a point,
      // but not exactly on this point, since it's quite difficult to achieve.
      if (qRound(10.0*newPolygon.orientedArea() * 100.0 / _orientedAreaTotal)/10.0 >= 0.1)
        newPolygonList << newPolygon;

      newVertices.clear();
    }
  }
}

GameController::LineType GameController::computeLineType(const std::vector<Segment>& lines) const {
  bool noCrossing = false;
  bool goodCrossing = false;
  bool badCrossing = false;

  PolygonList polygonList = _model->getPolygonList();
  for (const Segment& line: lines) {
    for (const Polygon& polygon: polygonList) {
      if (!polygon.isCrossing(line) && !polygon.isPointInside2(line.getA())) {
        noCrossing = true;
      } else if (polygon.isGoodSegment(line)) {
        goodCrossing = true;
      } else {
        badCrossing = true;
      }
    }

    TapeList tapeList = _model->getTapeList();
    for (const Tape& tape: tapeList) {
      if (tape.crossing(line)) {
        badCrossing = true;
        break;
      }
    }
  }

  if (badCrossing)
    return GameController::badCrossing;
  else if (goodCrossing)
    return GameController::goodCrossing;
  else if (noCrossing)
    return GameController::noCrossing;
  else
    return GameController::unknownCrossing;
}

void GameController::checkWinning(void) {
  if (_gameInfo->linesDrawn() >= _gameInfo->linesCount() || _gameInfo->partsCut() >= _gameInfo->partsCount()) {
    QList<float> orientedAreas;
    QList<Vector2d> shiftVectors;
    float minArea = 100.0;
    float maxArea = 0.0;

    for (const Polygon& polygon: _model->getPolygonList()) {
      float currArea = computePolygonPercentageArea(polygon);

      orientedAreas << currArea;
      minArea = qMin(currArea, minArea);
      maxArea = qMax(currArea, maxArea);

      Vector2d currShift(computeGlobalBarycenter(), polygon.barycenter());
      float currShiftLength = currShift.norm();
      try {
        currShift.normalize();
      } catch (std::runtime_error&) {
      }
      currShift *= 0.2*currShiftLength;
      shiftVectors << currShift;
    }

    translatePolygons(shiftVectors);

    float gap = qAbs(maxArea - minArea);

    int starsCount = 0;
    _gameInfo->setStars(starsCount);
    if (_gameInfo->partsCut() == _gameInfo->partsCount() && gap <= _maxGapToWin && _gameInfo->partsCut() == _gameInfo->partsCount()) {
      starsCount = 6-qCeil(gap / _maxGapToWin * 5);
      updateStarsMax(starsCount);
      _gameInfo->setStars(starsCount);
    }
    emit levelEnd(orientedAreas);

    _levelRunning = false;
  }
}

void GameController::undoSliceIt(void) {
  if (_polygonListPerTurn.size() > 1) {
    _polygonListPerTurn.pop_back();
    _model->setPolygonList(_polygonListPerTurn.last());
    _gameInfo->setLinesDrawn(_gameInfo->linesDrawn()-1);
    _gameInfo->setPartsCut(_model->getPolygonsCount());
    _levelRunning = true;

    emit update();
  }
}

void GameController::updateStarsMax(int starsMaxCount) {
  ParserXML parser(_fileName);
  int currentStarsCount = parser.getStarsCount();

  if (currentStarsCount < starsMaxCount) {
    parser.setStarsCount(starsMaxCount);
    parser.writeXML();

    QString levelName = "resources/levels/"+_fileName.split("resources/levels/").last();
    QStringList levelPackPath = levelName.split("/");
    levelPackPath.removeLast();
    QString levelsFileName = "../PieceOfCake/"+levelPackPath.join("/")+"/levels.xml";

    QFile XMLDoc(levelsFileName);
    if (!XMLDoc.exists()) {
      qDebug() << "Error:" << levelsFileName << "file not found";
      return;
    }

    if(!XMLDoc.open(QIODevice::ReadOnly | QIODevice::Text)) {
      qDebug() << "Cannot open XML file in GameController::updateStarsMax(int starsMaxCount)";
      return;
    }

    QDomDocument doc("PieceOfCakeLevelsML");
    if(!doc.setContent(&XMLDoc)) {
      XMLDoc.close();
      qDebug() << "Cannot set content of dom in GameController::updateStarsMax(int starsMaxCount)";
      return;
    }

    XMLDoc.close();

    QDomElement root = doc.firstChildElement("levels");

    QDomNodeList levelList = root.elementsByTagName("level");
    int k = 0;
    for (; k < levelList.size(); ++k) {
      if (levelList.at(k).toElement().attribute("name") == levelName) {
        break;
      }
    }

    doc.elementsByTagName("level").at(k).toElement().setAttribute("stars", starsMaxCount);

    if(!XMLDoc.open(QIODevice::WriteOnly | QIODevice::Text)) {
      qDebug() << "Cannot open XML file in LevelDesignerController::saveLevel(const QString& fileName)";
      return;
    }

    QTextStream inFile(&XMLDoc);
    inFile << doc.toString(2);

    XMLDoc.close();

  }
}

void GameController::translatePolygons(const QList<Vector2d>& shiftVectors) {
  PolygonList newPolygons;

  auto it = _model->getPolygonList().cbegin();
  unsigned int currIndex = 0;
  for (; it != _model->getPolygonList().cend(); ++it) {
    Polygon newPolygon(*it);
    newPolygon.translate(shiftVectors.at(currIndex));
    newPolygons << newPolygon;
    ++currIndex;
  }

  _model->setPolygonList(newPolygons);
}

void GameController::replay(void) {
  openLevel(_fileName);
}

void GameController::clearGame(void) {
  _model->clearPolygons();
  _model->clearTapes();
  _model->clearMirrors();
  _model->clearPortals();
  _model->clearRefractors();
  _model->clearDeviations();
  _gameInfo->setLinesCount(0);
  _gameInfo->setPartsCut(0);
  _gameInfo->setPartsCount(0);
  _gameInfo->setStars(-1);
  _gameInfo->setStarsMax(-1);
  _orientedAreaTotal = 0.0;
  _maxGapToWin = 0.0;
  _levelRunning = false;
  _polygonListPerTurn.clear();

  emit update();
}

Deviation* GameController::getNearestDeviation(const Segment& line) const {
  DeviationList deviations = _model->getDeviationList();

  float minDist = -1;
  Deviation* nearestDeviation = nullptr;

  for (Deviation* deviation: deviations) {
    std::vector<Segment> deviateLines = deviation->deviateLine(line);
    // If there is at least one reflected line
    if (deviateLines.size() > 1) {
      if (minDist == -1)
        deviateLines.at(0).length();
      else
        qMin(minDist, deviateLines.at(0).length());
      nearestDeviation = deviation;
    }
  }

  return nearestDeviation;
}

void GameController::computeDeviateLines(float firstLineLength, const Segment& line, std::vector<Segment>& lines) const {
  Deviation* nearestDeviation = getNearestDeviation(line);

  if (nearestDeviation) {
    std::vector<Segment> deviateLines = nearestDeviation->deviateLine(line);
    // Init firstLineLength
    if (firstLineLength == -1.f)
      firstLineLength = deviateLines.at(0).length();

    // At least two line has to be here: the line drawn and its reflexion
    assert(deviateLines.size() > 1);

    // Erase previous line, since it go through current mirror
    if (lines.size() > 1)
      lines.erase(lines.end()-1);

    // Get the mirrorLined the length of the firstLine
    Segment deviateLine = deviateLines.at(1);
    Point2d A = deviateLine.getA();
    Point2d B = deviateLine.getB();
    Vector2d lu = firstLineLength*Vector2d(A, B).normalize();
    Point2d BB(lu.getX(), lu.getY());
    deviateLine.setB(A + BB);

    // Push line and its reflexion
    lines.push_back(deviateLines.at(0));
    lines.push_back(deviateLine);

    computeDeviateLines(firstLineLength, deviateLine, lines);
  } else {
    lines.push_back(line);
  }
}

float GameController::computePolygonPercentageArea(const Polygon& polygon) const {
  return qRound(10.0*polygon.orientedArea() * 100.0 / _orientedAreaTotal)/10.0;
}

Point2d GameController::computeGlobalBarycenter() const {
  Point2d globalBarycenter;
  unsigned int polygonCount = 0;

  for (const Polygon& polygon: _model->getPolygonList()) {
    globalBarycenter += polygon.barycenter();
    ++polygonCount;
  }

  assert(polygonCount > 0);
  globalBarycenter /= polygonCount;

  return globalBarycenter;
}

void GameController::openLevel(const QString& fileName) {
  clear();
  clearGame();

  ParserXML parser(fileName);

  _fileName = QString(fileName);

  PolygonList polygonList(parser.createPolygonList());
  _model->setPolygonList(polygonList);

  TapeList tapeList(parser.createTapeList());
  _model->setTapeList(tapeList);

  MirrorList mirrorList(parser.createMirrorList());
  PortalList portalList(parser.createPortalList());
  RefractorList refractorList(parser.createRefractorList());
  DeviationList deviationsList;
  for (const Mirror& mirror: mirrorList) {
    deviationsList.push_back(new Mirror(mirror));
  }
  for (const Portal& portal: portalList) {
    deviationsList.push_back(new Portal(portal));
  }
  for (const Refractor& refractor: refractorList) {
    deviationsList.push_back(new Refractor(refractor));
  }
  _model->setDeviationList(deviationsList);

  _gameInfo->setLinesDrawn(0);
  _gameInfo->setLinesCount(parser.getLinesCount());
  _gameInfo->setPartsCut(_model->getPolygonsCount());
  _gameInfo->setPartsCount(parser.getPartsCount());
  _gameInfo->setStars(-1);
  _gameInfo->setStarsMax(parser.getStarsCount());
  _maxGapToWin = parser.getMaxGapToWin();

  for (const Polygon& polygon: _model->getPolygonList()) {
    _orientedAreaTotal += polygon.orientedArea();
  }

  _levelRunning = true;

  _polygonListPerTurn << _model->getPolygonList();

  emit update();
}



bool pointCompare(const Point2d* A, const Point2d* B) {
  return *A < *B;
}
