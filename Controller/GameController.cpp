#include "ParserXML.h"
#include "GameController.h"

#include <QMessageBox>
#include <QDoubleValidator>
#include <QMap>

GameController::GameController(GameModel* model, QWidget* tabWidget, QUndoStack* undoStack, QObject* parent):
  AbstractController(model, tabWidget, undoStack, parent),
  _model(model),
  _linesCount(-1),
  _partsCount(-1),
  _linesDrawn(-1),
  _polygonsCount(-1),
  _orientedAreaTotal(0.0),
  _maxGapToWin(0),
  _fileName("") {
}

void GameController::newComputeIntersections(const Polygon& polygon, const Segment& line, QVector<QPair<Point2d, Segment::Intersection>>& intersections, QMap<Segment, Point2d>& mapSegmentIntersection) const {
  std::vector<Point2d> vertices = polygon.getVertices();
  unsigned int verticesCount = vertices.size();

  for (unsigned int k = 0; k < verticesCount; ++k) {
    Point2d v0 = vertices.at(k);
    Point2d v1 = vertices.at((k+1)%verticesCount);
    Segment currEdge(v0, v1);

    Segment::Intersection intersectionType = currEdge.computeIntersection(line);
    switch (intersectionType) {
    case Segment::Regular:
    {
      Point2d currIntersection = Segment::intersectionPoint(currEdge, line);
      intersections << QPair<Point2d, Segment::Intersection>(currIntersection, intersectionType);
      mapSegmentIntersection.insert(currEdge, currIntersection);
      break;
    }
    case Segment::FirstVertexRegular:
    {
      Point2d v_1 = vertices.at((k-1)%verticesCount);
      if (!line.sameSide(v_1, v1)) {
        intersections << QPair<Point2d, Segment::Intersection>(v0, intersectionType);
        mapSegmentIntersection.insert(Segment(v0, v1), v0);
      }
      break;
    }
    default:
      break;
    }
  }

  qSort(intersections.begin(), intersections.end(), pairCompare);
}

void GameController::splitVertices(const Polygon& polygon, const Segment& line, QVector<Point2d>& rightVertices, QVector<Point2d>& leftVertices) const {
  std::vector<Point2d> vertices = polygon.getVertices();
  for (const Point2d& vertex: vertices) {
    Segment::Side side = line.location(vertex);
    if (side == Segment::OnLeft) {
      leftVertices << vertex;
    } else if (side == Segment::OnRight) {
      rightVertices << vertex;
    }
  }
}

QVector<Segment> GameController::computeNewEdges(const QVector<QPair<Point2d, Segment::Intersection>>& intersections) const {
  QVector<Segment> newEdges;

  Point2d fstPoint(-1, -1);
  bool isInside = false;

  for (const QPair<Point2d, Segment::Intersection>& intersection: intersections) {
    Point2d currPoint = intersection.first;
    Segment::Intersection currIntersectionType = intersection.second;
    if (!isInside && currIntersectionType == Segment::Regular) {
      fstPoint = currPoint;
      isInside = true;
    } else if (isInside && currIntersectionType == Segment::Regular) {
      newEdges << Segment(fstPoint, currPoint);
      isInside = false;
    } else if (!isInside && currIntersectionType == Segment::FirstVertexRegular) {
      if (currIntersectionType == Segment::FirstVertexRegular) {
        fstPoint = currPoint;
        isInside = true;
      }
    } else if (isInside && currIntersectionType == Segment::FirstVertexRegular) {
      if (currIntersectionType == Segment::FirstVertexRegular) {
        newEdges << Segment(fstPoint, currPoint);
        isInside = false;
      }
    }
  }

  return newEdges;
}

void GameController::shiftLineIfEdgeCut(const Polygon& polygon, Segment& line) const {
  std::vector<Point2d> vertices = polygon.getVertices();
  unsigned int verticesCount = vertices.size();

  for (unsigned int k = 0; k < verticesCount; ++k) {
    Point2d v0 = vertices.at(k);
    Point2d v1 = vertices.at((k+1)%verticesCount);
    Segment currEdge(v0, v1);

    if (currEdge.computeIntersection(line) == Segment::Edge) {
      line.setA(line.getA()+Point2d(1.0, 1.0));
      return;
    }

    if (currEdge.computeIntersection(line) == Segment::FirstVertexRegular) {
      line.setA(line.getA()+Point2d(1.0, 1.0));
      line.setB(line.getB()+Point2d(1.0, 1.0));
      return;
    }
  }
}

Point2d GameController::findOtherBound(const Point2d& fstBound, const QVector<Segment>& newEdges) const {
  for (const Segment& currSegment: newEdges) {
    if (currSegment.getA() == fstBound) {
      return currSegment.getB();
    } else if (currSegment.getB() == fstBound) {
      return currSegment.getA();
    }
  }

  return Point2d();
}

