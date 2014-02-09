#ifndef POLYGONTREEVIEW_H
#define POLYGONTREEVIEW_H

#include <QTreeView>
#include <QUndoStack>
#include "PolygonTreeModel.h"
#include "PolygonController.h"
#include "PolygonItemDelegate.h"

class PolygonTreeView : public QTreeView {
    Q_OBJECT

public:
    PolygonTreeView(PolygonController* controller, QWidget* parent = 0);

    virtual void setModel(PolygonTreeModel* model);
    QAbstractItemModel* model() const { return _model; }

    virtual void keyPressEvent(QKeyEvent* event);

public slots:
    void movingVertex(QModelIndex vertexIndex, int value, bool editModel = false);
    void debugDataChanged(QModelIndex, QModelIndex);
    void updateAll(void);
    void selection(void);

signals:
    void updateViewNotModel(QModelIndex, int);

protected:
    QUndoStack* _undoStack;
    PolygonTreeModel* _model;
    PolygonController* _controller;
    PolygonItemDelegate* _delegate;
    int _oldX;
    int _oldY;
};

#endif // POLYGONTREEVIEW_H
