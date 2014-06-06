#include "ParserXML.h"
#include "GameController.h"

#include <QMessageBox>
#include <QDoubleValidator>
#include <QMap>

GameController::GameController(GameModel* model, QWidget* tabWidget, QUndoStack* undoStack, QObject* parent) :
  AbstractController(model, tabWidget, undoStack, parent),
  _model(model),
  _newVertices(),
  _cuttingSegments(),
  _linesCount(-1),
  _partsCount(-1),
  _linesDrawn(-1),
  _polygonsCount(-1) {

}

//PolygonList GameController::splitSmartVertices(const QVector<QPair<Point2d, bool>>& smartVertices) const {
//    PolygonList polygons;
//    Polygon polygon;

//    QVector<QPair<Point2d, bool>> smartVerticesTmp(smartVertices);

//    bool seekNextNewVertex = false;
//    Point2d prevNewVertex;

//    int stop = 0;
//    while (smartVerticesTmp.size() > 0) {
//        QVector<QPair<Point2d, bool>> smartVerticesMutable;
//        unsigned int smartVerticesCount = smartVerticesTmp.size();

//        for (unsigned int k = 0; k < smartVerticesCount; k++) {
//            QPair<Point2d, bool> fstSmartVertex = smartVerticesTmp.at(k);

//            if (fstSmartVertex.second && seekNextNewVertex) {
//                Segment segment(prevNewVertex, fstSmartVertex.first);
//               if (isACuttingSegment(segment)) {
//                    polygon << fstSmartVertex.first;
//                    seekNextNewVertex = false;
//                } else {
//                    smartVerticesMutable.push_back(fstSmartVertex);
//                }
//            } else if (fstSmartVertex.second && !seekNextNewVertex) {
//                if (k != 0) {
//                    seekNextNewVertex = true;
//                    prevNewVertex = fstSmartVertex.first;
//                }
//                polygon << fstSmartVertex.first;
//            } else if (!fstSmartVertex.second && seekNextNewVertex) {
//                smartVerticesMutable.push_back(fstSmartVertex);
//            } else if (!fstSmartVertex.second && !seekNextNewVertex) {
//                polygon << fstSmartVertex.first;
//            }
//        }

//        seekNextNewVertex = false;

//        //if (polygon.hasEnoughVertices())
//            polygons << polygon;
//        polygon.clear();

//        smartVerticesTmp.clear();
//        smartVerticesTmp = smartVerticesMutable;

//        stop++;

//        if (stop == smartVertices.size()) {
//            break;
//            std::cerr << "Stop infinite loop from Polygon::splitSmartVertices." << std::endl;
//        }
//    }

//    return polygons;
//}

//PolygonList GameController::cutPolygon(const Polygon& currPolygon, const Segment& line) {
//    PolygonList polygons;
//    QVector<QPair<Point2d, bool>> smartVerticesLeft;
//    QVector<QPair<Point2d, bool>> smartVerticesRight;

//    _newVertices.clear();

//    if (!currPolygon.isCrossing(line)) {
//        polygons << currPolygon;
//        return polygons;
//    }

//    bool onLeft = true;
//    std::vector<Point2d> currVertices = currPolygon.getVertices();
//    unsigned int verticesCount = currVertices.size();

//    for (unsigned int k = 0; k < verticesCount; k++) {
//      qDebug() << "-----------------------------------------------------------";
//      qDebug() << "Vertex" << k;
//      qDebug() << "OnLeft:" << onLeft;

//        Point2d fstPoint(currVertices.at(k));
//        Point2d sndPoint(currVertices.at((k+1)%verticesCount));
//        Segment currSegment(fstPoint, sndPoint);
//        Segment::Intersection intersection = currSegment.computeIntersection(line);

