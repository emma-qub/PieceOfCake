#ifndef SEGMENT_H
#define SEGMENT_H

#include "Point2d.h"

class Segment {

public:
    enum Side { OnLeft, OnSegmentInside, OnSegmentOutside, OnRight, IsBoundA, IsBoundB };
    enum Intersection { Regular, FirstVertexRegular, SecondVertexRegular, Edge, VertexUseless, EdgeUseless, None };

    Segment(const Point2d& a = Point2d(), const Point2d& b = Point2d());
    Segment(const Segment& segment);
    virtual ~Segment(void) {}

    inline Point2d getA(void) const { return _boundaries[0]; }
    inline Point2d getB(void) const { return _boundaries[1]; }
    inline void setA(const Point2d& a) { _boundaries[0] = a; }
    inline void setB(const Point2d& b) { _boundaries[1] = b; }

    Point2d getOtherBoundary(const Point2d& boundary);

    Side location(const Point2d& P) const;
    bool sameSide(const Point2d& P, const Point2d& Q) const;
    Intersection computeIntersection(const Point2d& P, const Point2d& Q) const;
    Intersection computeIntersection(const Segment& PQ) const;
    static Point2d intersectionPoint(const Segment& AB, const Segment& PQ);

    float orientedArea(void) const;

    friend bool operator==(const Segment& segment1, const Segment& segment2);
    friend bool operator<(const Segment& segment1, const Segment& segment2);

    Segment& operator=(const Segment& segment);

    friend std::ostream& operator<<(std::ostream& os, const Segment& segment);

    friend QDebug operator<<(QDebug d, const Segment& segment);

private:
    Point2d _boundaries[2];
};

#endif // SEGMENT_H
