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

  PolygonList splitSmartVertices(const std::vector<std::pair<Point2d, bool>>& smartVertices) const;
  PolygonList cutPolygon(const Polygon& currPolygon, const Segment& line);
  PolygonList cutPolygons(const Segment& line);
  void computeCuttingSegments(void);
  void sliceIt(const Segment& line);

  bool isACuttingSegment(const Segment& segment) const;

  inline std::vector<Segment> getCuttingSegments(void) { return _cuttingSegments; }

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
  std::vector<Point2d> _newVertices;
  std::vector<Segment> _cuttingSegments;
};

#endif // GAMECONTROLLER_HXX
