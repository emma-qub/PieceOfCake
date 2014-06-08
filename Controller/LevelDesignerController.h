#ifndef POLYGONCONTROLLER_H
#define POLYGONCONTROLLER_H

#include "AbstractController.h"
#include "LevelDesignerModel.h"
#include "Commands.h"

#include <QPoint>

class LevelDesignerController: public AbstractController {
  Q_OBJECT

public:
  LevelDesignerController(LevelDesignerModel* model, QWidget* tabWidget, QUndoStack* undoStack, QObject* parent = 0);

  void initPolygon(void);

  void appendPolygon(void);
  void addPolygon(int polygonRow, const Polygon& polygon);
  void removePolygon(int polygonRow, const Polygon& polygon);
  void movePolygon(int polygonRow, int oldX, int oldY, int newX, int newY, bool pushToStack = true);

  void addVertex(int polygonRow, int vertexRow, const Point2d& vertex);
  void removeVertex(int polygonRow, int vertexRow, const Point2d& vertex);
  void moveVertex(int polygonRow, int vertexRow, int oldX, int oldY, int newX, int newY, bool pushToStack = true);

  void clear(void);

public slots:
  void saveLevel(const QString& fileName);
  void openLevel(const QString& fileName);
  void alignToGrid(void);
  void emitUpdate(int stackIndex);

signals:
  void update(void);
  void updateReset(void);
  void selection(void);

protected:
  LevelDesignerModel* _model;
};

#endif // POLYGONCONTROLLER_H