#include "GameController.h"

GameController::GameController(GameModel* model, QObject* parent) :
  QObject(parent),
  _model(model),
  _newVertices(),
  _cuttingSegments() {
}

PolygonList GameController::splitSmartVertices(const std::vector<std::pair<Point2d, bool>>& smartVertices) const {
//    std::vector<Polygon> polygons;
//    Polygon polygon;

//    std::vector<std::pair<Point2d, bool>> smartVerticesTmp(smartVertices);

//    bool seekNextNewVertex = false;
//    Point2d prevNewVertex;

//    unsigned int stop = 0;
//    while (smartVerticesTmp.size() > 0) {
//        std::vector<std::pair<Point2d, bool>> smartVerticesMutable;
//        unsigned int smartVerticesCount = smartVerticesTmp.size();


////        QString mess = "";
////        for (const std::pair<Point2d, bool>& p: smartVerticesTmp)
////            mess += " ("+QString::number(p.first.getX())+"; "+QString::number(p.first.getY())+" "+QString::number(p.second)+") ";
////        std::cerr << "## " << mess.toStdString() << std::endl << std::endl;


//        for (unsigned int k = 0; k < smartVerticesCount; k++) {
//            std::pair<Point2d, bool> fstSmartVertex = smartVerticesTmp.at(k);

////            std::cerr << "Seek next new vertex: " << std::boolalpha << seekNextNewVertex << std::endl;

//            if (fstSmartVertex.second && seekNextNewVertex) {
//                Segment segment(prevNewVertex, fstSmartVertex.first);
////                std::cerr << "Segment: " << segment << std::endl;
//                if (isACuttingSegment(segment)) {
////                    std::cerr << "Is a cutting segment: true" << std::endl;
//                    polygon << fstSmartVertex.first;
////                    std::cerr << "Adding " << fstSmartVertex.first << " to polygon" << std::endl;
//                    seekNextNewVertex = false;
//                } else {
////                    std::cerr << "Is a cutting segment: false" << std::endl;
//                    smartVerticesMutable.push_back(fstSmartVertex);
////                    std::cerr << "Adding " << fstSmartVertex.first << " to smart vertices" << std::endl;
//                }
//            } else if (fstSmartVertex.second && !seekNextNewVertex) {
//                if (k != 0) {
//                    seekNextNewVertex = true;
//                    prevNewVertex = fstSmartVertex.first;
//                }
//                polygon << fstSmartVertex.first;
////                std::cerr << "Adding " << fstSmartVertex.first << " to polygon" << std::endl;
//            } else if (!fstSmartVertex.second && seekNextNewVertex) {
//                smartVerticesMutable.push_back(fstSmartVertex);
////                std::cerr << "Adding " << fstSmartVertex.first << " to smart vertices" << std::endl;
//            } else if (!fstSmartVertex.second && !seekNextNewVertex) {
//                polygon << fstSmartVertex.first;
////                std::cerr << "Adding " << fstSmartVertex.first << " to polygon" << std::endl;
//            }
//        }

//        seekNextNewVertex = false;

//        polygons.push_back(polygon);
//        polygon.clear();

//        smartVerticesTmp.clear();
//        smartVerticesTmp = smartVerticesMutable;

//        stop++;

//        if (stop == smartVertices.size()) {
//            break;
//            std::cerr << "Stopping infinite loop from Polygon::splitSmartVertices." << std::endl;
//        }
//    }

//    return polygons;
}

