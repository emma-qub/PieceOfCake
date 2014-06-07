#include "Portal.h"

int Portal::_length = 10;

Portal::Portal():
  LineModifier(),
  _fstPosition(),
  _sndPosition(),
  _fstDirection(),
  _sndDirection() {
}

std::vector<Segment> Portal::transform(const Segment& line) {
  std::vector<Segment> res;

  Segment fstPortalSegment = computePortalSegment(0);
  Segment sndPortalSegment = computePortalSegment(1);

  if (fstPortalSegment.computeIntersection(line) == Segment::None
      && sndPortalSegment.computeIntersection(line) == Segment::None) {
    res.push_back(line);
    return res;
  }

  return res;
}

Segment Portal::computePortalSegment(bool sndPortal) {
  Point2d center = sndPortal ? _sndPosition : _fstPosition;
  Vector2d direction = sndPortal ? _sndDirection : _fstDirection;

  Vector2d segmentDirection(direction.getY(), -direction.getX());
  segmentDirection.normalize();

  Vector2d translateVector = _length/2*segmentDirection;
  return Segment(center.applyVector(-translateVector), center.applyVector(translateVector));
}
