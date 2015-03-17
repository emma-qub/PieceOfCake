#include "Mirror.h"

#include "Vector2d.h"

Mirror::Mirror(int xa, int ya, int xb, int yb):
  Deviation(),
  _mirrorLine(xa, ya, xb, yb) {
}

Mirror::~Mirror(void) {
}

std::string Mirror::getName() const {
  return "mirror";
}

std::vector<Segment> Mirror::deviateLine(const Segment& line) const {
  std::vector<Segment> deviatedLines;

  if (_mirrorLine.computeIntersection(line) == Segment::Regular) {
    Point2d intersection(Segment::intersectionPoint(_mirrorLine, line));

    // Mirror's normal
    //Vector2d n(line.getNormal());

    // mirror's bounds coordinates
    int xc = _mirrorLine.getA().getX();
    int yc = _mirrorLine.getA().getY();
    int xd = _mirrorLine.getB().getX();
    int yd = _mirrorLine.getB().getY();

    // Intersection's coordinates
    int xi = intersection.getX();
    int yi = intersection.getY();

    // First line's coordinates
    int xa = line.getA().getX();
    int ya = line.getA().getY();

    // System's determinant
    float det = (xd-xc)*(xd-xc) + (yd-yc)*(yd-yc);
    // det == 0 <=> mirror's bound are confounded. It shall not happen.
    assert(det != 0);

    // Orthogonal projection of line's first bound on the line
    // normal to the mirror's line and going through th intersection
    float detxj = (xd-xc)*((xd-xc)*xi+(yd-yc)*yi) - (yd-yc)*((yc-yd)*xa+(xd-xc)*ya);
    int xj = qRound(detxj / det);
    float detyj = ((yc-yd)*xa+(xd-xc)*ya)*(xd-xc) - ((xd-xc)*xi+(yd-yc)*yi)*(yc-yd);
    int yj = qRound(detyj / det);

    // Symetry of first line intersection
    int xb = 2*xj - xa;
    int yb = 2*yj - ya;

    // Add line and reflected line
    deviatedLines.push_back(Segment(line.getA(), intersection));
    deviatedLines.push_back(Segment(intersection, Point2d(xb, yb)));
  } else {
    deviatedLines.push_back(line);
  }

  return deviatedLines;
}
