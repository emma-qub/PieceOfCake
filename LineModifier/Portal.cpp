#include "Portal.h"

#define cerro(x) std::cerr << x << std::endl

Portal::Portal(int xaIn, int yaIn, int xbIn, int ybIn, int xaOut, int yaOut, int xbOut, int ybOut):
  Deviation(),
  _in(Segment(xaIn, yaIn, xbIn, ybIn)),
  _out(Segment(xaOut, yaOut, xbOut, ybOut)) {

  Vector2d inVector(Vector2d::fromSegment(_in));
  _normalIn = Vector2d(-inVector.getY(), inVector.getX());

  Vector2d outVector(Vector2d::fromSegment(_out));
  _normalOut = Vector2d(-outVector.getY(), outVector.getX());
}

Portal::Portal(const Segment& in, const Segment& out):
  Deviation(),
  _in(in),
  _out(out) {

  Vector2d inVector(Vector2d::fromSegment(_in));
  _normalIn = Vector2d(-inVector.getY(), inVector.getX());

  Vector2d outVector(Vector2d::fromSegment(_out));
  _normalOut = Vector2d(-outVector.getY(), outVector.getX());
}

Portal::~Portal(void) {
}

std::string Portal::getName() const {
  return "portal";
}

std::vector<Segment> Portal::deviateLine(const Segment& line) const {
  std::vector<Segment> deviatedLines;

  Segment in(_in);
  Segment out(_out);
  if (_out.computeIntersection(line) == Segment::Regular) {
    out = _in;
    in = _out;
  }
  if (in.computeIntersection(line) == Segment::Regular) {
    // Compute theta the rotation's angle
    float theta = Vector2d::angle(Vector2d::fromSegment(in), Vector2d::fromSegment(out));

    // Compute in point (I) and out point (J)
    Point2d I(Segment::intersectionPoint(in, line));
    float ratio = Point2d::distance(in.getA(), I)/out.length();
    Point2d J = out.getA().applyVector(ratio*Vector2d::fromSegment(out));

    // Compute extended point
    Point2d A1 = I.applyVector(Vector2d(line.getA(), I));

    // Compute point after moving rotation's center to origin
    Point2d A2 = A1.applyVector(Vector2d(-I.getX(), -I.getY()));

    // Apply rotation
    Point2d A3(A2.getX()*cos(theta)-A2.getY()*sin(theta), A2.getX()*sin(theta)+A2.getY()*cos(theta));

    // Translate back
    Point2d A4 = A3.applyVector(Vector2d(J.getX(), J.getY()));

    // Add in line and out line
    deviatedLines.push_back(Segment(line.getA(), I));
    deviatedLines.push_back(Segment(J, A4));
  } else {
    deviatedLines.push_back(line);
  }

  return deviatedLines;
}
