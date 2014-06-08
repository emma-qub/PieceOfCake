#ifndef LINEMODIFIER_H
#define LINEMODIFIER_H

#include "Polygon.h"

#include <string>

class LineModifier {
public:
  LineModifier();

  virtual std::string getName(void) const = 0;
};

#endif // LINEMODIFIER_H
