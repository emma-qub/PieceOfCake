#include "ParserXML.h"
#include "GameController.h"

#include <QMessageBox>

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

PolygonList GameController::splitSmartVertices(const std::vector<std::pair<Point2d, bool>>& smartVertices) const {
    PolygonList polygons;
    Polygon polygon;

    std::vector<std::pair<Point2d, bool>> smartVerticesTmp(smartVertices);

    bool seekNextNewVertex = false;
    Point2d prevNewVertex;

    unsigned int stop = 0;
    while (smartVerticesTmp.size() > 0) {
        std::vector<std::pair<Point2d, bool>> smartVerticesMutable;
        unsigned int smartVerticesCount = smartVerticesTmp.size();

        for (unsigned int k = 0; k < smartVerticesCount; k++) {
            std::pair<Point2d, bool> fstSmartVertex = smartVerticesTmp.at(k);


            if (fstSmartVertex.second && seekNextNewVertex) {
                Segment segment(prevNewVertex, fstSmartVertex.first);
               if (isACuttingSegment(segment)) {
                    polygon << fstSmartVertex.first;
                    seekNextNewVertex = false;
                } else {
                    smartVerticesMutable.push_back(fstSmartVertex);
                }
            } else if (fstSmartVertex.second && !seekNextNewVertex) {
                if (k != 0) {
                    seekNextNewVertex = true;
                    prevNewVertex = fstSmartVertex.first;
                }
                polygon << fstSmartVertex.first;
            } else if (!fstSmartVertex.second && seekNextNewVertex) {
                smartVerticesMutable.push_back(fstSmartVertex);
            } else if (!fstSmartVertex.second && !seekNextNewVertex) {
                polygon << fstSmartVertex.first;
            }
        }

        seekNextNewVertex = false;

        //if (polygon.hasEnoughVertices())
            polygons << polygon;
        polygon.clear();

        smartVerticesTmp.clear();
        smartVerticesTmp = smartVerticesMutable;

        stop++;

        if (stop == smartVertices.size()) {
            break;
            std::cerr << "Stop infinite loop from Polygon::splitSmartVertices." << std::endl;
        }
    }

    return polygons;
}

PolygonList GameController::cutPolygon(const Polygon& currPolygon, const Segment& line) {
    PolygonList polygons;
    std::vector<std::pair<Point2d, bool>> smartVerticesLeft;
    std::vector<std::pair<Point2d, bool>> smartVerticesRight;

    _newVertices.clear();

    if (!currPolygon.isCrossing(line)) {
        polygons << currPolygon;
        return polygons;
    }

    bool onLeft = true;
    std::vector<Point2d> currVertices = currPolygon.getVertices();
    unsigned int verticesCount = currVertices.size();

    for (unsigned int k = 0; k < verticesCount; k++) {
        Point2d fstPoint(currVertices.at(k));
        Point2d sndPoint(currVertices.at((k+1)%verticesCount));
        Segment currSegment(fstPoint, sndPoint);
        Segment::Intersection intersection = currSegment.computeIntersection(line);

        switch (intersection) {
        case Segment::Edge: {
          qDebug() << "Edge";
          Point2d prevPoint(currVertices.at((k-1)%verticesCount));
          Point2d nextNextPoint(currVertices.at((k+2)%verticesCount));
          bool isCutting = !line.sameSide(prevPoint, nextNextPoint);

          if (onLeft && isCutting) {
//              smartVerticesLeft.push_back(std::pair<Point2d, bool>(fstPoint, false));
//              smartVerticesLeft.push_back(std::pair<Point2d, bool>(sndPoint, true));
//              smartVerticesRight.push_back(std::pair<Point2d, bool>(sndPoint, true));
              smartVerticesRight.push_back(std::pair<Point2d, bool>(nextNextPoint, false));
              _newVertices.push_back(nextNextPoint);
          } else if (onLeft && !isCutting) {
//              smartVerticesLeft.push_back(std::pair<Point2d, bool>(fstPoint, false));
//              smartVerticesLeft.push_back(std::pair<Point2d, bool>(sndPoint, true));
//              smartVerticesLeft.push_back(std::pair<Point2d, bool>(sndPoint, true));
              _newVertices.push_back(nextNextPoint);
              _newVertices.push_back(nextNextPoint);
          } else if (!onLeft && !isCutting) {
//              smartVerticesRight.push_back(std::pair<Point2d, bool>(sndPoint, true));
//              smartVerticesRight.push_back(std::pair<Point2d, bool>(sndPoint, true));
              smartVerticesRight.push_back(std::pair<Point2d, bool>(nextNextPoint, false));
              _newVertices.push_back(nextNextPoint);
              _newVertices.push_back(nextNextPoint);
          } else if (!onLeft && isCutting) {
//              smartVerticesLeft.push_back(std::pair<Point2d, bool>(sndPoint, true));
//              smartVerticesRight.push_back(std::pair<Point2d, bool>(sndPoint, true));
              _newVertices.push_back(nextNextPoint);
          }

          if (isCutting) {
              onLeft = !onLeft;
          }

          k++;
          break;
        }
        case Segment::Regular: {
            Point2d intersectionPoint(Segment::intersectionPoint(currSegment, line));
            if (onLeft) {
                smartVerticesLeft.push_back(std::pair<Point2d, bool>(fstPoint, false));
                smartVerticesLeft.push_back(std::pair<Point2d, bool>(intersectionPoint, true));
                smartVerticesRight.push_back(std::pair<Point2d, bool>(intersectionPoint, true));
                smartVerticesRight.push_back(std::pair<Point2d, bool>(sndPoint, false));
            } else {
                smartVerticesRight.push_back(std::pair<Point2d, bool>(intersectionPoint, true));
                smartVerticesLeft.push_back(std::pair<Point2d, bool>(intersectionPoint, true));
            }
            _newVertices.push_back(intersectionPoint);
            onLeft = !onLeft;
            break;
        }
        case Segment::None:
            if (onLeft) {
                smartVerticesLeft.push_back(std::pair<Point2d, bool>(fstPoint, false));
            } else {
                smartVerticesRight.push_back(std::pair<Point2d, bool>(sndPoint, false));
            }
            break;
        case Segment::FirstVertexRegular:
            break;
        case Segment::SecondVertexRegular: {
            Point2d nextPoint(currVertices.at((k+2)%verticesCount));
            bool isCutting = !line.sameSide(fstPoint, nextPoint);

            if (onLeft && isCutting) {
                smartVerticesLeft.push_back(std::pair<Point2d, bool>(fstPoint, false));
                smartVerticesLeft.push_back(std::pair<Point2d, bool>(sndPoint, true));
                smartVerticesRight.push_back(std::pair<Point2d, bool>(sndPoint, true));
                smartVerticesRight.push_back(std::pair<Point2d, bool>(nextPoint, false));
                _newVertices.push_back(sndPoint);
            } else if (onLeft && !isCutting) {
                smartVerticesLeft.push_back(std::pair<Point2d, bool>(fstPoint, false));
                smartVerticesLeft.push_back(std::pair<Point2d, bool>(sndPoint, true));
                smartVerticesLeft.push_back(std::pair<Point2d, bool>(sndPoint, true));
                _newVertices.push_back(sndPoint);
                _newVertices.push_back(sndPoint);
            } else if (!onLeft && !isCutting) {
                smartVerticesRight.push_back(std::pair<Point2d, bool>(sndPoint, true));
                smartVerticesRight.push_back(std::pair<Point2d, bool>(sndPoint, true));
                smartVerticesRight.push_back(std::pair<Point2d, bool>(nextPoint, false));
                _newVertices.push_back(sndPoint);
                _newVertices.push_back(sndPoint);
            } else if (!onLeft && isCutting) {
                smartVerticesLeft.push_back(std::pair<Point2d, bool>(sndPoint, true));
                smartVerticesRight.push_back(std::pair<Point2d, bool>(sndPoint, true));
                _newVertices.push_back(sndPoint);
            }

            if (isCutting) {
                onLeft = !onLeft;
            }
            break;
        }
        }
    }

    computeCuttingSegments();

    PolygonList polygonsOnLeft = splitSmartVertices(smartVerticesLeft);
    foreach (const Polygon& polygon, polygonsOnLeft)
        polygons << polygon;

    PolygonList polygonsOnRight = splitSmartVertices(smartVerticesRight);
    foreach (const Polygon& polygon, polygonsOnRight)
        polygons << polygon;

    return polygons;
}

