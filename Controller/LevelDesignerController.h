#ifndef POLYGONCONTROLLER_H
#define POLYGONCONTROLLER_H

#include "AbstractController.h"
#include "LevelDesignerModel.h"
#include "Commands.h"
#include "LevelInfo.h"

#include <QPoint>

class LevelDesignerController: public AbstractController {
  Q_OBJECT

public:
  LevelDesignerController(LevelDesignerModel* model, QUndoStack* undoStack, LevelInfo* levelInfo, QObject* parent = 0);
  virtual ~LevelDesignerController(void);

  LevelInfo* getLevelInfo(void) const;

  void initPolygon(void);

  void addPolygon(int polygonRow, const poc::Polygon& polygon);
  void removePolygon(int polygonRow, const poc::Polygon& polygon);
  void movePolygon(int polygonRow, int oldX, int oldY, int newX, int newY, bool pushToStack = true);

  void addVertex(int polygonRow, int vertexRow, const Point2d& vertex);
  void removeVertex(int polygonRow, int vertexRow, const Point2d& vertex);
  void moveVertex(int polygonRow, int vertexRow, int oldX, int oldY, int newX, int newY, bool pushToStack = true);

  void updateLinesCount(int linesCount);
  void updatePartsCount(int partsCount);
  void updateMaxGapToWin(int maxGapToWin);

  void clear(void);

public slots:
  void appendPolygon(void);
  void saveLevel(const QString& fileName);
  void openLevel(const QString& fileName);
  void alignToGrid(void);
  void emitUpdate(int stackIndex);

signals:
  void update(void);
  void updateStats(void);
  void enableStats(bool);
  void updateReset(void);
  void selection(void);

protected:
  LevelDesignerModel* _model;

private:
  LevelInfo* _levelInfo;
};

#endif // POLYGONCONTROLLER_H
