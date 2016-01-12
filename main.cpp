#include <cstdlib>
#include <time.h>   // init rand

#include "tests.h"
#include "MainWindow.h"
#include "LevelDesignerModel.h"
#include "LevelDesignerScribbleView.h"
#include "GameInfo.h"
#include "LevelInfo.h"

#include <QtQml>

#define TESTPOINT 0
#define TESTVECTOR 0
#define TESTPOLYGON 0
#define TESTMODELVIEW 0
#define XML 0
#define GUI 1
#define GNUPLOT 0
#define QML 0
#define Thumbnail 0
#define PORTAL 0

#if Thumbnail
# include "ThumbnailCreator.h"
# include <QLabel>
#endif

#if QML
# include <QGuiApplication>
# include <QQmlApplicationEngine>
#endif

#if GUI
# include <QApplication>
#endif

#if PORTAL
# include "Portal.h"
#endif

int main(int argc, char** argv) {
  srand(time(NULL));  // init rand;

#if PORTAL
  Portal p(Segment(10, 40, 10, 20), Segment(40, 10, 20, 10));
  std::vector<Segment> lines(p.deviateLine(Segment(20, 40, 0, 30)));
  for (const Segment& line: lines)
    std::cerr << line << std::endl;
#endif

#if Thumbnail


  QApplication app(argc, argv);

  PolygonList polygonList;
  //polygonList << poc::Polygon(30, 250, 21, 260, 5);
  poc::Polygon polygon;
  polygon << Point2d(55, 55)
  << Point2d(455, 55)
  << Point2d(455, 455)
  << Point2d(155, 455)
  << Point2d(155, 255)
  << Point2d(255, 255)
  << Point2d(255, 355)
  << Point2d(355, 355)
  << Point2d(355, 155)
  << Point2d(55, 155);
  polygonList << polygon;
  qDebug() << polygonList;
  ThumbnailCreator thumbnailCreator(polygonList);
  QImage thumbnail = thumbnailCreator.makeThumbnail();

  QLabel* label = new QLabel;
  label->setPixmap(QPixmap::fromImage(thumbnail));
  label->show();

  return app.exec();
#endif

#if TESTPOINT
  Test::test<< Point2d();
#endif

#if TESTVECTOR
  Test::testVector2d();
#endif

#if TESTPOLYGON
  Test::testPolygon();
#endif

#if GUI
  QApplication app(argc, argv);

//  QFontDatabase::addApplicationFont("../PieceOfCake/fonts/edosz.ttf");
//  QFontDatabase::addApplicationFont("../PieceOfCake/fonts/watermark.ttf");

  qmlRegisterType<GameInfo>("gameinfo", 1, 0, "GameInfo");
  qmlRegisterType<LevelInfo>("levelinfo", 1, 0, "LevelInfo");

  MainWindow window;
  window.show();

  return app.exec();
#endif

#if QML
  QGuiApplication app(argc, argv);

  QQmlApplicationEngine engine;
  engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

  return app.exec();
#endif

#if GNUPLOT
  // poc::Polygon polygon;
  // polygon << << Point2d() << << Point2d(0, 1) << << Point2d(1, 1) << << Point2d(1);

  poc::Polygon polygon(0, 100, 0, 100, 7);
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
