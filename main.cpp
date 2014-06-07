#include <cstdlib>
#include <time.h>   // init rand

#include <QApplication>

#include "tests.h"
#include "MainWindow.h"
#include "LevelDesignerModel.h"
#include "LevelDesignerScribbleView.h"

#define TESTPOINT 0
#define TESTVECTOR 0
#define TESTPOLYGON 0
#define TESTMODELVIEW 0
#define XML 0
#define GUI 1
#define GNUPLOT 0

int main(int argc, char** argv) {
  srand(time(NULL));  // init rand;

#if TESTPOINT
  Test::testPoint2d();
#endif

#if TESTVECTOR
  Test::testVector2d();
#endif

#if TESTPOLYGON
  Test::testPolygon();
#endif

#if GUI
  QApplication app(argc, argv);

  QFontDatabase::addApplicationFont("../SliceIt/fonts/edosz.ttf");
  QFontDatabase::addApplicationFont("../SliceIt/fonts/watermark.ttf");

  MainWindow window;
  window.show();

  return app.exec();
#endif

#if GNUPLOT
  // Polygon polygon;
  // polygon << Point2d() << Point2d(0, 1) << Point2d(1, 1) << Point2d(1);

  Polygon polygon(0, 100, 0, 100, 7);
  //    std::vector<Point2d> vertices = polygon.getVertices();
  //    for (unsigned int k = 0; k < vertices.size(); ++k)
  //        std::cerr << vertices.at(k).getX() << " " << vertices.at(k).getY() << " " << -10 << std::endl;

  float size = 300;
  float delta = 10;
  int max = size/delta;

  float** matrix = polygon.surface(size, delta);
  for (int i = 0; i < max; ++i) {
    for (int j = 0; j < max; ++j) {
      std::cerr << i << " " << j << " " << matrix[i][j] << std::endl;
    }
    delete[] matrix[i];
  }
  delete[] matrix;

#endif

#if XML
  Test::testXML();
#else
  return EXIT_SUCCESS;
#endif
}
