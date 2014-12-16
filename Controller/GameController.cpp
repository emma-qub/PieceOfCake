#include "ParserXML.h"
#include "GameController.h"

#include <QMessageBox>
#include <QDoubleValidator>
#include <QMap>

#include <algorithm>
#include <cassert>
#include <QTimer>

GameController::GameController(GameModel* model, QUndoStack* undoStack, QObject* parent):
  AbstractController(model, undoStack, parent),
  _model(model),
  _gameInfo(new GameInfo),
  _orientedAreaTotal(0.0),
  _maxGapToWin(0),
  _fileName(""),
  _levelRunning(false) {
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

void GameController::sliceIt(const Segment& line) {
  PolygonList newPolygonList;

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

      newPolygonList << Polygon(newVertices);
      newVertices.clear();
    }
  }
  _model->setPolygonList(newPolygonList);

  _gameInfo->setPartsCut(_model->getPolygonsCount());
  _gameInfo->setLinesDrawn(_gameInfo->linesDrawn()+1);

  emit update();

  checkWinning();
}

GameController::LineType GameController::computeLineType(const Segment& line) const {
  PolygonList polygonList = _model->getPolygonList();

  bool noCrossing = false;
  bool goodCrossing = false;
  bool badCrossing = false;

  foreach (const Polygon& polygon, polygonList) {
    if (!polygon.isCrossing(line) && !polygon.isPointInside2(line.getA())) {
      noCrossing = true;
    } else if (polygon.isGoodSegment(line)) {
      goodCrossing = true;
    } else {
      badCrossing = true;
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
  if (_gameInfo->linesDrawn() >= _gameInfo->linesCount()) {
    QList<float> orientedAreas;
    QList<Vector2d> shiftVectors;
    float minArea = 100.0;
    float maxArea = 0.0;

    for (const Polygon& polygon: _model->getPolygonList()) {
      float currArea = qRound(10.0*polygon.orientedArea() * 100.0 / _orientedAreaTotal)/10.0;
      orientedAreas << currArea;
      minArea = qMin(currArea, minArea);
      maxArea = qMax(currArea, maxArea);

      Vector2d currShift(computeGlobalBarycenter(), polygon.barycenter());
      float currShiftLength = currShift.norm();
      currShift.normalize();
      currShift *= 0.2*currShiftLength;
      shiftVectors << currShift;
    }

    translatePolygons(shiftVectors);

    float gap = qAbs(maxArea - minArea);

    if (_gameInfo->partsCut() != _gameInfo->partsCount() || gap > _maxGapToWin)
      emit levelEnd(orientedAreas);
    else {
      _gameInfo->setStars(6-qCeil(gap / _maxGapToWin * 5));
      emit levelEnd(orientedAreas);
    }

    _levelRunning = false;
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
  _gameInfo->setLinesCount(0);
  _gameInfo->setLinesCount(0);
  _gameInfo->setPartsCut(0);
  _gameInfo->setPartsCount(0);
  _gameInfo->setStars(0);
  _gameInfo->setStarsMax(0);
  _orientedAreaTotal = 0.0;
  _maxGapToWin = 0.0;
  _levelRunning = false;

  emit update();
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

  _gameInfo->setLinesDrawn(0);
  _gameInfo->setLinesCount(parser.getLinesCount());
  _gameInfo->setPartsCut(_model->getPolygonsCount());
  _gameInfo->setPartsCount(parser.getPartsCount());
  std::cerr << "## " << parser.getStarsCount() << std::endl;
  _gameInfo->setStarsMax(parser.getStarsCount());
  _maxGapToWin = parser.getMaxGapToWin();

  for (const Polygon& polygon: _model->getPolygonList()) {
    _orientedAreaTotal += polygon.orientedArea();
  }

  _levelRunning = true;

  emit update();
}



bool pointCompare(const Point2d* A, const Point2d* B) {
  return *A < *B;
}
