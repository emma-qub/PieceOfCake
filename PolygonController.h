#ifndef POLYGONCONTROLLER_H
#define POLYGONCONTROLLER_H

#include "PolygonTreeModel.h"
#include "Commands.h"
#include <QUndoStack>
#include <QPoint>

class PolygonController : public QObject {
    Q_OBJECT

public:
    PolygonController(PolygonTreeModel* model, QUndoStack* undoStack, QObject* parent = 0);

    void initPolygon(void);

    void appendPolygon(void);
    void addPolygon(int polygonRow, const Polygon& polygon);
    void removePolygon(int polygonRow, const Polygon& polygon);
    void movePolygon(int polygonRow, int oldX, int oldY, int newX, int newY, bool pushToStack = true);

    void addVertex(int polygonRow, int vertexRow, const Point2d& vertex);
    void removeVertex(int polygonRow, int vertexRow, const Point2d& vertex);
    void moveVertex(int polygonRow, int vertexRow, int oldX, int oldY, int newX, int newY, bool pushToStack = true);

    void clear(void);

    void saveLevel(const QString& fileName);
    void openLevel(const QString& fileName);

    void alignToGrid(void);

public slots:
    void emitUpdate(int stackIndex);

signals:
    void update(void);
    void updateReset(void);
    void selection(void);

private:
    PolygonTreeModel* _model;
    QUndoStack* _undoStack;
};

#endif // POLYGONCONTROLLER_H