//        switch (intersection) {
//        case Segment::Edge: {
//            qDebug() << "Intersection: Edge";
//            Point2d prevPoint(currVertices.at((k-1)%verticesCount));
//            Point2d nextPoint(currVertices.at((k+2)%verticesCount));
//            bool isCutting = !line.sameSide(prevPoint, nextPoint);
//            qDebug() << "is cutting?" << isCutting;
//            if (isCutting && onLeft) {
//                smartVerticesLeft.removeLast();
//                smartVerticesLeft.push_back(QPair<Point2d, bool>(sndPoint, true));
//                _newVertices.push_back(fstPoint);
//                _newVertices.push_back(sndPoint);
//            } else if (isCutting && !onLeft) {
//                if (smartVerticesRight.size() >= 1) {
//                    smartVerticesRight.removeLast();
//                    if (smartVerticesRight.size() >= 1) {
//                        smartVerticesRight.removeLast();
//                    }
//                }
//                smartVerticesRight.push_back(QPair<Point2d, bool>(sndPoint, true));
//                smartVerticesRight.push_back(QPair<Point2d, bool>(nextPoint, false));
//                _newVertices.push_back(sndPoint);
//                _newVertices.push_back(sndPoint);
//            } else if (!isCutting && onLeft) {
//                smartVerticesLeft.removeLast();
//                smartVerticesRight.push_back(QPair<Point2d, bool>(sndPoint, true));
//                smartVerticesRight.push_back(QPair<Point2d, bool>(nextPoint, false));
//                _newVertices.push_back(sndPoint);
//                _newVertices.push_back(sndPoint);
//                onLeft = false;
//                k++;
//            }

//            break;
//        }
//        case Segment::Regular: {
//          qDebug() << "Intersection: Regular";
//            Point2d intersectionPoint(Segment::intersectionPoint(currSegment, line));
//            if (onLeft) {
//                smartVerticesLeft.push_back(QPair<Point2d, bool>(fstPoint, false));
//                smartVerticesLeft.push_back(QPair<Point2d, bool>(intersectionPoint, true));
//                smartVerticesRight.push_back(QPair<Point2d, bool>(intersectionPoint, true));
//                smartVerticesRight.push_back(QPair<Point2d, bool>(sndPoint, false));
//            } else {
//                smartVerticesRight.push_back(QPair<Point2d, bool>(intersectionPoint, true));
//                smartVerticesLeft.push_back(QPair<Point2d, bool>(intersectionPoint, true));
//            }
//            _newVertices.push_back(intersectionPoint);
//            onLeft = !onLeft;
//            break;
//        }
//        case Segment::None:
//          qDebug() << "Intersection: None";
//            if (onLeft) {
//                smartVerticesLeft.push_back(QPair<Point2d, bool>(fstPoint, false));
//            } else {
//                smartVerticesRight.push_back(QPair<Point2d, bool>(sndPoint, false));
//            }
//            break;
//        case Segment::FirstVertexRegular: {
//          qDebug() << "Intersection: FirstVertexRegular";
//            Point2d prevPoint(currVertices.at((k-1)%verticesCount));
//            bool isCutting = !line.sameSide(prevPoint, sndPoint);
//            if (!isCutting && onLeft) {
//              smartVerticesLeft.removeLast();
//              smartVerticesLeft.push_back(QPair<Point2d, bool>(fstPoint, false));
//              _newVertices.removeLast();
//            } else if (!isCutting && !onLeft) {
//              smartVerticesRight.removeAt(smartVerticesRight.size()-2);
//              smartVerticesRight.insert(smartVerticesRight.end()-1, QPair<Point2d, bool>(fstPoint, false));
//              _newVertices.removeLast();
//            }
////            if (!isCutting && onLeft) {
////              smartVerticesLeft.push_back(QPair<Point2d, bool>(fstPoint, true));
//////              smartVerticesRight.push_back(QPair<Point2d, bool>(fstPoint, false));
////              _newVertices.push_back(fstPoint);
////            }
////            else if (!isCutting && !onLeft) {
//////              smartVerticesLeft.push_back(QPair<Point2d, bool>(fstPoint, false));
////              smartVerticesRight.push_back(QPair<Point2d, bool>(fstPoint, true));
////              _newVertices.push_back(fstPoint);
////            }
//            break;
//        }
//        case Segment::SecondVertexRegular: {
//          qDebug() << "Intersection: SecondVertexRegular";
//            Point2d nextPoint(currVertices.at((k+2)%verticesCount));
//            bool isCutting = !line.sameSide(fstPoint, nextPoint);
//            qDebug() << "isCutting?" << isCutting;

