#include "Polygon.h"

#include "Point2d.h"
#include "Vector2d.h"
#include "Segment.h"

#include <cmath>        // round, abs
#include <cfloat>       // FLT_EPSILON
#include <cstring>      // string
#include <algorithm>    // sort
#include <iomanip>      // boolalpha

#include <QString>

Polygon::Polygon(const std::vector<Point2d>& vertices) :
    _vertices(vertices),
    _newVertices(),
    _cuttingSegments() {
}

Polygon::Polygon(int xMin, int xMax, int yMin, int yMax, unsigned int verticesCount) :
    _newVertices(),
    _cuttingSegments() {

    if (verticesCount < 3)
        Polygon();

    for (int k = 0; k < 3; k++) {
        _vertices.push_back(Point2d(xMin, xMax, yMin, yMax));
    }

    for (unsigned int k = 3; k < verticesCount; k++) {
        Point2d newVertex;
        do {
            newVertex = Point2d(xMin, xMax, yMin, yMax);
        } while (!newPointIsGood(newVertex));
        _vertices.push_back(newVertex);
    }
}

Polygon::Polygon(const Polygon& polygon) {
    _vertices = polygon._vertices;
    _newVertices = polygon._newVertices;
    _cuttingSegments = polygon._cuttingSegments;
}

Polygon::Polygon(std::istream& is) :
    _newVertices(),
    _cuttingSegments() {

    operator<<(is);

    if(_vertices.size() < 3) {
        std::cerr << "Not enough vertices inside file.";
        _vertices.clear();
    }
}

void Polygon::insertVertex(const Point2d& vertex, int position) {
    if (position == -1)
        position = _vertices.size();

    _vertices.insert(_vertices.begin()+position, vertex);
}

void Polygon::removeVertex(int position) {
    if (position == -1)
        position = _vertices.size();

    _vertices.erase(_vertices.begin()+position);
}

void Polygon::replaceVertex(int position, const Point2d& newVertex) {
    removeVertex(position);
    insertVertex(newVertex, position);
}

void Polygon::translate(const Vector2d& direction) {
    for (unsigned int k = 0; k < _vertices.size(); k++) {
        _vertices.at(k).move(direction.getX(), direction.getY());
    }
}

bool Polygon::newPointIsGood(const Point2d& vertex) const {
    Segment fstSegment(vertex, _vertices.at(0));
    Segment sndSegment(vertex, _vertices.at(_vertices.size()-1));

    for (unsigned int k = 0; k < _vertices.size()-1; k++) {
        Segment currSegment(_vertices.at(k), _vertices.at(k+1));
        if (k != 0) {
            if (currSegment.computeIntersection(fstSegment) != Segment::None) {
                return false;
            }
        }

        if (k != _vertices.size()-2) {
            if (currSegment.computeIntersection(sndSegment) != Segment::None) {
                return false;
            }
        }
    }

    return true;
}

