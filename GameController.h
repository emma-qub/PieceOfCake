#ifndef GAMECONTROLLER_HXX
#define GAMECONTROLLER_HXX

#include "AbstractController.h"

#include "GameModel.h"

class GameController : public AbstractController {
  Q_OBJECT

public:
  enum LineType {
    goodCrossing,
    badCrossing,
    noCrossing,
    unknownCrossing
  };

  GameController(GameModel* model, QWidget* tabWidget, QUndoStack* undoStack, QObject* parent = 0);

  inline int getLinesCount(void) const { return _linesCount; }
  inline int getPartsCount(void) const { return _partsCount; }
  inline int getLinesDrawn(void) const { return _linesDrawn; }
  inline int getPolygonsCount(void) const { return _polygonsCount; }

//  PolygonList splitSmartVertices(const QVector<QPair<Point2d, bool> >& smartVertices) const;
//  PolygonList cutPolygon(const Polygon& currPolygon, const Segment& line);
//  PolygonList cutPolygons(const Segment& line);
//  void computeCuttingSegments(void);
//  QVector<QPair<Point2d, Segment::Intersection> > computeIntersections(const Polygon& polygon, const Segment& line) const;
//  QList<Segment> computeSegments(const Polygon& polygon, const QVector<QPair<Point2d, Segment::Intersection> >& intersections) const;
//  bool isACuttingSegment(const Segment& segment) const;
//  inline QVector<Segment> getCuttingSegments(void) { return _cuttingSegments; }

  QVector<QPair<Point2d, Segment::Intersection>> newComputeIntersections(const Polygon& polygon, const Segment& line) const;
  void splitVertices(const Polygon& polygon, const Segment& line, QVector<Point2d>& rightVertices, QVector<Point2d>& leftVertices) const;
  QVector<Segment> computeNewEdges(const QVector<QPair<Point2d, Segment::Intersection>>& intersections) const;
  void sliceIt(const Segment& line);
  LineType computeLineType(const Segment& line) const;

public slots:
  inline virtual void clear(void) { AbstractController::clear(); _newVertices.clear(); _cuttingSegments.clear(); }
  virtual void openLevel(const QString& fileName);
  virtual void saveLevel(const QString& fileName) { Q_UNUSED(fileName) }

signals:
  void updateLine(LineType);
  void update(void);

private:
  GameModel* _model;
  QVector<Point2d> _newVertices;
  QVector<Segment> _cuttingSegments;
  int _linesCount;
  int _partsCount;
  int _linesDrawn;
  int _polygonsCount;
};

bool pairCompare(const QPair<Point2d, Segment::Intersection>& fstElem, const QPair<Point2d, Segment::Intersection>& sndElem);

#endif // GAMECONTROLLER_HXX
