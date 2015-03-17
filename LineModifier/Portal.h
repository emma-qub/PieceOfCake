#ifndef PORTAL_H
#define PORTAL_H

#include "Deviation.h"
#include "Vector2d.h"

class Portal: public Deviation {

public:
  Portal(const Segment& entrance, const Segment& exit);
  virtual ~Portal(void);

  std::string getName() const final;
  std::vector<Segment> deviateLine(const Segment& line) const final;

private:
  Segment _entrance;
  Segment _exit;

  Vector2d _normalEntrance;
  Vector2d _normalExit;
};

#endif // PORTAL_H
