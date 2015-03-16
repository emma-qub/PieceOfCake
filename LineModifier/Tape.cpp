#include "Tape.h"

Tape::Tape(int x, int y, int w, int h):
  Obstacle(),
  _x(x),
  _y(y),
  _w(w),
  _h(h) {
}

Tape::~Tape(void) {
}

int Tape::getX() const
{
  return _x;
}

void Tape::setX(int x)
{
  _x = x;
}

int Tape::getY() const {
  return _y;
}

void Tape::setY(int y) {
  _y = y;
}

int Tape::getW() const {
  return _w;
}

void Tape::setW(int w) {
  _w = w;
}

int Tape::getH() const {
  return _h;
}

void Tape::setH(int h) {
  _h = h;
}

std::string Tape::getName(void) const {
  return "tape";
}

std::vector<Point2d> Tape::getTapeVertices(void) const {
  std::vector<Point2d> tapeVertices;
  tapeVertices.push_back(Point2d(_x, _y));
  tapeVertices.push_back(Point2d(_x+_w, _y));
  tapeVertices.push_back(Point2d(_x+_w, _y+_h));
  tapeVertices.push_back(Point2d(_x, _y+_w));

  return tapeVertices;
}

std::vector<Segment> Tape::getTapeEdges(void) const  {
  std::vector<Segment> tapeEdges;

  auto vertices = getTapeVertices();
  for (unsigned int k = 0; k < vertices.size(); ++k) {
    Point2d fstVertex = vertices.at(k);
    Point2d sndVertex = vertices.at((k+1)%vertices.size());
    tapeEdges.push_back(Segment(fstVertex, sndVertex));
  }


  return tapeEdges;
}

bool Tape::crossing(const Segment& line) const {
  auto edges = getTapeEdges();

  for (auto edge: edges) {
    if (edge.computeIntersection(line) == Segment::Regular) {
      return true;
    }
  }

  return false;
}