std::vector<Polygon> Polygon::splitSmartVertices(const std::vector<std::pair<Point2d, bool>>& smartVertices) const {
    std::vector<Polygon> polygons;
    Polygon polygon;

    std::vector<std::pair<Point2d, bool>> smartVerticesTmp(smartVertices);

    bool seekNextNewVertex = false;
    Point2d prevNewVertex;

    unsigned int stop = 0;
    while (smartVerticesTmp.size() > 0) {
        std::vector<std::pair<Point2d, bool>> smartVerticesMutable;
        unsigned int smartVerticesCount = smartVerticesTmp.size();


//        QString mess = "";
//        for (const std::pair<Point2d, bool>& p: smartVerticesTmp)
//            mess += " ("+QString::number(p.first.getX())+"; "+QString::number(p.first.getY())+" "+QString::number(p.second)+") ";
//        std::cerr << "## " << mess.toStdString() << std::endl << std::endl;


        for (unsigned int k = 0; k < smartVerticesCount; k++) {
            std::pair<Point2d, bool> fstSmartVertex = smartVerticesTmp.at(k);

//            std::cerr << "Seek next new vertex: " << std::boolalpha << seekNextNewVertex << std::endl;

            if (fstSmartVertex.second && seekNextNewVertex) {
                Segment segment(prevNewVertex, fstSmartVertex.first);
//                std::cerr << "Segment: " << segment << std::endl;
                if (isACuttingSegment(segment)) {
//                    std::cerr << "Is a cutting segment: true" << std::endl;
                    polygon << fstSmartVertex.first;
//                    std::cerr << "Adding " << fstSmartVertex.first << " to polygon" << std::endl;
                    seekNextNewVertex = false;
                } else {
//                    std::cerr << "Is a cutting segment: false" << std::endl;
                    smartVerticesMutable.push_back(fstSmartVertex);
//                    std::cerr << "Adding " << fstSmartVertex.first << " to smart vertices" << std::endl;
                }
            } else if (fstSmartVertex.second && !seekNextNewVertex) {
                if (k != 0) {
                    seekNextNewVertex = true;
                    prevNewVertex = fstSmartVertex.first;
                }
                polygon << fstSmartVertex.first;
//                std::cerr << "Adding " << fstSmartVertex.first << " to polygon" << std::endl;
            } else if (!fstSmartVertex.second && seekNextNewVertex) {
                smartVerticesMutable.push_back(fstSmartVertex);
//                std::cerr << "Adding " << fstSmartVertex.first << " to smart vertices" << std::endl;
            } else if (!fstSmartVertex.second && !seekNextNewVertex) {
                polygon << fstSmartVertex.first;
//                std::cerr << "Adding " << fstSmartVertex.first << " to polygon" << std::endl;
            }
        }

        seekNextNewVertex = false;

        polygons.push_back(polygon);
        polygon.clear();

        smartVerticesTmp.clear();
        smartVerticesTmp = smartVerticesMutable;

        stop++;

        if (stop == smartVertices.size()) {
            break;
            std::cerr << "Stopping infinite loop from Polygon::splitSmartVertices." << std::endl;
        }
    }

    return polygons;
}

bool Polygon::isPointInside(const Point2d& point) const {
    Point2d pointFarAway(100000, 100000);
    Segment halfLine(point, pointFarAway);

    unsigned int countVertices = _vertices.size();
    int countIntersections = 0;

    for (unsigned int k = 0; k < countVertices; k++) {
        Point2d fstVertex(_vertices.at(k));
        Point2d sndVertex(_vertices.at((k+1)%countVertices));

        Segment currSegment(fstVertex, sndVertex);
        Segment::Intersection intersection = currSegment.computeIntersection(halfLine);

        switch (intersection) {
        case Segment::Regular:
            countIntersections++;
            break;
        case Segment::FirstVertexRegular: {
            Point2d prevVertex(_vertices.at((k-1)%countVertices));
            if (!halfLine.sameSide(prevVertex, sndVertex))
                countIntersections++;
            break;
        }
        case Segment::SecondVertexRegular: {
            Point2d nextVertex(_vertices.at((k+2)%countVertices));
            if (!halfLine.sameSide(fstVertex, nextVertex))
                countIntersections++;
            break;
        }
        case Segment::None:
            break;
        default:
            break;
        }
    }

    return countIntersections%2;
}

bool Polygon::isPointInside2(const Point2d& point) const {
    unsigned int countVertices = _vertices.size();
    float theta = 0;

    for (unsigned int k = 0; k < countVertices; k++) {
        Point2d fstVertex(_vertices.at(k));
        Point2d sndVertex(_vertices.at((k+1)%countVertices));
        Vector2d v1(fstVertex - point);
        Vector2d v2(sndVertex - point);
        theta += Vector2d::angle(v1, v2);
    }
    return std::round(std::abs(theta)) == 6;
}

bool Polygon::isCrossing(const Segment& line) const {
    unsigned int verticesCount = _vertices.size();

    for (unsigned int k = 0; k < verticesCount; k++) {
        Segment segment(_vertices.at(k), _vertices.at((k+1)%verticesCount));
        if (segment.computeIntersection(line) != Segment::None)
            return true;
    }

    return false;
}

