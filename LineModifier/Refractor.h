#ifndef REFRACTOR_H
#define REFRACTOR_H

#include "Deviation.h"

class Refractor: public Deviation {
public:
  Refractor(int xa = -1, int ya = -1, int xb = -1, int yb = -1, float indice = 1.f);

  inline Segment getRefractorLine(void) const { return _refractorLine; }
  inline float getIndice(void) const { return _indice; }

  std::string getName() const final;
  std::vector<Segment> deviateLine(const Segment& line) const final;

private:
  Segment _refractorLine;
  float _indice;
};

#endif // REFRACTOR_H
