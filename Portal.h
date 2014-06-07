#ifndef PORTAL_H
#define PORTAL_H

#include "LineModifier.h"
#include "Vector2d.h"

class Portal: LineModifier {
public:
  static int _length;

  Portal(void);

  std::vector<Segment> transform(const Segment& line);

private:
  Segment computePortalSegment(bool sndPortal);

  Point2d _fstPosition;
  Point2d _sndPosition;
  Vector2d _fstDirection;
  Vector2d _sndDirection;
};

#endif // PORTAL_H
