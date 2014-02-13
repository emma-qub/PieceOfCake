#ifndef GAMECONTROLLER_HXX
#define GAMECONTROLLER_HXX

#include <QObject>

#include "GameModel.h"

class GameController : public QObject {
  Q_OBJECT

public:
  enum LineType {
    goodCrossing,
    badCrossing,
    noCrossing,
    unknownCrossing
  };

  GameController(GameModel* model, QObject* parent = 0);

  void sortSmartVertices(std::vector<std::pair<Point2d, bool>>& smartVertices) const;
  PolygonList splitSmartVertices(const std::vector<std::pair<Point2d, bool>>& smartVertices) const;
  PolygonList cutPolygons(const Segment& line);
  void computeCuttingSegments(void);
  bool isACuttingSegment(const Segment& segment) const;

  inline std::vector<Segment> getCuttingSegments(void) { return _cuttingSegments; }
  inline void clear(void) { _newVertices.clear(); _cuttingSegments.clear(); }

  LineType computeLineType(const Segment& line) const;
  void openLevel(const QString& fileName);

signals:
  void updateLine(LineType);

private:
  GameModel* _model;
  std::vector<Point2d> _newVertices;
  std::vector<Segment> _cuttingSegments;
};

#endif // GAMECONTROLLER_HXX
