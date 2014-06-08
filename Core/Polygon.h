#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include <iostream>

#include "Segment.h"

class Point2d;
class Vector2d;

class Polygon {
public:
  Polygon(const std::vector<Point2d>& vertices = std::vector<Point2d>());
  Polygon(int xMin, int xMax, int yMin, int yMax, unsigned int verticesCount);
  Polygon(std::istream& is);
  Polygon(const Polygon& polygon);
  virtual ~Polygon(void) {}

  inline bool hasEnoughVertices(void) const { return _vertices.size() > 2; }
  inline std::vector<Point2d> getVertices(void) const { return _vertices; }
  inline void setVertices(const std::vector<Point2d>& vertices) { _vertices = vertices; }

  void insertVertex(const Point2d& vertex, int position = -1);
  void removeVertex(int position = -1);
  void replaceVertex(int position, const Point2d& newVertex);

  void translate(const Vector2d& direction);

  bool newPointIsGood(const Point2d& vertex) const;

  bool isPointInside(const Point2d& point) const;
  bool isPointInside2(const Point2d& point) const;

  bool isCrossing(const Segment& line) const;
  bool isGoodSegment(const Segment& line) const;

  inline void clear(void) { _vertices.clear(); }

  float orientedArea(void) const;
  Point2d barycenter(void) const;

  friend bool operator==(const Polygon& polygon1, const Polygon& polygon2);

  friend Polygon& operator<<(Polygon& polygon, const Point2d& vertex);
  friend Polygon& operator<<(Polygon& polygon, const std::vector<Point2d>& vertices);

  std::istream& operator<<(std::istream &is);
  friend std::istream& operator>>(std::istream &is, Polygon& polygon);
  friend std::ostream& operator<<(std::ostream& os, const Polygon& polygon);

  float computeAngleFromPoint(float x, float y);
  float** surface(float size, float delta);

private:
  std::vector<Point2d> _vertices;
};

#endif // POLYGON_H
