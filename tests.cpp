#include "tests.h"

#include <cmath>    // abs
#include <cfloat>   // EPS
#include <iomanip>  // setprecision

#define EPS 5*FLT_EPSILON

void Test::testPoint2d(void) {
  // Default constructor
  Point2d p, q;

  assert(p.getX() == 0);
  assert(p.getY() == 0);


  // Setters
  p.setX(3);
  p.setY(4.2);

  assert(std::abs(p.getX() - 3.0) < EPS);
  assert(std::abs(p.getY() - 4.2) < EPS);


  // Operator=
  q = p;

  assert(std::abs(q.getX() - 3.0) < EPS);
  assert(std::abs(q.getY() - 4.2) < EPS);


  // Copy constructor
  Point2d r(p);

  assert(std::abs(r.getX() - 3.0) < EPS);
  assert(std::abs(r.getY() - 4.2) < EPS);


  // Move
  p.move(3.1);
  p.move(0, -1.1);

  assert(std::abs(p.getX() - 6.1) < EPS);
  assert(std::abs(p.getY() - 3.1) < EPS);


  // Homothetie
  p.homothetie(r, 2.0);

  assert(std::abs(p.getX() - (3.0+2*(6.1-3.0))) < EPS);
  assert(std::abs(p.getY() - (4.2+2*(3.1-4.2))) < EPS);


  // Operator/
  Point2d s(r/7);

  assert(std::abs(s.getX() - 3.0/7.0) < EPS);
  assert(std::abs(s.getY() - 4.2/7.0) < EPS);


  // Operator/=
  r/=7;

  assert(std::abs(r.getX() - 3.0/7.0) < EPS);
  assert(std::abs(r.getY() - 4.2/7.0) < EPS);

  // Operator==
  assert(r == s);


  // Operator!=
  assert(p != r);


  // Operator<
  assert(r < p);


  // Operator<=
  assert(r <= p);
  assert(r <= s);


  // Operator>
  assert(p > r);


  // Operator>=
  assert(p >= r);
  assert(s >= r);


  // Operator+
  Point2d t(p+q);

  assert(std::abs(t.getX() - (p.getX()+q.getX())) < EPS);
  assert(std::abs(t.getY() - (p.getY()+q.getY())) < EPS);


  // Operator+=
  Point2d u(s);
  u += t;

  assert(std::abs(u.getX() - (s.getX()+t.getX())) < EPS);
  assert(std::abs(u.getY() - (s.getY()+t.getY())) < EPS);


  // Operator<<
  std::cerr << p << std::endl;

  // Operator<
  Point2d p1(10, 20), p2(30, 60), p3(40, 80), p4(5, 10);
  std::cerr << (p1 < p2 && p2 < p3) << std::endl;
  std::cerr << (p4 < p3 && p3 < p2) << std::endl;

  unsigned int last;

  std::vector<Point2d> v;
  v.push_back(p1);
  v.push_back(p2);
  v.push_back(p3);
  v.push_back(p4);

  permute(v);

  last = v.size()-1;
  std::cerr << (v.at(0) < v.at(1) && v.at(1) < v.at(2)) << std::endl;
  std::cerr << (v.at(last-2) < v.at(last-1) && v.at(last-1) < v.at(last)) << std::endl;

  std::vector<Point2d> w;
  w.push_back(p4);
  w.push_back(p3);
  w.push_back(p2);
  w.push_back(p1);

  permute(w);

  last = w.size()-1;
  std::cerr << (w.at(0) < w.at(1) && w.at(1) < w.at(2)) << std::endl;
  std::cerr << (w.at(last-2) < w.at(last-1) && w.at(last-1) < w.at(last)) << std::endl;

  std::vector<Point2d> x;
  x.push_back(p4);
  x.push_back(p2);
  x.push_back(p3);
  x.push_back(p1);

  permute(x);

  last = x.size()-1;
  std::cerr << (x.at(0) < x.at(1) && x.at(1) < x.at(2)) << std::endl;
  std::cerr << (x.at(last-2) < x.at(last-1) && x.at(last-1) < x.at(last)) << std::endl;
}