bool Polygon::isGoodSegment(const Segment& line) const {
//    std::cerr << line.getA() << std::endl;
    return (!isPointInside(line.getA()) /*&& isCrossing(line)*/ && !isPointInside(line.getB()));
}

float Polygon::orientedArea(void) const {
    unsigned int countVertices = _vertices.size();

    float area = 0.0;

    for (unsigned int k = 0; k < countVertices; k++) {
        Point2d fstVertex(_vertices.at(k));
        Point2d sndVertex(_vertices.at((k+1)%countVertices));
        Segment segment(fstVertex, sndVertex);

        area += segment.orientedArea();
    }

    return std::abs(area);
}

Point2d Polygon::barycenter(void) const {
    if (_vertices.size() < 3) {
        std::cerr << "Error within Polygon::barycenter: polygon has less than 3 vertices." << std::endl;
        return Point2d();
    }


    Point2d bary;

    for (const Point2d& currPoint: _vertices) {
        bary += currPoint;
    }

    bary /= _vertices.size();

    return bary;
}

float Polygon::computeAngleFromPoint(float x, float y) {
    unsigned int countVertices = _vertices.size();
    float res = 0.0;

    for (unsigned int k = 0; k < countVertices; k++) {
        float x1 = _vertices.at(k).getX() - x;
        float y1 = _vertices.at(k).getY() - y;
        float x2 = _vertices.at((k+1)%countVertices).getX() - x;
        float y2 = _vertices.at((k+1)%countVertices).getY() - y;

        res += std::atan2(x1*y2 - x2*y1, x1*x2 + y1*y2);
    }

    return res;
}

float** Polygon::surface(float size, float delta) {
    int nbPoints = size/delta;
    float** matrix = new float*[nbPoints];
    for (int i=0; i< nbPoints; ++i)
        matrix[i] = new float[nbPoints];

//    std::cerr << "Compute polygon barycenter from Polygon::surface" << std::endl;
    Point2d polygonCenter(barycenter());
    float centerX = polygonCenter.getX();
    float centerY = polygonCenter.getY();
    float xMin = centerX - size/2;
    float yMin = centerY - size/2;

    int ii = 0,jj;
    for (float i = xMin; i < xMin+size-delta/2; i+=delta, ++ii) {
        jj = 0;
        for (float j = yMin; j < yMin+size-delta/2; j+=delta, ++jj) {
            matrix[ii][jj] = computeAngleFromPoint(i, j);
        }
    }

    return matrix;
}



