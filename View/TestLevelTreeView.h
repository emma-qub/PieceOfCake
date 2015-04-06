#ifndef TESTLEVELTREEVIEW_H
#define TESTLEVELTREEVIEW_H

#include <QTreeView>

#include "TestLevelModel.h"
#include "TestLevelController.h"
#include "TestLevelTreeItemDelegate.h"

class TestLevelTreeView: public QTreeView {
  Q_OBJECT

public:
  TestLevelTreeView(TestLevelController* controller, QWidget* parent = NULL);

  virtual void setModel(TestLevelModel* model);
  QAbstractItemModel* model() const { return _model; }

  virtual void keyPressEvent(QKeyEvent* event);

public slots:
  void movingVertex(QModelIndex vertexIndex, int value, bool editModel = false);
//  void debugDataChanged(QModelIndex, QModelIndex);
  void updateAll(void);
  void selection(void);

signals:
  void updateViewNotModel(QModelIndex, int);

protected:
  TestLevelModel* _model;
  TestLevelController* _controller;
  TestLevelTreeItemDelegate* _delegate;
  int _oldX;
  int _oldY;
};

#endif // TESTLEVELTREEVIEW_H
