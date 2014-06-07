#ifndef POLYGONTREEVIEW_H
#define POLYGONTREEVIEW_H

#include <QTreeView>
#include <QUndoStack>

#include "LevelDesignerModel.h"
#include "LevelDesignerController.h"
#include "LevelDesignerTreeItemDelegate.h"

class LevelDesignerTreeView: public QTreeView {
  Q_OBJECT

public:
  LevelDesignerTreeView(LevelDesignerController* controller, QWidget* parent = 0);

  virtual void setModel(LevelDesignerModel* model);
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
  LevelDesignerModel* _model;
  LevelDesignerController* _controller;
  LevelDesignerTreeItemDelegate* _delegate;
  int _oldX;
  int _oldY;
};

#endif // POLYGONTREEVIEW_H
