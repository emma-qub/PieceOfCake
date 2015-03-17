#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "LineModifier.h"
#include "Segment.h"

class Obstacle: public LineModifier {
public:
  Obstacle(void);
  ~Obstacle(void);

  virtual bool crossing(const Segment& line) const = 0;
};

#endif // OBSTACLE_H
