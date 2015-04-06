#include "TestLevelTreeView.h"

TestLevelTreeView::TestLevelTreeView(TestLevelController* controller, QWidget* parent):
  QTreeView(parent),
  _controller(controller),
  _oldX(-1),
  _oldY(-1) {

  _delegate = new TestLevelTreeItemDelegate;
  setItemDelegate(_delegate);

  setItemsExpandable(false);

  connect(_delegate, SIGNAL(moveVertex(QModelIndex,int,bool)), this, SLOT(movingVertex(QModelIndex,int,bool)));
  connect(_controller, SIGNAL(update(void)), this, SLOT(expandAll(void)));
  connect(_controller, SIGNAL(selection(void)), this, SLOT(selection(void)));

  resize(375, 500);
}

void TestLevelTreeView::setModel(TestLevelModel* model) {
  QTreeView::setModel(model);
  _model = model;

  setColumnWidth(0, 120);
  for (int k = 1; k < 4; k++)
    setColumnWidth(k, 70);

  expandAll();

//  connect(_model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(debugDataChanged(QModelIndex,QModelIndex)));
}

void TestLevelTreeView::keyPressEvent(QKeyEvent* event) {
  if (event->key() == Qt::Key_Delete) {

    QModelIndex lineIndex;
    QModelIndex vertexIndex;

    QModelIndex currIndex = currentIndex();
    if (!currIndex.isValid()) {
      qDebug() << "Error within TestLevelTreeView::keyPressEvent: index is not valid";
      return;
    }

    if (currIndex.column() != 0)
      currIndex = _model->index(currIndex.row(), 0, currIndex.parent());

    QString fstData = _model->itemData(currIndex).value(Qt::DisplayRole).toString();

    // This is a vertex
    if (!fstData.startsWith("Line")) {
      vertexIndex = currIndex;
      lineIndex = vertexIndex.parent();
      _controller->removeVertex(lineIndex.row(), vertexIndex.row(), _model->vertexFromIndex(vertexIndex));
      // This is the header
    } else if (fstData == "Line") {
      return;
      // This is a line
    } else if (fstData.startsWith("Line ")) {
      _controller->removeLine(currIndex.row());
    } else {
      qDebug() << "Error within TestLevelTreeView::keyPressEvent: cannot know whether it's a line or a vertex";
      return;
    }
  }
}

void TestLevelTreeView::movingVertex(QModelIndex vertexIndex, int value, bool editModel) {
  int column = vertexIndex.column();
  TreeItem* currVertex = _model->getItem(vertexIndex);

  int newX = -1;
  int newY = -1;

  if (column == 2) {
    newX = value;
    newY = currVertex->data(3).toInt();
  } else if (column == 3) {
    newX = currVertex->data(2).toInt();
    newY = value;
  } else
    qDebug() << "Error: cannot edit this column within TestLevelTreeView::movingVertex";

  if (editModel) {
    _controller->moveVertex(vertexIndex.parent().row(), vertexIndex.row(), newX, newY);
  } else {
    emit updateViewNotModel(vertexIndex, value);
  }
}

//void TestLevelTreeView::debugDataChanged(QModelIndex, QModelIndex) {
//  qDebug() << "debugDataChanged";
//}

void TestLevelTreeView::updateAll(void) {
  collapseAll();
  expandAll();
}

void TestLevelTreeView::selection(void) {
  QStack<QPair<int, int>> selections = _model->getSelections();

  if (selections.isEmpty()) {
    qDebug() << "No no no, it can't be.";
    return;
  }

  int lineRow = selections.last().first;
  int vertexRow = selections.last().second;

  QModelIndex currIndex;
  if (lineRow == -1 && vertexRow != -1) {
    qDebug() << "Impossiburu";
    return;
  }
  else if (lineRow == -1 && vertexRow == -1) {
    qDebug() << "No line here.";
    return;
  } else if (lineRow != -1 && vertexRow == -1) {
    currIndex = _model->index(lineRow, 0);
  } else {
    currIndex = _model->index(vertexRow, 0, _model->index(lineRow, 0));
  }

  if (currIndex.isValid()) {
    selectionModel()->setCurrentIndex(currIndex, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
  } else {
    qDebug() << "currIndex is still not valid!";
  }
}
