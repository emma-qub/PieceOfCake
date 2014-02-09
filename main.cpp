#include <cstdlib>
#include <time.h>   // init rand

#include <QApplication>

#include "tests.h"
#include "MainWindow.h"
#include "PolygonTreeModel.h"
#include "PolygonEditorView.h"
#include "PolygonScribbleView.h"

#define TESTPOINT 0
#define TESTVECTOR 0
#define TESTPOLYGON 0
#define TESTMODELVIEW 0
#define XML 0
#define GUI 1
#define GNUPLOT 0

#define toto *(1+1)

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

#if TESTMODELVIEW
    QList<Polygon> polygonList;

    Polygon p;
    Point2d A(10, 10), B(70, 50), C(10, 50);
    p << A << B << C;

    Polygon q;
    Point2d D(200, 80), E(250, 80), F(250, 300), G(200, 300);
    q << D << E << F << G;

    int xMin = 0;
    int yMin = 0;
    int xMax = 400;
    int yMax = 400;

    polygonList << p << q;
    polygonList << Polygon(xMin, xMax, yMin, yMax, 5)/*
                << Polygon(xMin, xMax, yMin, yMax, 5)
                << Polygon(xMin, xMax, yMin, yMax, 5)
                << Polygon(xMin, xMax, yMin, yMax, 5)
                << Polygon(xMin, xMax, yMin, yMax, 5)
                << Polygon(xMin, xMax, yMin, yMax, 5)
                << Polygon(xMin, xMax, yMin, yMax, 5)
                << Polygon(xMin, xMax, yMin, yMax, 5)
                << Polygon(xMin, xMax, yMin, yMax, 5)
                << Polygon(xMin, xMax, yMin, yMax, 5)
                << Polygon(xMin, xMax, yMin, yMax, 5)
                << Polygon(xMin, xMax, yMin, yMax, 5)
                << Polygon(xMin, xMax, yMin, yMax, 5)
                << Polygon(xMin, xMax, yMin, yMax, 5)
                << Polygon(xMin, xMax, yMin, yMax, 5)
                << Polygon(xMin, xMax, yMin, yMax, 5)
                << Polygon(xMin, xMax, yMin, yMax, 5)*/;

    PolygonTreeModel* model = new PolygonTreeModel;
    model->setPolygonList(polygonList);

    QApplication app(argc, argv);

    QSplitter* splitter = new QSplitter;
    splitter->resize(1000, 200);

    PolygonScribbleView* scribbleView = new PolygonScribbleView(splitter);
    scribbleView->setModel(model);

    PolygonEditorItemDelegate* itemDelegate = new PolygonEditorItemDelegate;

    QTreeView* treeView = new QTreeView(splitter);
    treeView->setModel(model);
    treeView->setItemDelegate(itemDelegate);

    splitter->show();

    return app.exec();
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
