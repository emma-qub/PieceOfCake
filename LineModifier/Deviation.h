#ifndef DEVIATION_H
#define DEVIATION_H

#include "LineModifier.h"
#include "Segment.h"

#include <vector>

class Deviation: LineModifier {
public:
  Deviation();

  virtual std::vector<Segment> deviateLine(const Segment& line) = 0;
};

#endif // DEVIATION_H