//            if (onLeft && isCutting) {
//                smartVerticesLeft.push_back(QPair<Point2d, bool>(fstPoint, false));
//                smartVerticesLeft.push_back(QPair<Point2d, bool>(sndPoint, true));
//                smartVerticesRight.push_back(QPair<Point2d, bool>(sndPoint, true));
//                smartVerticesRight.push_back(QPair<Point2d, bool>(nextPoint, false));
//                _newVertices.push_back(sndPoint);
//            } else if (onLeft && !isCutting) {
//                smartVerticesLeft.push_back(QPair<Point2d, bool>(fstPoint, false));
//                smartVerticesLeft.push_back(QPair<Point2d, bool>(sndPoint, true));
////                smartVerticesLeft.push_back(QPair<Point2d, bool>(sndPoint, true));
////                _newVertices.push_back(sndPoint);
//                _newVertices.push_back(sndPoint);
//            } else if (!onLeft && !isCutting) {
////                smartVerticesRight.push_back(QPair<Point2d, bool>(sndPoint, true));
//                smartVerticesRight.push_back(QPair<Point2d, bool>(sndPoint, true));
//                smartVerticesRight.push_back(QPair<Point2d, bool>(nextPoint, false));
////                _newVertices.push_back(sndPoint);
//                _newVertices.push_back(sndPoint);
//            } else if (!onLeft && isCutting) {
//                smartVerticesLeft.push_back(QPair<Point2d, bool>(sndPoint, true));
//                smartVerticesRight.push_back(QPair<Point2d, bool>(sndPoint, true));
//                _newVertices.push_back(sndPoint);
//            }

//            if (isCutting) {
//                onLeft = !onLeft;
//            }
//            break;
//        }
//        default:
//            break;
//        }
//        qDebug() << "SVLeft:" << smartVerticesLeft;
//        qDebug() << "SVRight:" << smartVerticesRight;
//        qDebug() << "NewVertices:" << _newVertices;
//        qDebug() << "OnLeft:" << onLeft;
//        qDebug() << "-----------------------------------------------------------\n";
//    }

//    computeCuttingSegments();

//    PolygonList polygonsOnLeft = splitSmartVertices(smartVerticesLeft);
//    foreach (const Polygon& polygon, polygonsOnLeft)
//        polygons << polygon;

//    PolygonList polygonsOnRight = splitSmartVertices(smartVerticesRight);
//    foreach (const Polygon& polygon, polygonsOnRight)
//        polygons << polygon;

//    return polygons;
//}

//PolygonList GameController::cutPolygons(const Segment& line) {
//    PolygonList polygons;

//    PolygonList polygonsToCut = _model->getPolygonList();
//    foreach (const Polygon& currPolygon, polygonsToCut) {
//        polygons << cutPolygon(currPolygon, line);
//    }

//    return polygons;
//}

//void GameController::computeCuttingSegments(void) {
//    _cuttingSegments.clear();

//    qSort(_newVertices.begin(), _newVertices.end());
//    qDebug() << "###################################################";
//    qDebug() << _newVertices;
//    qDebug() << "###################################################";

//    for (int k = 0; k < _newVertices.size()-1; k += 2) {
//        _cuttingSegments.push_back(Segment(_newVertices.at(k), _newVertices.at((k+1)%_newVertices.size())));
//        if (k >= 2 && _newVertices.size() >= 4) {
//          if (_newVertices.at(k-1) == _newVertices.at(k)) {
//            _cuttingSegments.push_back(Segment(_newVertices.at(k-2), _newVertices.at((k+1)%_newVertices.size())));
//          }
//        }
//    }
//    qDebug() << "###################################################";
//    qDebug() << _cuttingSegments;
//    qDebug() << "###################################################";
//}

//QVector<QPair<Point2d, Segment::Intersection>> GameController::computeIntersections(const Polygon& polygon, const Segment& line) const {
//    QVector<QPair<Point2d, Segment::Intersection>> res;
//    QMap<Point2d, Segment::Intersection> tmp;

//    QVector<Point2d> intersectionPoints;
//    QVector<Segment::Intersection> intersectionTypes;

//    std::vector<Point2d> vertices = polygon.getVertices();
//    unsigned int vertexCount = vertices.size();
//    for (unsigned int k = 0; k < vertexCount; ++k) {
//        Point2d vertex0 = vertices.at(k);
//        Point2d vertex1 = vertices.at((k+1)%vertexCount);
//        Segment edge(vertex0, vertex1);

//        Segment::Intersection currIntersectionType = edge.computeIntersection(line);
//        if (currIntersectionType == Segment::None)
//            continue;

//        if (currIntersectionType == Segment::Edge) {
////            intersectionPoints.push_back(vertex0);
////            intersectionTypes.push_back(currIntersectionType);
////            tmp.insert(vertex0, currIntersectionType);
////            intersectionPoints.push_back(vertex1);
////            intersectionTypes.push_back(currIntersectionType);
////            tmp.insert(vertex1, currIntersectionType);
//            continue;
//        }

