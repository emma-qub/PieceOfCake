#include "LevelDesignerController.h"
#include "ParserXML.h"

#include <QDebug>

LevelDesignerController::LevelDesignerController(LevelDesignerModel *model, QWidget* tabWidget, QUndoStack* undoStack, QObject* parent):
  AbstractController(model, tabWidget, undoStack, parent),
  _model(model) {

  connect(_undoStack, SIGNAL(indexChanged(int)), this, SLOT(emitUpdate(int)));
}

void LevelDesignerController::addPolygon(int polygonRow, const Polygon& polygon) {
  QUndoCommand* addPolygonCommand = new AddPolygonCommand(_model, polygonRow, polygon, polygonRow, -1);
  _undoStack->push(addPolygonCommand);
}

void LevelDesignerController::removePolygon(int polygonRow, const Polygon& polygon) {
  int newPolygonRow = polygonRow;
  if (_model->rowCount() == 0) {
    newPolygonRow = -1;
  } else {
    if (polygonRow == _model->rowCount()-1) {
      newPolygonRow = polygonRow-1;
    }
  }

  QUndoCommand* removePolygonCommand = new RemovePolygonCommand(_model, polygonRow, polygon, newPolygonRow, -1);
  _undoStack->push(removePolygonCommand);
}

void LevelDesignerController::movePolygon(int polygonRow, int oldX, int oldY, int newX, int newY, bool pushToStack) {
  if (pushToStack) {
    QUndoCommand* movePolygonCommand = new MovePolygonCommand(_model, polygonRow, oldX, oldY, newX, newY, polygonRow, -1);
    _undoStack->push(movePolygonCommand);
  } else {
    Polygon polygon(_model->polygonFromIndex(_model->index(polygonRow, 0)));
    polygon.translate(Vector2d(newX-oldX, newY-oldY));
    _model->replacePolygon(polygonRow, polygon);

    emitUpdate(0);
  }
}

void LevelDesignerController::addVertex(int polygonRow, int vertexRow, const Point2d& vertex) {
  QUndoCommand* addVertexCommand = new AddVertexCommand(_model, polygonRow, vertexRow, vertex, polygonRow, vertexRow);
  _undoStack->push(addVertexCommand);
}

void LevelDesignerController::removeVertex(int polygonRow, int vertexRow, const Point2d& vertex) {
  int newVertexRow = vertexRow;
  if (_model->rowCount(_model->index(polygonRow, 0)) == 0) {
    newVertexRow = -1;
  } else {
    if (vertexRow == _model->rowCount(_model->index(polygonRow, 0)) - 1) {
      newVertexRow = vertexRow-1;
    }
  }

  QUndoCommand* removeVertexCommand = new RemoveVertexCommand(_model, polygonRow, vertexRow, vertex, polygonRow, newVertexRow);
  _undoStack->push(removeVertexCommand);
}

void LevelDesignerController::moveVertex(int polygonRow, int vertexRow, int oldX, int oldY, int newX, int newY, bool pushToStack) {
  if (pushToStack) {
    QUndoCommand* moverVertexCommand = new MoveVertexCommand(_model, polygonRow, vertexRow, oldX, oldY, newX, newY, polygonRow, vertexRow);
    _undoStack->push(moverVertexCommand);
  } else {
    _model->replaceVertex(polygonRow, vertexRow, Point2d(newX, newY));

    emitUpdate(0);
  }
}

void LevelDesignerController::updateLinesCount(int linesCount) {
  _model->setLinesCount(linesCount);
}

void LevelDesignerController::updatePartsCount(int partsCount) {
  _model->setPartsCount(partsCount);
}

void LevelDesignerController::updateMaxGapToWin(int maxGapToWin) {
  _model->setMaxGapToWin(maxGapToWin);
}

void LevelDesignerController::clear(void) {
  _model->clear();
  _undoStack->clear();
  emit updateReset();
  emitUpdate(0);
}

void LevelDesignerController::saveLevel(const QString& fileName) {
  ParserXML parser;
  parser.initFileName(fileName);

  PolygonList polygonList(_model->getPolygonList());
  foreach (const Polygon& polygon, polygonList) {
    parser.addPolygon(polygon);
  }

  parser.setLinesCount(_model->getLinesCount());
  parser.setPartsCount(_model->getPartsCount());
  parser.setMaxGapToWin(_model->getMaxGapToWin());
  parser.setStarsCount(0);
  parser.setTolerances(0);

  parser.writeXML();
}

void LevelDesignerController::openLevel(const QString& fileName) {
  clear();

  ParserXML parser(fileName);

  PolygonList polygonList(parser.createPolygonList());

  _model->setPolygonList(polygonList);
  emitUpdate(0);
}

void LevelDesignerController::alignToGrid(void) {
  PolygonList polygonList = _model->getPolygonList();
  for (int k = 0; k < _model->rowCount(); ++k) {
    TreeItem* polygonItem = _model->getItem(_model->index(k, 0));
    std::vector<Point2d> vertices = polygonList.at(k).getVertices();
    for (int i = 0; i < polygonItem->childCount(); ++i) {
      Point2d currVertex = vertices.at(i);

      int oldX = currVertex.getX();
      int oldY = currVertex.getY();

      int extraX = static_cast<int>(oldX)%50;
      int extraY = static_cast<int>(oldY)%50;

      int newX = oldX;
      int newY = oldY;

      if (extraY < 50/3)
        newY = oldY - extraY + 5;
      else if (extraY > 50 - 50/3)
        newY = oldY + (50 - extraY) + 5;

      if (extraX < 50/3)
        newX = oldX - extraX + 5;
      else if (extraX > 50 - 50/3)
        newX = oldX + (50 - extraX) + 5;

      if (oldX != newX || oldY != newY)
        moveVertex(k, i, oldX, oldY, newX, newY);
    }
  }
}

void LevelDesignerController::emitUpdate(int) {
  emit selection();
  emit update();
}
