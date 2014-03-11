#ifndef LINEMODIFIER_H
#define LINEMODIFIER_H

#include "Segment.h"
#include <vector>

class LineModifier
{
public:
    LineModifier();

    virtual std::vector<Segment> transform(const Segment& line) = 0;
};

#endif // LINEMODIFIER_H