void Test::permute(std::vector<Point2d>& v) {
  unsigned int last = v.size()-1;
  unsigned int k = 0;
  while (!((v.at(0) < v.at(1) && v.at(1) < v.at(2)) && (v.at(last-2) < v.at(last-1) && v.at(last-1) < v.at(last)))
         && !((v.at(2) < v.at(1) && v.at(1) < v.at(0)) && (v.at(last) < v.at(last-1) && v.at(last-1) < v.at(last-2)))) {
    Point2d p(v.at(0));
    v.erase(v.begin());
    v.push_back(p);

    for (const Point2d& currPoint: v) {
      std::cerr << currPoint << std::endl;
    }
    std::cerr << std::endl;

    k++;

    if (k == last+1) {
      std::cerr << "Error: cannot sort vector." << std::endl;
      break;
    }
  }
}

void Test::testVector2d(void) {
  // Default constructor
  Vector2d u, v;

  assert(std::abs(u.getX() - 0) < EPS);
  assert(std::abs(u.getY() - 0) < EPS);


  // Setters
  u.setX(3.0);
  u.setY(4.2);

  assert(std::abs(u.getX() - 3.0) < EPS);
  //    std::cerr << std::setprecision(10) << u.getY() << " " << 4.2 << std::endl;
  assert(std::abs(u.getY() - 4.2) < EPS);


  // Operator=
  v = u;

  assert(std::abs(v.getX() - 3.0) < EPS);
  assert(std::abs(v.getY() - 4.2) < EPS);


  // Copy constructor
  Vector2d w(u);

  assert(std::abs(w.getX() - 3.0) < EPS);
  assert(std::abs(w.getY() - 4.2) < EPS);


  // Operator+
  Vector2d t(u+v);


  // Operator==
  assert(u == v);


  // Operator!=
  assert(u != t);


  // Operator-
  t = -u;

  assert(std::abs(u.getX() + t.getX()) < EPS);
  assert(std::abs(u.getY() + t.getY()) < EPS);


  // Operator*
  t = u*4.5;

  assert(std::abs(t.getX() - u.getX()*4.5) < EPS);
  assert(std::abs(t.getY() - u.getY()*4.5) < EPS);


  // Operator*
  t = 4.5*u;

  assert(std::abs(t.getX() - u.getX()*4.5) < EPS);
  assert(std::abs(t.getY() - u.getY()*4.5) < EPS);


  // Operator *
  float scalar = u*v;

  assert(std::abs(scalar - (u.getX()*v.getX() + u.getY()*v.getY())) < EPS);


  // Operator/
  t = u/3.7;

  assert(std::abs(t.getX() - u.getX()/3.7) < EPS);
  assert(std::abs(t.getY() - u.getY()/3.7) < EPS);


  // Norm
  t = Vector2d(3.0, 4.0);

  assert(std::abs(t.norm() - 5.0) < EPS);


  // SquaredNorm
  t = Vector2d(3.0, 4.0);

  assert(std::abs(t.squaredNorm() - 25.0) < EPS);


  // Normalized
  u = t.normalized();

  assert(std::abs(t.getX() - 3.0) < EPS);
  assert(std::abs(t.getY() - 4.0) < EPS);
  assert(std::abs(u.getX() - 3.0/5.0) < EPS);
  assert(std::abs(u.getY() - 4.0/5.0) < EPS);


  // Normalize
  t.normalize();

  assert(std::abs(t.getX() - 3.0/5.0) < EPS);
  assert(std::abs(t.getY() - 4.0/5.0) < EPS);


  // Determinant
  u = Vector2d(1.0, 2.0);
  t = Vector2d(3.0, 2.0);

  float det = Vector2d::determinant(u, t);

  assert((det + 4.0) < EPS);


  // Are colinear
  u = Vector2d(1.0, 2.0);
  t = Vector2d(3.0, 6.0);

  assert(Vector2d::areColinear(u, t));

  t = -t;

  assert(Vector2d::areColinear(u, t));


  // Are orthogonal
  u = Vector2d(3.0, 4.0);
  t = Vector2d(-12.0, 9.0);

  assert(Vector2d::areOrthogonal(u, t));

  t = -t;

  assert(Vector2d::areOrthogonal(u, t));


  // Angle
  // PI and PI/2
  u = Vector2d(3.0, 2.0);
  t = Vector2d(6.0, 4.0);
  assert(std::abs(Vector2d::angle(u, t) - 0.0) < EPS);

  t = Vector2d(-4.0, 6.0);
  assert(std::abs(Vector2d::angle(u, t) - M_PI_2) < EPS);

  t = Vector2d(-6.0, -4.0);
  assert(std::abs(Vector2d::angle(u, t) - M_PI) < EPS);

  t = Vector2d(4.0, -6.0);
  assert(std::abs(Vector2d::angle(u, t) + M_PI_2) < EPS);

  // PI/4
  t = Vector2d(1.0, 5.0);
  assert(std::abs(Vector2d::angle(u, t) - M_PI_4) < EPS);

  t = Vector2d(-5.0, 1.0);
  assert(std::abs(Vector2d::angle(u, t) - 3*M_PI_4) < EPS);

  t = Vector2d(-1.0, -5.0);
  assert(std::abs(Vector2d::angle(u, t) + 3*M_PI_4) < EPS);

  t = Vector2d(5.0, -1.0);
  assert(std::abs(Vector2d::angle(u, t) + M_PI_4) < EPS);

  // PI/3
  float sqrt3Over2 = std::sqrt(3.0)/2.0;

  u = Vector2d(3.0, 0.0);
  t = Vector2d(sqrt3Over2, 0.5);
  //    std::cerr << Vector2d::angle(u, t) - M_PI_2/3.0 << std::endl;
  assert(std::abs(Vector2d::angle(u, t) - M_PI_2/3.0) < EPS);

  t = Vector2d(0.5, sqrt3Over2);
  assert(std::abs(Vector2d::angle(u, t) - M_PI/3.0) < EPS);

  t = Vector2d(-0.5, sqrt3Over2);
  assert(std::abs(Vector2d::angle(u, t) - 2*M_PI/3.0) < EPS);

  t = Vector2d(-sqrt3Over2, 0.5);
  assert(std::abs(Vector2d::angle(u, t) - 5*M_PI_2/3.0) < EPS);

  t = Vector2d(-sqrt3Over2, -0.5);
  assert(std::abs(Vector2d::angle(u, t) + 5*M_PI_2/3.0) < EPS);

  t = Vector2d(-0.5, -sqrt3Over2);
  assert(std::abs(Vector2d::angle(u, t) + 2*M_PI/3.0) < EPS);

  t = Vector2d(0.5, -sqrt3Over2);
  assert(std::abs(Vector2d::angle(u, t) + M_PI/3.0) < EPS);

  t = Vector2d(sqrt3Over2, -0.5);
  assert(std::abs(Vector2d::angle(u, t) + M_PI_2/3.0) < EPS);


  // Operator<<
  std::cerr << u << std::endl;
}

