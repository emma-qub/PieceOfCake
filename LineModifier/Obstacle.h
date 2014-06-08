#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "LineModifier.h"
#include "Segment.h"

class Obstacle: LineModifier {
public:
  Obstacle();

  virtual bool crossing(const Segment& line) = 0;
};

#endif // OBSTACLE_H