PolygonList GameController::cutPolygons(const Segment& line) {
    PolygonList polygons;

    PolygonList polygonsToCut = _model->getPolygonList();
    foreach (const Polygon& currPolygon, polygonsToCut) {
        polygons << cutPolygon(currPolygon, line);
    }

    return polygons;
}

void GameController::computeCuttingSegments(void) {
    _cuttingSegments.clear();

    std::sort(_newVertices.begin(), _newVertices.end());

    for (unsigned int k = 0; k < _newVertices.size()-1; k += 2) {
        _cuttingSegments.push_back(Segment(_newVertices.at(k), _newVertices.at((k+1)%_newVertices.size())));
    }
}

void GameController::sliceIt(const Segment& line) {
    _model->setPolygonList(cutPolygons(line));

    _polygonsCount = _model->getPoligonsCount();
    _linesDrawn++;

    emit update();

//    if (_polygonsCount > _partsCount) {
//      QMessageBox::critical(_tabWidget, tr("Too many parts"), tr("There are too many parts"));
//    } else if (_linesDrawn == _linesCount && _polygonsCount < _partsCount) {
//      QMessageBox::critical(_tabWidget, tr("Too few parts"), tr("There are too few parts"));
//    } else if (_linesDrawn == _linesCount && _polygonsCount == _partsCount) {
//      QMessageBox::information(_tabWidget, tr("Success"), tr("Well done!"));
//    }

}

bool GameController::isACuttingSegment(const Segment& segment) const {
    Segment usedSegment(segment);
    if (segment.getA() > segment.getB()) {
        usedSegment.setA(segment.getB());
        usedSegment.setB(segment.getA());
    }

    for (const Segment& currSegment: _cuttingSegments) {
        if (usedSegment == currSegment) {
            return true;
        }
    }
    return false;
}

GameController::LineType GameController::computeLineType(const Segment& line) const {
    PolygonList polygonList = _model->getPolygonList();

    bool noCrossing = false;
    bool goodCrossing = false;
    bool badCrossing = false;

    foreach (const Polygon& polygon, polygonList) {
        if (!polygon.isCrossing(line) && !polygon.isPointInside(line.getA())) {
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
    _polygonsCount = _model->getPoligonsCount();

    emit update();
}