//        Point2d currIntersectionPoint;
//        if (currIntersectionType == Segment::FirstVertexRegular) {
//            currIntersectionPoint = vertices.at(k);
//        } else if(currIntersectionType == Segment::SecondVertexRegular) {
//            currIntersectionPoint = vertices.at((k+1)%vertexCount);
//        } else {
//            currIntersectionPoint = Segment::intersectionPoint(edge, line);
//        }

//        intersectionPoints.push_back(currIntersectionPoint);
//        intersectionTypes.push_back(currIntersectionType);
//        tmp.insert(currIntersectionPoint, currIntersectionType);
//    }

//    if (qAbs(intersectionPoints.first().getX() - intersectionPoints.last().getX()) < 2) {
//        qDebug() << "bouuh";
//        for (int k = 0; k < intersectionPoints.count(); ++k) {
//            Point2d currPoint = intersectionPoints.at(k);
//            currPoint.symetry();
//            intersectionPoints.replace(k, currPoint);
//        }
//        qSort(intersectionPoints);
//        for (int k = 0; k < intersectionPoints.count(); ++k) {
//            Point2d currPoint = intersectionPoints.at(k);
//            currPoint.symetry();
//            intersectionPoints.replace(k, currPoint);
//        }
//    } else {
//        qSort(intersectionPoints);
//    }
//    qDebug() << intersectionPoints;

////    for (unsigned int k = 0; k < vertexCount; ++k) {

////        Point2d vertex1 = vertices.at((k+1)%vertexCount);
////        Point2d vertex2 = vertices.at((k+2)%vertexCount);
////        Segment edge1(vertex1, vertex2);

////        Segment::Intersection currIntersectionType = edge1.computeIntersection(line);
////        if (currIntersectionType == Segment::SecondVertexRegular) {
////            Point2d vertex3 = vertices.at((k+3)%vertexCount);
////            Segment edge2(vertex2, vertex3);
////            Segment::Intersection nextIntersectionType = edge2.computeIntersection(line);
////            if (nextIntersectionType == Segment::Edge) {
////                Point2d vertex0 = vertices.at(k);
////                if (edge2.sameSide(vertex0, vertex3)) {
////                    QMap<Point2d, Segment::Intersection>::iterator it = tmp.find(vertex1);
////                    it.value() = Segment::EdgeUseless;
////                    it = tmp.find(vertex2);
////                    it.value() = Segment::EdgeUseless;
////                }
////            }
////        }
////    }

//    for (int k = 0; k < intersectionPoints.size(); ++k) {
//        Point2d currIntersectionPoint = intersectionPoints.at(k);

//        Segment::Intersection currIntersectionType = tmp.value(currIntersectionPoint);

//        res.push_back(QPair<Point2d, Segment::Intersection>(currIntersectionPoint, currIntersectionType));

//        qDebug() << "##" << currIntersectionPoint;
//        qDebug() << "##" << res;
//    }

//    qDebug() << res;

//    return res;
//}

//QList<Segment> GameController::computeSegments(const Polygon& polygon, const QVector<QPair<Point2d, Segment::Intersection>>& intersections) const {
//    QList<Segment> res;

//    int size = intersections.count();
//    for (int k = 0; k < size; ++k) {
//        QPair<Point2d, Segment::Intersection> intersection = intersections.at(k);

//        Point2d fstPoint = intersection.first;
//        if (k < size-1) {
//            QPair<Point2d, Segment::Intersection> nextIntersection = intersections.at(k+1);
//            Point2d sndPoint = nextIntersection.first;
//            Segment::Intersection sndIntersection = nextIntersection.second;

//            Point2d middlePoint = (fstPoint+sndPoint)/2;
//            if (polygon.isPointInside2(middlePoint)) {
//                res << Segment(fstPoint, sndPoint);
//            }

//            if (sndIntersection == Segment::FirstVertexRegular) {
//                if (k < size-2) {
//                    QPair<Point2d, Segment::Intersection> nextNextIntersection = intersections.at(k+2);
//                    Point2d thdPoint = nextNextIntersection.first;

//                    Point2d sndMiddlePoint = (sndPoint+thdPoint)/2;
//                    if (polygon.isPointInside2(sndMiddlePoint)) {
//                        res << Segment(fstPoint, thdPoint);
//                    }
//                }
//            }
//        }
//    }

