#include "Polygon.h"

#include "Point2d.h"
#include "Vector2d.h"
#include "Segment.h"

#include <cmath>        // round, abs
#include <cfloat>       // FLT_EPSILON
#include <cstring>      // string
#include <algorithm>    // sort
#include <iomanip>      // boolalpha

namespace poc {

Polygon::Polygon(const std::vector<Point2d>& vertices):
  _vertices(vertices) {
}

Polygon::Polygon(int xMin, int xMax, int yMin, int yMax, unsigned int verticesCount) {

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
}

Polygon::Polygon(std::istream& is) {

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
  translate(direction.getX(), direction.getY());
}

void Polygon::translate(float x, float y) {
  if (x == 0. && y == 0.)
    return;

  auto it = _vertices.begin();
  for (; it != _vertices.end(); ++it)
    it->move(x, y);
}

void Polygon::homothetie(const Point2d& origin, float scale) {
  for (Point2d& vertex: _vertices) {
    vertex.homothetie(origin, scale);
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

  int regularCount = 0;
  int fstVertexCount = 0;
  int sndVertexCount = 0;
  int edgeCount = 0;
  int noneCount = 0;
  int otherCount = 0;

  for (unsigned int k = 0; k < verticesCount; k++) {
    Segment segment(_vertices.at(k), _vertices.at((k+1)%verticesCount));
    Segment::Intersection intersection = segment.computeIntersection(line);
    switch (intersection) {
    case Segment::Regular:
      regularCount++;
      break;
    case Segment::FirstVertexRegular:
      fstVertexCount++;
      break;
    case Segment::SecondVertexRegular:
      sndVertexCount++;
      break;
    case Segment::Edge:
      edgeCount++;
      break;
    case Segment::None:
      noneCount++;
      break;
    default:
      break;
    }
  }

  otherCount = fstVertexCount + sndVertexCount + regularCount;

  // Prevent from cutting through a polygon's edge since it's really not a good idea.
  return (edgeCount == 0 && otherCount > 0);
}

bool Polygon::isGoodSegment(const Segment& line) const {
  return (!isPointInside2(line.getA()) && isCrossing(line) && !isPointInside2(line.getB()));
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
  float baryX = 0.f;
  float baryY = 0.f;

  int vertexCount = _vertices.size();
  for (int k = 0; k < vertexCount; ++k) {
    const Point2d& v0 = _vertices.at(k);
    const Point2d& v1 = _vertices.at((k+1)%vertexCount);

    float xi0 = v0.getX();
    float xi1 = v1.getX();
    float yi0 = v0.getY();
    float yi1 = v1.getY();

    baryX += (xi0+xi1)*(xi0*yi1 - xi1*yi0);
    baryY += (yi0+yi1)*(xi0*yi1 - xi1*yi0);
  }

  // take abs other wise barycenter depends on vertex browse direction
  bary.setX(std::abs(baryX));
  bary.setY(std::abs(baryY));

  bary /= (6*orientedArea());

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

QDebug operator<<(QDebug dbg, const Polygon &polygon) {
  std::vector<Point2d> vertices = polygon._vertices;

  dbg.nospace() << vertices.size() << ";";
  for (unsigned k = 0; k < vertices.size(); k++) {
    dbg.nospace() << vertices.at(k) << ";";
  }

  return dbg.maybeSpace();
}

}
