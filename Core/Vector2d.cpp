#include "Vector2d.h"

#include <cmath>    // sqrt, abs, atan2
#include <cfloat>   // FLT_EPSILON

#include "Point2d.h"

Vector2d::Vector2d(const float& x, const float& y) {
  _coords[0] = x;
  _coords[1] = y;
}

Vector2d::Vector2d(const Point2d& A, const Point2d& B) {
  _coords[0] = B.getX()-A.getX();
  _coords[1] = B.getY()-A.getY();
}

Vector2d::Vector2d(const Vector2d& vector) {
  _coords[0] = vector._coords[0];
  _coords[1] = vector._coords[1];
}

bool operator==(const Vector2d& vector1, const Vector2d& vector2) {
  return (vector1[0] - vector2[0]) <  FLT_EPSILON
      && (vector1[0] - vector2[0]) > -FLT_EPSILON
      && (vector1[1] - vector2[1]) <  FLT_EPSILON
      && (vector1[1] - vector2[1]) > -FLT_EPSILON;
}

bool operator!=(const Vector2d& vector1, const Vector2d& vector2) {
  return !(vector1 == vector2);
}

const Vector2d& Vector2d::operator+(void) const {
  return *this;
}

Vector2d Vector2d::operator-(void) const {
  return Vector2d(-_coords[0], -_coords[1]);
}

Vector2d& Vector2d::operator=(const Vector2d& vector) {
  _coords[0] = vector._coords[0];
  _coords[1] = vector._coords[1];
  return *this;
}

Vector2d operator+(const Vector2d& vector1, const Vector2d& vector2) {
  return Vector2d(vector1._coords[0] + vector2._coords[0],
      vector1._coords[1] + vector2._coords[1]);
}

Vector2d operator-(const Vector2d& vector1, const Vector2d& vector2) {
  return Vector2d(vector1._coords[0] - vector2._coords[0],
      vector1._coords[1] - vector2._coords[1]);
}

Vector2d operator*(const Vector2d& vector, const float& k) {
  return Vector2d(vector._coords[0] * k,
      vector._coords[1] * k);
}

Vector2d operator*(const float& k, const Vector2d& vector) {
  return Vector2d(vector._coords[0] * k,
      vector._coords[1] * k);
}

float operator*(const Vector2d& vector1, const Vector2d& vector2) {
  return vector1._coords[0] * vector2._coords[0]
      + vector1._coords[1] * vector2._coords[1];
}

Vector2d operator/(const Vector2d& vector, const float& k) {
  assert(k != 0);
  return Vector2d(vector._coords[0] / k,
      vector._coords[1] / k);
}

Vector2d& Vector2d::operator+=(const Vector2d& vector) {
  return *this = (*this + vector);
}

Vector2d& Vector2d::operator-=(const Vector2d& vector) {
  return *this = (*this - vector);
}

Vector2d& Vector2d::operator*=(const float& k) {
  return *this = (*this * k);
}

Vector2d& Vector2d::operator/=(const float& k) {
  return *this = (*this / k);
}

float Vector2d::norm(void) const {
  return std::sqrt(_coords[0] * _coords[0] + _coords[1] * _coords[1]);
}

float Vector2d::squaredNorm(void) const {
  return (_coords[0] * _coords[0] + _coords[1] * _coords[1]);
}


Vector2d Vector2d::normalized(void) const {
  float vectorNorm = norm();
  if (vectorNorm == 0.f)
    throw std::runtime_error("Vector is nul, cannot normalize it. (Vector2d Vector2d::normalized(void) const)");

  float n = 1.0 / vectorNorm;
  return Vector2d(_coords[0]*n, _coords[1]*n);
}

Vector2d& Vector2d::normalize(void) {
  float vectorNorm = norm();
  if (vectorNorm == 0.f)
    throw std::runtime_error("Vector is nul, cannot normalize it. (Vector2d& Vector2d::normalize(void))");

  float n = 1.0 / vectorNorm;
  _coords[0] *= n;
  _coords[1] *= n;
  return *this;
}

float Vector2d::determinant(const Vector2d& vector1, const Vector2d& vector2) {
  return vector1._coords[0] * vector2._coords[1]
      - vector1._coords[1] * vector2._coords[0];
}

bool Vector2d::areColinear(const Vector2d& vector1, const Vector2d& vector2) {
  return std::abs(determinant(vector1, vector2)) < 100*FLT_EPSILON;
}

bool Vector2d::areOrthogonal(const Vector2d& vector1, const Vector2d& vector2) {
  return std::abs(vector1 * vector2) < FLT_EPSILON;
}

float Vector2d::angle(const Vector2d& vector1, const Vector2d& vector2) {
  float cos = vector1.normalized()*vector2.normalized();
  float sin = determinant(vector1.normalized(), vector2.normalized());

  return std::atan2(sin, cos);
}

std::ostream& operator<<(std::ostream &os, const Vector2d& vector) {
  return os << "(Vector) (" << vector[0] << "; " << vector[1] << ")";
}