//    qDebug() << "<<" << res;

//    return res;
//}

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
      //mapSegmentIntersection.insert(currEdge, currIntersection);
      break;
    }
    case Segment::FirstVertexRegular:
    {
      Point2d v_1 = vertices.at((k-1)%verticesCount);
      if (!line.sameSide(v_1, v1)) {
        Point2d currIntersection = Segment::intersectionPoint(currEdge, line);
        intersections << QPair<Point2d, Segment::Intersection>(v0, intersectionType);
        //mapSegmentIntersection.insert(Segment(v0, v1), currIntersection);
      }
//      else
//        intersections << QPair<Point2d, Segment::Intersection>(v0, Segment::VertexUseless);
      break;
    }
//    case Segment::SecondVertexRegular:
//    {
//      Point2d v2 = vertices.at((k+2)%verticesCount);
//      if (line.sameSide(v0, v2))
//        intersections << QPair<Point2d, Segment::Intersection>(v1, Segment::VertexUseless);
//      else
//        intersections << QPair<Point2d, Segment::Intersection>(v1, intersectionType);
//      break;
//    }
//    case Segment::Edge:
//    {
//      Point2d v_1 = vertices.at((k-1)%verticesCount);
//      Point2d v2 = vertices.at((k+2)%verticesCount);
//      if (line.sameSide(v_1, v2)) {
//        intersections << QPair<Point2d, Segment::Intersection>(v1, Segment::EdgeUseless);
//        qDebug() << "EdgeUseless" << v0 << v1;
//      } else {
//        intersections << QPair<Point2d, Segment::Intersection>(v1, intersectionType);
//        qDebug() << "Edge" << v0 << v1;
//      }
//      break;
//    }
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
    qDebug() << "Dealing with" << intersection;
    Point2d currPoint = intersection.first;
    Segment::Intersection currIntersectionType = intersection.second;
    if (!isInside && currIntersectionType == Segment::Regular) {
      fstPoint = currPoint;
      isInside = true;
    } else if (isInside && currIntersectionType == Segment::Regular) {
      newEdges << Segment(fstPoint, currPoint);
      isInside = false;
    }/* else if (isInside && (currIntersectionType == Segment::Edge || currIntersectionType == Segment::EdgeUseless)) {
      if (currIntersectionType == Segment::Edge) {
        qDebug() << "########### Edge" << fstPoint << currPoint;
        newEdges << Segment(fstPoint, currPoint);
        isInside = false;
      } else if (currIntersectionType == Segment::EdgeUseless) {
        qDebug() << "########### EdgeUseless" << fstPoint << currPoint;
        newEdges << Segment(fstPoint, currPoint);
        isInside = true;
      }
    }*/ else if (!isInside && (currIntersectionType == Segment::FirstVertexRegular/* || currIntersectionType == Segment::VertexUseless*/)) {
      if (currIntersectionType == Segment::FirstVertexRegular) {
        fstPoint = currPoint;
        isInside = true;
      }
    } else if (isInside && (currIntersectionType == Segment::FirstVertexRegular/* || currIntersectionType == Segment::VertexUseless*/)) {
      if (currIntersectionType == Segment::FirstVertexRegular) {
        newEdges << Segment(fstPoint, currPoint);
        isInside = false;
      }/* else if (currIntersectionType == Segment::VertexUseless) {
        newEdges << Segment(fstPoint, currPoint);
        fstPoint = currPoint;
      }*/
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

  return Point2d(-1.0, -1.0);
}

