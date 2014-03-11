#ifndef TAPE_H
#define TAPE_H

#include <LineModifier.h>

class Tape : public LineModifier {

public:
    Tape();

    std::vector<Segment> transform(const Segment&);
};

#endif // TAPE_H