PolygonList GameController::cutPolygons(const Segment& line) {
//    std::vector<Polygon> polygons;
//    std::vector<std::pair<Point2d, bool>> smartVerticesLeft;
//    std::vector<std::pair<Point2d, bool>> smartVerticesRight;

//    bool onLeft = true;
//    unsigned int verticesCount = _vertices.size();

//    for (unsigned int k = 0; k < verticesCount; k++) {
//        Point2d fstPoint(_vertices.at(k));
//        Point2d sndPoint(_vertices.at((k+1)%verticesCount));
////        std::cerr << "Dealing with " << fstPoint << " " << sndPoint << std::endl;
//        Segment currSegment(fstPoint, sndPoint);
//        Segment::Intersection intersection = currSegment.computeIntersection(line);

//        switch (intersection) {
//        case Segment::Regular: {
////            std::cerr << "Intersection: Regular" << std::endl;
//            Point2d intersectionPoint(Segment::intersectionPoint(currSegment, line));
//            if (onLeft) {
//                smartVerticesLeft.push_back(std::pair<Point2d, bool>(fstPoint, false));
//                smartVerticesLeft.push_back(std::pair<Point2d, bool>(intersectionPoint, true));
////                std::cerr << "Adding " << fstPoint << " to left vertices" << std::endl;
////                std::cerr << "Adding " << intersectionPoint << " to left vertices" << std::endl;
//                smartVerticesRight.push_back(std::pair<Point2d, bool>(intersectionPoint, true));
////                std::cerr << "Adding " << intersectionPoint << " to right vertices" << std::endl;
//                smartVerticesRight.push_back(std::pair<Point2d, bool>(sndPoint, false));
////                std::cerr << "Adding " << sndPoint << " to right vertices" << std::endl;
//            } else {
//                smartVerticesRight.push_back(std::pair<Point2d, bool>(intersectionPoint, true));
////                std::cerr << "Adding " << intersectionPoint << " to right vertices" << std::endl;
//                smartVerticesLeft.push_back(std::pair<Point2d, bool>(intersectionPoint, true));
////                std::cerr << "Adding " << intersectionPoint << " to left vertices" << std::endl;
//            }
//            _newVertices.push_back(intersectionPoint);
//            onLeft = !onLeft;
//            break;
//        }
//        case Segment::None:
////            std::cerr << "Intersection: None" << std::endl;
//            if (onLeft) {
////                std::cerr << "Adding " << fstPoint << " to left vertices" << std::endl;
//                smartVerticesLeft.push_back(std::pair<Point2d, bool>(fstPoint, false));
//            } else {
////                std::cerr << "Adding " << sndPoint << " to right vertices" << std::endl;
//                smartVerticesRight.push_back(std::pair<Point2d, bool>(sndPoint, false));
//            }
//            break;
//        case Segment::FirstVertexRegular:
//            break;
//        case Segment::SecondVertexRegular: {
////            std::cerr << "Intersection: SecondVertexRegular" << std::endl;
//            Point2d nextPoint(_vertices.at((k+2)%verticesCount));
//            bool isCutting = !line.sameSide(fstPoint, nextPoint);

//            if (onLeft && isCutting) {
//                smartVerticesLeft.push_back(std::pair<Point2d, bool>(fstPoint, false));
//                smartVerticesLeft.push_back(std::pair<Point2d, bool>(sndPoint, true));
////                std::cerr << "Adding " << fstPoint << " to left vertices" << std::endl;
////                std::cerr << "Adding " << sndPoint << " to left vertices" << std::endl;
//                smartVerticesRight.push_back(std::pair<Point2d, bool>(sndPoint, true));
//                smartVerticesRight.push_back(std::pair<Point2d, bool>(nextPoint, false));
////                std::cerr << "Adding " << sndPoint << " to right vertices" << std::endl;
////                std::cerr << "Adding " << nextPoint << " to right vertices" << std::endl;
//                _newVertices.push_back(sndPoint);
//            } else if (onLeft && !isCutting) {
//                smartVerticesLeft.push_back(std::pair<Point2d, bool>(fstPoint, false));
//                smartVerticesLeft.push_back(std::pair<Point2d, bool>(sndPoint, true));
//                smartVerticesLeft.push_back(std::pair<Point2d, bool>(sndPoint, true));
////                std::cerr << "Adding " << fstPoint << " to left vertices" << std::endl;
////                std::cerr << "Adding " << sndPoint << " to left vertices" << std::endl;
////                std::cerr << "Adding " << sndPoint << " to left vertices" << std::endl;
//                _newVertices.push_back(sndPoint);
//                _newVertices.push_back(sndPoint);
//            } else if (!onLeft && !isCutting) {
//                smartVerticesRight.push_back(std::pair<Point2d, bool>(sndPoint, true));
//                smartVerticesRight.push_back(std::pair<Point2d, bool>(sndPoint, true));
//                smartVerticesRight.push_back(std::pair<Point2d, bool>(nextPoint, false));
////                std::cerr << "Adding " << sndPoint << " to right vertices" << std::endl;
////                std::cerr << "Adding " << sndPoint << " to right vertices" << std::endl;
////                std::cerr << "Adding " << nextPoint << " to right vertices" << std::endl;
//                _newVertices.push_back(sndPoint);
//                _newVertices.push_back(sndPoint);
//            } else if (!onLeft && isCutting) {
//                smartVerticesRight.push_back(std::pair<Point2d, bool>(sndPoint, true));
//                smartVerticesLeft.push_back(std::pair<Point2d, bool>(sndPoint, true));
////                std::cerr << "Adding " << sndPoint << " to right vertices" << std::endl;
////                std::cerr << "Adding " << sndPoint << " to left vertices" << std::endl;
//                _newVertices.push_back(sndPoint);
//            }

//            if (isCutting) {
//                onLeft = !onLeft;
//            }
//            break;
//        }
//        }
//    }

////    std::cerr << std::endl;

////    QString mess = "";
////    for (const std::pair<Point2d, bool>& p: smartVerticesLeft)
////        mess += " ("+QString::number(p.first.getX())+"; "+QString::number(p.first.getY())+" | ("+QString::number(p.second)+"))";
////    std::cerr << "Left vertices:" << mess.toStdString() << std::endl;

////    mess = "";
////    for (const std::pair<Point2d, bool>& p: smartVerticesRight)
////        mess += " ("+QString::number(p.first.getX())+"; "+QString::number(p.first.getY())+" | ("+QString::number(p.second)+"))";
////    std::cerr << "Right vertices:" << mess.toStdString() << std::endl << std::endl;

//    computeCuttingSegments();

//    std::vector<Polygon> polygonsOnLeft = splitSmartVertices(smartVerticesLeft);
//    for (const Polygon& polygon: polygonsOnLeft)
//        polygons.push_back(polygon);

//    std::vector<Polygon> polygonsOnRight = splitSmartVertices(smartVerticesRight);
//    for (const Polygon& polygon: polygonsOnRight)
//        polygons.push_back(polygon);

//    return polygons;
}

void GameController::computeCuttingSegments(void) {
    _cuttingSegments.clear();

    std::sort(_newVertices.begin(), _newVertices.end());

    for (unsigned int k = 0; k < _newVertices.size()-1; k += 2) {
        _cuttingSegments.push_back(Segment(_newVertices.at(k), _newVertices.at((k+1)%_newVertices.size())));
    }
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
        } else if (!polygon.isGoodSegment(line) && polygon.isCrossing(line)) {
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
