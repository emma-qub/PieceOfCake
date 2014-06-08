#ifndef GAMECONTROLLER_HXX
#define GAMECONTROLLER_HXX

#include "AbstractController.h"

#include "GameModel.h"

class GameController: public AbstractController {
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

  void newComputeIntersections(const Polygon& polygon, const Segment& line, QVector<QPair<Point2d, Segment::Intersection>>& intersections, QMap<Segment, Point2d>& mapSegmentIntersection) const;
  void splitVertices(const Polygon& polygon, const Segment& line, QVector<Point2d>& rightVertices, QVector<Point2d>& leftVertices) const;
  QVector<Segment> computeNewEdges(const QVector<QPair<Point2d, Segment::Intersection>>& intersections) const;
  Point2d findOtherBound(const Point2d& fstBound, const QVector<Segment>& newEdges) const;
  void createPolygons(QVector<Point2d>& oldPolygonVertices, std::vector<Point2d>& newPolygonVertices, const Polygon& oldPolygon, const QMap<Segment, Point2d>& mapSegmentIntersection, const QVector<Segment>& newEdges) const;
  void shiftLineIfEdgeCut(const Polygon& polygon, Segment& line) const;
  void sliceIt(Segment& line);
  LineType computeLineType(const Segment& line) const;

public slots:
  virtual void openLevel(const QString& fileName);
  virtual void saveLevel(const QString& fileName) { Q_UNUSED(fileName) }

signals:
  void updateLine(LineType);
  void update(void);

private:
  GameModel* _model;
  int _linesCount;
  int _partsCount;
  int _linesDrawn;
  int _polygonsCount;
};

bool pairCompare(const QPair<Point2d, Segment::Intersection>& fstElem, const QPair<Point2d, Segment::Intersection>& sndElem);

#endif // GAMECONTROLLER_HXX