void Test::testPolygon(void) {
  Polygon p;
  std::cerr << p << std::endl;

  std::vector<Point2d> vertices;
  vertices.push_back(Point2d(5.4, 6.7));
  vertices.push_back(Point2d(8.3, 9.2));
  Polygon q(vertices);
  std::cerr << q << std::endl;

  Point2d A(1, 3), B(4.2, 3.5), C(1.4, 7.8);
  p << A << B << C;
  std::cerr << p << std::endl;

  q << A << B << C;
  std::cerr << q << std::endl;

  p << q.getVertices();
  std::cerr << p << std::endl;
  std::cerr << q << std::endl;
}

void Test::testXML(void) {
  ParserXML parser("../SliceIt/levels/testXML.xml");

  parser.setLinesCount(3);
  parser.setPartsCount(5);
  parser.setStarsCount(4);
  parser.setTolerances(7);

  Polygon p;
  Point2d A(100, 300), B(420, 350), C(140, 780);
  p << A << B << C;

  std::cerr << p.orientedArea() << std::endl;

  parser.addPolygon(p);
  parser.addHint(Hint(Segment(Point2d(23, 45), Point2d(362, 495)), 3));

  Polygon q;
  Point2d D(100, 100), E(400, 100), F(400, 400), G(100, 400);
  q << D << E << F << G;

  std::cerr << q.orientedArea() << std::endl;

  Polygon r;
  r << D << G << F << E;

  std::cerr << r.orientedArea() << std::endl;

  parser.addPolygon(q);
  parser.addPolygon(r);
  parser.addHint(Hint(Segment(Point2d(32, 55), Point2d(562, 395)), 2));
  parser.addPolygon(p);
  parser.removePolygon(0);

  parser.removeHint(0);
  parser.replacePolygon(p, 2);

  parser.writeXML();
}
