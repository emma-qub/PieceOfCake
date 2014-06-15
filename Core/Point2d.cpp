#include "Point2d.h"
#include "Vector2d.h"

#include <cmath>    // sqrt, pow, abs
#include <cfloat>   // FLT_EPSILON

Point2d::Point2d(float x, float y) {
  _coords[0] = x;
  _coords[1] = y;
}

Point2d::Point2d(int xMin, int xMax, int yMin, int yMax) {
  _coords[0] = rand()%(xMax - xMin) + xMin;
  _coords[1] = rand()%(yMax - yMin) + yMin;
}

Point2d::Point2d(const Point2d& point2d) {
  _coords[0] = point2d._coords[0];
  _coords[1] = point2d._coords[1];
}

void Point2d::move(const float &x, const float &y) {
  _coords[0] += x;
  _coords[1] += y;
}

void Point2d::symetry(void) {
  float tmp = _coords[1];
  _coords[1] = _coords[0];
  _coords[0] = tmp;
}

void Point2d::homothetie(const Point2d& origin, float scale) {
  _coords[0] = scale*(_coords[0] - origin._coords[0]) + origin._coords[0];
  _coords[1] = scale*(_coords[1] - origin._coords[1]) + origin._coords[1];
}

Point2d Point2d::applyVector(const Vector2d& vector) const {
  return Point2d(_coords[0] + vector.getX(), _coords[1] + vector.getY());
}

Point2d Point2d::operator/(const float& k) const {
  return Point2d(_coords[0]/k, _coords[1]/k);
}

Point2d& Point2d::operator/=(const float& k) {
  _coords[0] /= k;
  _coords[1] /= k;
  return *this;
}

Point2d operator+(const Point2d& point1, const Point2d& point2) {
  return Point2d(point1._coords[0] + point2._coords[0],
      point1._coords[1] + point2._coords[1]);
}

Point2d& Point2d::operator+=(const Point2d& point) {
  _coords[0] += point._coords[0];
  _coords[1] += point._coords[1];
  return *this;
}

Vector2d operator-(const Point2d& point1, const Point2d& point2) {
  return Vector2d(point2._coords[0] - point1._coords[0],
      point2._coords[1] - point1._coords[1]);
}

Point2d& Point2d::operator=(const Point2d& point) {
  _coords[0] = point._coords[0];
  _coords[1] = point._coords[1];
  return *this;
}

bool operator==(const Point2d& point1, const Point2d& point2) {
  return std::abs(point1._coords[0] - point2._coords[0]) < FLT_EPSILON
      && std::abs(point1._coords[1] - point2._coords[1]) < FLT_EPSILON;
}

bool operator!=(const Point2d& point1, const Point2d& point2) {
  return !(point1 == point2);
}

bool operator<(const Point2d& point1, const Point2d& point2) {
  return (point1._coords[0] < point2._coords[0])
      || ((point1._coords[0] == point2._coords[0]) && (point1._coords[1] < point2._coords[1]));
}

bool operator<=(const Point2d& point1, const Point2d& point2) {
  return (point1 == point2) || (point1 < point2);
}

bool operator>(const Point2d& point1, const Point2d& point2) {
  return !(point1 <= point2);
}

bool operator>=(const Point2d& point1, const Point2d& point2) {
  return !(point1 < point2);
}

float Point2d::distance(const Point2d& point1, const Point2d& point2) {
  return std::sqrt(std::pow(point2._coords[0] - point1._coords[0], 2)
      + std::pow(point2._coords[1] - point1._coords[1], 2));
}

std::ostream& operator<<(std::ostream& os, const Point2d& point) {
  return os << "(Point) (" << point._coords[0] << "; " << point._coords[1] << ")";
}

QDebug operator<<(QDebug d, const Point2d& point) {
  return d << "(" << point._coords[0] << "; " << point._coords[1] << ")";
}
