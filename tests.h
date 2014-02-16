#ifndef TESTS_H
#define TESTS_H

#include <cassert>

#include "Point2d.h"
#include "Vector2d.h"
#include "Polygon.h"
#include "LevelDesignerModel.h"
#include "ParserXML.h"

class Test {

public:
    Test(void) {}
    virtual ~Test(void) {}

    static void testPoint2d(void);
    static void testVector2d(void);
    static void testPolygon(void);
    static void testModelView(void);
    static void testXML(void);

private:
    static void permute(std::vector<Point2d>& v);
};



#endif // TESTS_H
