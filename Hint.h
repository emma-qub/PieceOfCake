#ifndef HINT_H
#define HINT_H

#include <vector>
#include "Segment.h"

class Hint {

public:
    Hint(Segment line = Segment(), int cost = 0);
    virtual ~Hint(void) {}

    inline Segment getLine(void) const { return _line; }
    inline void setLine(const Segment& line) { _line = line; }
    inline int getCost(void) const { return _cost; }
    inline void setCost(int cost) { _cost = cost; }

private:
    Segment _line;
    int _cost;
};

#endif // HINT_H
