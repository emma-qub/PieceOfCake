#ifndef PORTAL_H
#define PORTAL_H

#include "Deviation.h"
#include "Vector2d.h"

class Portal: public Deviation {

public:
  Portal(int xaIn = -1, int yaIn = -1, int xbIn = -1, int ybIn = -1, int xaOut = -1, int yaOut = -1, int xbOut = -1, int ybOut = -1);
  Portal(const Segment& in, const Segment& out);
  virtual ~Portal(void);

  inline Segment getIn(void) const { return _in; }
  inline Segment getOut(void) const { return _out; }
  inline Vector2d getNormalIn(void) const { return _normalIn; }
  inline Vector2d getNormalOut(void) const { return _normalOut; }

  std::string getName() const final;
  std::vector<Segment> deviateLine(const Segment& line) const final;
  virtual std::vector<Segment> deviateLine2(const Segment& line) const final;

private:
  Segment _in;
  Segment _out;

  Vector2d _normalIn;
  Vector2d _normalOut;
};

#endif // PORTAL_H
