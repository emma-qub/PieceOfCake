#ifndef DEVIATION_H
#define DEVIATION_H

#include "LineModifier.h"
#include "Segment.h"

#include <vector>

class Deviation: LineModifier {
public:
  Deviation(void);

  virtual std::vector<Segment> deviateLine(const Segment& line) const = 0;
};

#endif // DEVIATION_H
