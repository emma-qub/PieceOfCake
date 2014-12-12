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
  enum Ranking {
    fail = 0,
    poor = 1,
    average = 2,
    nice = 3,
    good = 4,
    great = 5,
    perfect = 6
  };

  GameController(GameModel* model, QUndoStack* undoStack, QObject* parent = 0);

  inline int getLinesCount(void) const { return _linesCount; }
  inline int getPartsCount(void) const { return _partsCount; }
  inline int getLinesDrawn(void) const { return _linesDrawn; }
  inline int getPolygonsCount(void) const { return _polygonsCount; }
  inline bool isLevelRunning() const { return _levelRunning; }

  Point2d* getOtherBound(const Point2d* intersection, const std::vector<std::pair<Point2d*, Point2d*>>& cuttingSegments) const;
  std::vector<std::pair<Point2d*, Point2d*>> getCuttingSegments(const std::vector<Point2d*>& intersections) const;
  bool stillHasBaseVertices(const std::vector<Point2d*>& verticesGlobal, const std::vector<Point2d*>& intersections) const;
  void cleanIntersections(const Polygon& polygon, std::vector<Point2d*>& intersections) const;
  void getVerticesAndIntersections(const Segment& line, const Polygon& polygon,
    std::vector<Point2d*>& globalVertices, std::vector<Point2d*>& intersections) const;
  void sliceIt(const Segment& line);

  LineType computeLineType(const Segment& line) const;
  void checkWinning(void);
  void replay(void);
  void clearGame(void);

public slots:
  virtual void openLevel(const QString& fileName);
  virtual void saveLevel(const QString& fileName) { Q_UNUSED(fileName) }

signals:
  void updateLine(LineType);
  void update(void);
  void levelEnd(QList<float>, GameController::Ranking);

private:
  GameModel* _model;
  int _linesCount;
  int _partsCount;
  int _linesDrawn;
  int _polygonsCount;
  float _orientedAreaTotal;
  int _maxGapToWin;
  QString _fileName;
  bool _levelRunning;
};

bool pointCompare(const Point2d* A, const Point2d* B);

#endif // GAMECONTROLLER_HXX