void GameController::createPolygons(QVector<Point2d>& oldPolygonVertices, std::vector<Point2d>& newPolygonVertices, const Polygon& oldPolygon, const QMap<Segment, Point2d>& mapSegmentIntersection, const QVector<Segment>& newEdges) const {
  std::vector<Point2d> polygonVertices = oldPolygon.getVertices();
  int verticesCount = polygonVertices.size();
  bool seekNewVertex = false;
  Point2d otherBound(-1.0, -1.0);

  for (int k = 0; k < verticesCount; ++k) {
    Point2d v0 = oldPolygonVertices.at(k);
    Point2d v1 = oldPolygonVertices.at((k+1)%verticesCount);
    Segment currSegment(v0, v1);

    bool v0IsToBeAdd = oldPolygonVertices.contains(v0);
    bool v1IsToBeAdd = oldPolygonVertices.contains(v1);

    if (v0IsToBeAdd && v1IsToBeAdd) {
      newPolygonVertices.push_back(v0);
    } else if (v0IsToBeAdd && !v1IsToBeAdd) {
      //Point2d currIntersection = mapSegmentIntersection.value(Segment(v0, v1));
      newPolygonVertices.push_back(v0);
      //newPolygonVertices.push_back(currIntersection);
      seekNewVertex = true;
      //otherBound = findOtherBound(currIntersection, newEdges);
    } else if (!v0IsToBeAdd && v1IsToBeAdd) {
      //newPolygonVertices.push_back(mapSegmentIntersection.value(Segment(v0, v1)));
      seekNewVertex = false;
    }

    if (!seekNewVertex) {
      newPolygonVertices.push_back(v0);
//      Point2d currIntersection = mapSegmentIntersection.value(currSegment, Point2d(-1.0, -1.0));
//      if (currIntersection != Point2d(-1.0, -1.0)) {
//        newPolygonVertices.push_back(currIntersection);
//        seekNewVertex = true;
//      }
    }
  }
}

void GameController::sliceIt(Segment& line) {
  //QVector<Polygon> outputPolygons;

//    for (const Polygon& polygon: _model->getPolygonList()) {
//        QVector<QPair<Point2d, Segment::Intersection>> intersections = computeIntersections(polygon, line);
//        computeSegments(polygon, intersections);


//    }

//    _model->setPolygonList(cutPolygons(line));

//    _polygonsCount = _model->getPoligonsCount();
//    _linesDrawn++;

//    emit update();

//    if (_polygonsCount > _partsCount) {
//      QMessageBox::critical(_tabWidget, tr("Too many parts"), tr("There are too many parts"));
//    } else if (_linesDrawn == _linesCount && _polygonsCount < _partsCount) {
//      QMessageBox::critical(_tabWidget, tr("Too few parts"), tr("There are too few parts"));
//    } else if (_linesDrawn == _linesCount && _polygonsCount == _partsCount) {
//      QMessageBox::information(_tabWidget, tr("Success"), tr("Well done!"));
//    }

  for (const Polygon& polygon: _model->getPolygonList()) {
    shiftLineIfEdgeCut(polygon, line);
    std::cerr << line << std::endl;

    QVector<QPair<Point2d, Segment::Intersection>> intersections;
    QMap<Segment, Point2d> mapSegmentIntersection;
    newComputeIntersections(polygon, line, intersections, mapSegmentIntersection);
    qDebug() << "### Intersections ###\n" << intersections << "\n";

    QVector<Point2d> rightVertices;
    QVector<Point2d> leftVertices;
    splitVertices(polygon, line, rightVertices, leftVertices);
    qDebug() << "### Right Vertices ###\n" << rightVertices << "\n";
    qDebug() << "### Left Vertices ###\n" << leftVertices << "\n";

    QVector<Segment> newEdges = computeNewEdges(intersections);
    qDebug() << "### New Edges ###\n" << newEdges << "\n";

    std::vector<Point2d> newPolygonVertices;
    createPolygons(leftVertices, newPolygonVertices, polygon, mapSegmentIntersection, newEdges);

    Polygon newPolygon(newPolygonVertices);
    _model->setPolygonList(PolygonList() << newPolygon);

    _polygonsCount = _model->getPolygonsCount();
    _linesDrawn++;

    emit update();

  }
}

//bool GameController::isACuttingSegment(const Segment& segment) const {
//    Segment usedSegment(segment);
//    if (segment.getA() > segment.getB()) {
//        usedSegment.setA(segment.getB());
//        usedSegment.setB(segment.getA());
//    }

//    for (const Segment& currSegment: _cuttingSegments) {
//        if (usedSegment == currSegment) {
//            return true;
//        }
//    }
//    return false;
//}

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

void GameController::openLevel(const QString& fileName) {
    clear();

    ParserXML parser(fileName);

    PolygonList polygonList(parser.createPolygonList());
    _model->setPolygonList(polygonList);

    _linesCount = parser.getLinesCount();
    _partsCount = parser.getPartsCount();
    _linesDrawn = 0;
    _polygonsCount = _model->getPolygonsCount();

    emit update();
}

bool pairCompare(const QPair<Point2d, Segment::Intersection>& fstElem, const QPair<Point2d, Segment::Intersection>& sndElem) {
  return fstElem.first < sndElem.first;
}
