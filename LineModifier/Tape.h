#ifndef TAPE_H
#define TAPE_H

#include "Obstacle.h"

#include <vector>

class Tape: public Obstacle {
public:
  Tape(int x, int y, int w, int h);
  ~Tape(void);

  int getX() const;
  void setX(int x);

  int getY() const;
  void setY(int y);

  int getW() const;
  void setW(int w);

  int getH() const;
  void setH(int h);

  std::vector<Point2d> getTapeVertices(void) const;
  std::vector<Segment> getTapeEdges(void) const;

  std::string getName(void) const final;
  bool crossing(const Segment& line) const final;

private:
  int _x;
  int _y;
  int _w;
  int _h;
};

#endif // TAPE_H