std::vector<Polygon> Polygon::sliceIt(const Segment& line) {
    std::vector<Polygon> polygons;
    std::vector<std::pair<Point2d, bool>> smartVerticesLeft;
    std::vector<std::pair<Point2d, bool>> smartVerticesRight;

    bool onLeft = true;
    unsigned int verticesCount = _vertices.size();

    for (unsigned int k = 0; k < verticesCount; k++) {
        Point2d fstPoint(_vertices.at(k));
        Point2d sndPoint(_vertices.at((k+1)%verticesCount));
//        std::cerr << "Dealing with " << fstPoint << " " << sndPoint << std::endl;
        Segment currSegment(fstPoint, sndPoint);
        Segment::Intersection intersection = currSegment.computeIntersection(line);

        switch (intersection) {
        case Segment::Regular: {
//            std::cerr << "Intersection: Regular" << std::endl;
            Point2d intersectionPoint(Segment::intersectionPoint(currSegment, line));
            if (onLeft) {
                smartVerticesLeft.push_back(std::pair<Point2d, bool>(fstPoint, false));
                smartVerticesLeft.push_back(std::pair<Point2d, bool>(intersectionPoint, true));
//                std::cerr << "Adding " << fstPoint << " to left vertices" << std::endl;
//                std::cerr << "Adding " << intersectionPoint << " to left vertices" << std::endl;
                smartVerticesRight.push_back(std::pair<Point2d, bool>(intersectionPoint, true));
//                std::cerr << "Adding " << intersectionPoint << " to right vertices" << std::endl;
                smartVerticesRight.push_back(std::pair<Point2d, bool>(sndPoint, false));
//                std::cerr << "Adding " << sndPoint << " to right vertices" << std::endl;
            } else {
                smartVerticesRight.push_back(std::pair<Point2d, bool>(intersectionPoint, true));
//                std::cerr << "Adding " << intersectionPoint << " to right vertices" << std::endl;
                smartVerticesLeft.push_back(std::pair<Point2d, bool>(intersectionPoint, true));
//                std::cerr << "Adding " << intersectionPoint << " to left vertices" << std::endl;
            }
            _newVertices.push_back(intersectionPoint);
            onLeft = !onLeft;
            break;
        }
        case Segment::None:
//            std::cerr << "Intersection: None" << std::endl;
            if (onLeft) {
//                std::cerr << "Adding " << fstPoint << " to left vertices" << std::endl;
                smartVerticesLeft.push_back(std::pair<Point2d, bool>(fstPoint, false));
            } else {
//                std::cerr << "Adding " << sndPoint << " to right vertices" << std::endl;
                smartVerticesRight.push_back(std::pair<Point2d, bool>(sndPoint, false));
            }
            break;
        case Segment::FirstVertexRegular:
            break;
        case Segment::SecondVertexRegular: {
//            std::cerr << "Intersection: SecondVertexRegular" << std::endl;
            Point2d nextPoint(_vertices.at((k+2)%verticesCount));
            bool isCutting = !line.sameSide(fstPoint, nextPoint);

            if (onLeft && isCutting) {
                smartVerticesLeft.push_back(std::pair<Point2d, bool>(fstPoint, false));
                smartVerticesLeft.push_back(std::pair<Point2d, bool>(sndPoint, true));
//                std::cerr << "Adding " << fstPoint << " to left vertices" << std::endl;
//                std::cerr << "Adding " << sndPoint << " to left vertices" << std::endl;
                smartVerticesRight.push_back(std::pair<Point2d, bool>(sndPoint, true));
                smartVerticesRight.push_back(std::pair<Point2d, bool>(nextPoint, false));
//                std::cerr << "Adding " << sndPoint << " to right vertices" << std::endl;
//                std::cerr << "Adding " << nextPoint << " to right vertices" << std::endl;
                _newVertices.push_back(sndPoint);
            } else if (onLeft && !isCutting) {
                smartVerticesLeft.push_back(std::pair<Point2d, bool>(fstPoint, false));
                smartVerticesLeft.push_back(std::pair<Point2d, bool>(sndPoint, true));
                smartVerticesLeft.push_back(std::pair<Point2d, bool>(sndPoint, true));
//                std::cerr << "Adding " << fstPoint << " to left vertices" << std::endl;
//                std::cerr << "Adding " << sndPoint << " to left vertices" << std::endl;
//                std::cerr << "Adding " << sndPoint << " to left vertices" << std::endl;
                _newVertices.push_back(sndPoint);
                _newVertices.push_back(sndPoint);
            } else if (!onLeft && !isCutting) {
                smartVerticesRight.push_back(std::pair<Point2d, bool>(sndPoint, true));
                smartVerticesRight.push_back(std::pair<Point2d, bool>(sndPoint, true));
                smartVerticesRight.push_back(std::pair<Point2d, bool>(nextPoint, false));
//                std::cerr << "Adding " << sndPoint << " to right vertices" << std::endl;
//                std::cerr << "Adding " << sndPoint << " to right vertices" << std::endl;
//                std::cerr << "Adding " << nextPoint << " to right vertices" << std::endl;
                _newVertices.push_back(sndPoint);
                _newVertices.push_back(sndPoint);
            } else if (!onLeft && isCutting) {
                smartVerticesRight.push_back(std::pair<Point2d, bool>(sndPoint, true));
                smartVerticesLeft.push_back(std::pair<Point2d, bool>(sndPoint, true));
//                std::cerr << "Adding " << sndPoint << " to right vertices" << std::endl;
//                std::cerr << "Adding " << sndPoint << " to left vertices" << std::endl;
                _newVertices.push_back(sndPoint);
            }

            if (isCutting) {
                onLeft = !onLeft;
            }
            break;
        }
        }
    }

//    std::cerr << std::endl;

//    QString mess = "";
//    for (const std::pair<Point2d, bool>& p: smartVerticesLeft)
//        mess += " ("+QString::number(p.first.getX())+"; "+QString::number(p.first.getY())+" | ("+QString::number(p.second)+"))";
//    std::cerr << "Left vertices:" << mess.toStdString() << std::endl;

//    mess = "";
//    for (const std::pair<Point2d, bool>& p: smartVerticesRight)
//        mess += " ("+QString::number(p.first.getX())+"; "+QString::number(p.first.getY())+" | ("+QString::number(p.second)+"))";
//    std::cerr << "Right vertices:" << mess.toStdString() << std::endl << std::endl;

    computeCuttingSegments();

    std::vector<Polygon> polygonsOnLeft = splitSmartVertices(smartVerticesLeft);
    for (const Polygon& polygon: polygonsOnLeft)
        polygons.push_back(polygon);

    std::vector<Polygon> polygonsOnRight = splitSmartVertices(smartVerticesRight);
    for (const Polygon& polygon: polygonsOnRight)
        polygons.push_back(polygon);

    return polygons;
}