void GameController::createPolygons(QVector<Point2d>& oldPolygonVertices, std::vector<Point2d>& newPolygonVertices, const Polygon& oldPolygon, const QMap<Segment, Point2d>& mapSegmentIntersection, const QVector<Segment>& newEdges) const {
  std::vector<Point2d> polygonVertices = oldPolygon.getVertices();
  int verticesCount = polygonVertices.size();
  bool seekNewVertex = false;
  Point2d otherBound;
  Segment otherSegment;

  for (int k = 0; k < verticesCount; ++k) {
    Point2d v0 = polygonVertices.at(k);
    Point2d v1 = polygonVertices.at((k+1)%verticesCount);
    Segment currSegment(v0, v1);

    bool v0IsToBeAdded = oldPolygonVertices.contains(v0);
    bool v1IsToBeAdded = oldPolygonVertices.contains(v1);

    if (!seekNewVertex) {
      if (v0IsToBeAdded && v1IsToBeAdded) {
        newPolygonVertices.push_back(v0);
      } else if (v0IsToBeAdded && !v1IsToBeAdded) {
        newPolygonVertices.push_back(v0);
        Point2d currIntersection = mapSegmentIntersection.value(currSegment);
        if (currIntersection != Point2d()) {
          newPolygonVertices.push_back(currIntersection);
          seekNewVertex = true;
          otherBound = findOtherBound(currIntersection, newEdges);
          if (otherBound != Point2d())
            otherSegment = mapSegmentIntersection.key(otherBound);
        }
      } else if (!v0IsToBeAdded && v1IsToBeAdded) {
        Point2d currIntersection = mapSegmentIntersection.value(currSegment);
        if (currIntersection != Point2d()) {
          newPolygonVertices.push_back(currIntersection);
          seekNewVertex = false;
        }
      }
    } else {
      if (currSegment == otherSegment) {
        newPolygonVertices.push_back(mapSegmentIntersection.value(currSegment));
        seekNewVertex = false;
      }
    }
  }

  for (const Point2d& vertex: newPolygonVertices) {
    int vertexIndex = oldPolygonVertices.indexOf(vertex);
    if (vertexIndex != -1)
      oldPolygonVertices.remove(vertexIndex);
  }
}

void GameController::sliceIt(Segment& line) {
  PolygonList newPolygonList;
  for (const Polygon& polygon: _model->getPolygonList()) {
    shiftLineIfEdgeCut(polygon, line);

    QVector<QPair<Point2d, Segment::Intersection>> intersections;
    QMap<Segment, Point2d> mapSegmentIntersection;
    newComputeIntersections(polygon, line, intersections, mapSegmentIntersection);
    if (intersections.size() == 0) {
      newPolygonList << polygon;
      continue;
    }

    QVector<Point2d> rightVertices;
    QVector<Point2d> leftVertices;
    splitVertices(polygon, line, rightVertices, leftVertices);

    QVector<Segment> newEdges = computeNewEdges(intersections);

    std::vector<Point2d> newPolygonVertices;
    int k = 0;
    while (leftVertices.size() > 0) {
      createPolygons(leftVertices, newPolygonVertices, polygon, mapSegmentIntersection, newEdges);
      newPolygonList << Polygon(newPolygonVertices);
      newPolygonVertices.clear();
      if (++k > 2) break;
    }
    k = 0;
    while (rightVertices.size() > 0) {
      createPolygons(rightVertices, newPolygonVertices, polygon, mapSegmentIntersection, newEdges);
      newPolygonList << Polygon(newPolygonVertices);
      newPolygonVertices.clear();
      if (++k > 2) break;
    }
  }
  _model->setPolygonList(newPolygonList);

  _polygonsCount = _model->getPolygonsCount();
  _linesDrawn++;

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
  qDebug() << _linesDrawn << "/" << _linesCount;

  if (_linesDrawn >= _linesCount) {
    QList<float> orientedAreas;
    for (const Polygon& polygon: _model->getPolygonList()) {
      orientedAreas << qRound(10.0*polygon.orientedArea() * 100.0 / _orientedAreaTotal)/10.0;
    }
    qSort(orientedAreas);

    float gap = qAbs(orientedAreas.last() - orientedAreas.first());
    qDebug() << "Gap:" << gap << "%";

    if (_polygonsCount != _partsCount || gap > _maxGapToWin)
      emit levelEnd(_polygonsCount, _partsCount, orientedAreas, fail);
    else {
      qDebug() << gap << _maxGapToWin << gap / _maxGapToWin << gap / _maxGapToWin * 5;
      int rank = qCeil(gap / _maxGapToWin * 5);
      emit levelEnd(_polygonsCount, _partsCount, orientedAreas, Ranking(6-rank));
    }
  }
}

void GameController::replay(void) {
  openLevel(_fileName);
}

void GameController::clearGame(void) {
  _model->clearPolygons();
  _linesCount = 0;
  _partsCount = 0;
  _linesDrawn = 0;
  _polygonsCount = 0;
  _orientedAreaTotal = 0.0;
  _maxGapToWin = 0.0;

  emit update();
}

void GameController::openLevel(const QString& fileName) {
    clear();
    clearGame();

    ParserXML parser(fileName);

    _fileName = QString(fileName);

    PolygonList polygonList(parser.createPolygonList());
    _model->setPolygonList(polygonList);

    _linesCount = parser.getLinesCount();
    _partsCount = parser.getPartsCount();
    _polygonsCount = _model->getPolygonsCount();
    _maxGapToWin = parser.getmaxGapToWin();

    for (const Polygon& polygon: _model->getPolygonList()) {
      _orientedAreaTotal += polygon.orientedArea();
    }

    emit update();
}



bool pairCompare(const QPair<Point2d, Segment::Intersection>& fstElem, const QPair<Point2d, Segment::Intersection>& sndElem) {
  return fstElem.first < sndElem.first;
}
