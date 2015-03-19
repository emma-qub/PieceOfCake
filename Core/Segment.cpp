#include "Segment.h"

#include "Vector2d.h"

#include <QDebug>

#include <cmath>    // abs
#include <cfloat>   // FLT_EPSILON

Segment::Segment(const Point2d& a, const Point2d& b) {
  _boundaries[0] = a;
  _boundaries[1] = b;
}

Segment::Segment(int xa, int ya, int xb, int yb) {
  _boundaries[0] = Point2d(xa, ya);
  _boundaries[1] = Point2d(xb, yb);
}

Segment::Segment(const Segment& segment) {
  _boundaries[0] = segment._boundaries[0];
  _boundaries[1] = segment._boundaries[1];
}

Point2d Segment::getCenter() const {
  return Point2d((getA().getX() + getB().getX())/2.0, (getA().getY() + getB().getY())/2.0);
}

Vector2d Segment::getNormal(void) const {
  return Vector2d(-(getB().getY() - getA().getY()), (getB().getX() - getA().getX())).normalized();
}

Vector2d Segment::getDirection(void) const {
  return Vector2d::fromSegment(*this).normalized();
}

Point2d Segment::getOtherBoundary(const Point2d& boundary) {
  if (boundary == _boundaries[0])
    return _boundaries[1];
  else if (boundary == _boundaries[1])
    return _boundaries[0];

  return Point2d();
}

Segment::Side Segment::location(const Point2d& P) const {
  Point2d A = _boundaries[0];
  Point2d B = _boundaries[1];

  if (A == P)
    return IsBoundA;
  if (B == P)
    return IsBoundB;

  Vector2d AB = B - A;
  Vector2d AP = P - A;

  float det = Vector2d::determinant(AB, AP);
  if (Vector2d::areColinear(AB, AP)) {
    if ((Point2d::distance(A, P) < Point2d::distance(A, B)) && (Point2d::distance(B, P) < Point2d::distance(A, B)))
      return OnSegmentInside;
    else
      return OnSegmentOutside;
  } else if (det > FLT_EPSILON)
    return OnLeft;

  return OnRight;
}

bool Segment::sameSide(const Point2d& P, const Point2d& Q) const {
  Side locationP = location(P);
  Side locationQ = location(Q);
  return (locationP == locationQ && locationP != OnSegmentInside && locationP != OnSegmentOutside && locationP != IsBoundA && locationP != IsBoundB);
}

Segment::Intersection Segment::computeIntersection(const Point2d& P, const Point2d& Q) const {
  Point2d A = _boundaries[0];
  Point2d B = _boundaries[1];

  Vector2d vAB(B - A);
  Vector2d vPQ(Q - P);

  // Deal with colinear
  if (Vector2d::areColinear(vAB, vPQ) || A == P || A == Q || B == P || B == Q) {
    Vector2d vAP(P - A);
    Vector2d vAQ(Q - A);
    if (Vector2d::areColinear(vAP, vAQ) && Point2d::distance(P, Q) > Point2d::distance(A, B)) {
      return Edge;
    } else {
      return None;
    }
  }

  Segment PQ(P, Q);
  Side positionP = location(P);
  Side positionQ = location(Q);
  Side positionA = PQ.location(A);
  Side positionB = PQ.location(B);

  if (!sameSide(P, Q) && !PQ.sameSide(A, B) && positionP != OnSegmentInside && positionQ != OnSegmentInside && positionA != OnSegmentInside && positionB != OnSegmentInside)
    return Regular;
  else if (positionA == OnSegmentInside)
    return FirstVertexRegular;
  else if (positionB == OnSegmentInside)
    return SecondVertexRegular;

  return None;
}

Segment::Intersection Segment::computeIntersection(const Segment& PQ) const {
  return computeIntersection(PQ.getA(), PQ.getB());
}

float Segment::orientedArea(void) const {
  Point2d A(_boundaries[0]);
  Point2d B(_boundaries[1]);

  float h = B.getX() - A.getX();
  float base1 = B.getY();
  float base2 = A.getY();

  return h*(base1+base2)/2;
}

float Segment::length(void) const {
  return std::sqrt((_boundaries[1].getX()-_boundaries[0].getX()) * (_boundaries[1].getX()-_boundaries[0].getX())
                 + (_boundaries[1].getY()-_boundaries[0].getY()) * (_boundaries[1].getY()-_boundaries[0].getY()));
}

Point2d Segment::intersectionPoint(const Segment& AB, const Segment& PQ) {
  if (AB.computeIntersection(PQ) != Segment::Regular) {
    std::cerr << "Error within intersectionPoint: no intersection." << std::endl;
    return Point2d(-1, -1);
  }

  Point2d A(AB.getA());
  Point2d B(AB.getB());
  Vector2d vAB(B - A);

  Point2d P(PQ.getA());
  Point2d Q(PQ.getB());
  Vector2d vPQ(Q - P);

  float Ax = A.getX();
  float Ay = A.getY();
  float ABx = vAB.getX();
  float ABy = vAB.getY();

  float Px = P.getX();
  float Py = P.getY();
  float PQx = vPQ.getX();
  float PQy = vPQ.getY();

  float t = -(Ax*PQy - Px*PQy - PQx*Ay + PQx*Py) / (ABx*PQy - ABy*PQx);

  Vector2d tvAB(t*vAB);
  Point2d tAB(std::round(tvAB.getX()), std::round(tvAB.getY()));
  return Point2d(A + tAB);
}

bool operator==(const Segment& segment1, const Segment& segment2) {
  return (segment1.getA() == segment2.getA() && segment1.getB() == segment2.getB());
}

bool operator!=(const Segment& segment1, const Segment& segment2) {
  return !(segment1 == segment2);
}

bool operator<(const Segment& segment1, const Segment& segment2) {
  Point2d A = segment1.getA();
  Point2d B = segment1.getB();
  Point2d C = segment2.getA();
  Point2d D = segment2.getB();

  if (A != C) {
    return A < C;
  } else {
    return B < D;
  }
}

std::ostream& operator<<(std::ostream& os, const Segment& segment) {
  return os << "(Segment) [" << segment.getA() << " " << segment.getB() << "]";
}

Segment& Segment::operator=(const Segment& segment) {
  _boundaries[0] = segment._boundaries[0];
  _boundaries[1] = segment._boundaries[1];
  return *this;
}

QDebug operator<<(QDebug d, const Segment& segment) {
  return d << "[" << segment.getA() << " " << segment.getB() << "]";
}
