#ifndef MIRROR_H
#define MIRROR_H

#include "Deviation.h"

class Mirror: public Deviation {
public:
  Mirror(int xa = -1, int ya = -1, int xb = -1, int yb = -1);
  ~Mirror(void);

  inline Segment getMirrorLine(void) const { return _mirrorLine; }

  std::string getName() const final;
  std::vector<Segment> deviateLine(const Segment& line) const final;

private:
  Segment _mirrorLine;
};

#endif // MIRROR_H