void Polygon::computeCuttingSegments(void) {
    _cuttingSegments.clear();

//    std::cerr << *this << std::endl;

    std::sort(_newVertices.begin(), _newVertices.end());

//    std::cerr << _newVertices.size() << std::endl;
//    for (const Point2d p: _newVertices)
//        std::cerr << p << std::endl;

    for (unsigned int k = 0; k < _newVertices.size()-1; k += 2) {
        _cuttingSegments.push_back(Segment(_newVertices.at(k), _newVertices.at((k+1)%_newVertices.size())));
    }
}

bool Polygon::isACuttingSegment(const Segment& segment) const {
    Segment usedSegment(segment);
    if (segment.getA() > segment.getB()) {
        usedSegment.setA(segment.getB());
        usedSegment.setB(segment.getA());
    }

//    std::cerr << segment << " " << usedSegment << std::endl;

    for (const Segment& currSegment: _cuttingSegments) {
//        std::cerr << currSegment << " ";
        if (usedSegment == currSegment) {
            return true;
        }
    }
//    std::cerr << std::endl;

    return false;
}

bool operator==(const Polygon& polygon1, const Polygon& polygon2) {
    if (polygon1._vertices.size() != polygon2._vertices.size()) {
        return false;
    } else {
        for (unsigned int i = 0; i < polygon1._vertices.size(); i++) {
            if (polygon1._vertices.at(i) != polygon2._vertices.at(i)) {
                return false;
            }
        }
    }
    return true;
}

Polygon& operator<<(Polygon& polygon, const Point2d& vertex) {
    bool isNotAlreadyThere = true;
    for (const Point2d& currVertex: polygon._vertices) {
        if (currVertex == vertex) {
            isNotAlreadyThere = false;
            break;
        }
    }

    if (isNotAlreadyThere)
        polygon._vertices.push_back(vertex);

    return polygon;
}

Polygon& operator<<(Polygon& polygon, const std::vector<Point2d>& vertices) {
    for (unsigned int k = 0; k < vertices.size(); k++) {
        polygon << vertices.at(k);
    }
    return polygon;
}

std::istream& Polygon::operator<<(std::istream& is) {
    unsigned size, s, t;
    is >> size;
    assert(is.get()==';');
    _vertices.resize(size);
    for (unsigned int k = 0; k < size; k++) {
        is >> s;
        assert(is.get() == ',');
        is >> t;
        _vertices.at(k) = Point2d(s, t);
        assert(is.get()==';');
    }
    return is;
}

std::istream& operator>>(std::istream& is, Polygon& polygon) {
    return polygon << is;
}

std::ostream& operator<<(std::ostream& os, const Polygon& polygon) {
    std::vector<Point2d> vertices = polygon._vertices;

    os << vertices.size() << ";";
    for (unsigned k = 0; k < vertices.size(); k++) {
        os << vertices.at(k) << ";";
    }
    return os;
}
