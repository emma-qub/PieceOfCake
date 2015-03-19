#include "Refractor.h"
#include "Vector2d.h"

Refractor::Refractor(int xa, int ya, int xb, int yb, float indice):
  Deviation(),
  _refractorLine(xa, ya, xb, yb),
  _indice(indice) {
}

std::string Refractor::getName() const {
  return "refractor";
}

std::vector<Segment> Refractor::deviateLine(const Segment& line) const {
  std::vector<Segment> deviatedLines;

  if (_refractorLine.computeIntersection(line) == Segment::Regular) {
    // Compute line - refractor intersection
    Point2d I(Segment::intersectionPoint(_refractorLine, line));

    // Compute i1 the angle between line and refractor's normal
    Vector2d n(_refractorLine.getNormal());
    float i1 = Vector2d::angle(n, line.getDirection());
    if (i1 > M_PI/2) {
      i1 -= M_PI;
      n *= -1.f;
    } else if (i1 < -M_PI/2) {
      i1 += M_PI;
      n *= -1.f;
    }

    // Compute refracted angle i2
    float i2 = std::asin(std::sin(i1)/_indice);

    // Compute direction of refracted line
    float det = n.squaredNorm();  // nx²+ny²
    assert(det != 0.f);
    float nx = n.getX();
    float ny = n.getY();
    Vector2d v((nx*cos(i2) - ny*sin(i2))/det, (nx*sin(i2) + ny*cos(i2))/det);

    // Compute other bound of exit line
    Point2d P = I + Point2d::distance(line.getA(), I)*v;

    // Add line and refracted line
    deviatedLines.push_back(Segment(line.getA(), I));
    deviatedLines.push_back(Segment(I, P));
  } else {
    deviatedLines.push_back(line);
  }

  return deviatedLines;
}
