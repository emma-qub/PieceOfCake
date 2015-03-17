#include "Portal.h"

#define cerro(x) std::cerr << x << std::endl

Portal::Portal(const Segment& entrance, const Segment& exit):
  Deviation(),
  _entrance(entrance),
  _exit(exit) {

  Vector2d entranceVector(Vector2d::fromSegment(_entrance));
  _normalEntrance = Vector2d(-entranceVector.getY(), entranceVector.getX());

  Vector2d exitVector(Vector2d::fromSegment(_exit));
  _normalExit = Vector2d(-exitVector.getY(), exitVector.getX());
}

Portal::~Portal(void) {
}

std::string Portal::getName() const {
  return "portal";
}

std::vector<Segment> Portal::deviateLine(const Segment& line) const {
  std::vector<Segment> deviatedLines;

  if (_entrance.computeIntersection(line) == Segment::Regular) {
    // Compute theta the rotation's angle
    float theta = Vector2d::angle(Vector2d::fromSegment(_entrance), Vector2d::fromSegment(_exit));

    // Compute entrance point (I) and exit point (J)
    Point2d I(Segment::intersectionPoint(_entrance, line));
    float ratio = Point2d::distance(_entrance.getA(), I)/_exit.length();
    Point2d J = _exit.getA().applyVector(ratio*Vector2d::fromSegment(_exit));

    cerro("I ");cerro(I);cerro("");
    cerro("J ");cerro(J);cerro("");

    // Compute extended point
    Point2d A1 = I.applyVector(Vector2d(line.getA(), I));
    cerro("A1 ");cerro(A1);cerro("");

    // Compute point after moving rotation's center to origin
    Point2d A2 = A1.applyVector(Vector2d(-I.getX(), -I.getY()));
    cerro("A2 ");cerro(A2);cerro("");

    // Apply rotation
    Point2d A3(A2.getX()*cos(theta)-A2.getY()*sin(theta), A2.getX()*sin(theta)+A2.getY()*cos(theta));
    cerro("A3 ");cerro(A3);cerro("");

    // Translate back
    Point2d A4 = A3.applyVector(Vector2d(J.getX(), J.getY()));
    cerro("A4 ");cerro(A4);cerro("");

    // Add entrance line and exit line
    deviatedLines.push_back(Segment(line.getA(), I));
    deviatedLines.push_back(Segment(J, A4));
  } else {
    deviatedLines.push_back(line);
  }

  return deviatedLines;
}
