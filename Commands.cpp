#include "Commands.h"
#include <QDebug>

AddVertexCommand::AddVertexCommand(LevelDesignerModel* model, int polygonRow, int vertexRow, const Point2d& vertex, int selectionPolygonRow, int selectionVertexRow, QUndoCommand* parent):
  QUndoCommand(parent),
  _model(model),
  _polygonRow(polygonRow),
  _vertexRow(vertexRow),
  _vertex(vertex),
  _selectionPolygonRow(selectionPolygonRow),
  _selectionVertexRow(selectionVertexRow) {

  setText("Add new Vertex");
}

void AddVertexCommand::undo(void) {
  _model->removeVertex(_polygonRow, _vertexRow);
  _model->popSelection();
}

void AddVertexCommand::redo(void) {
  _model->insertVertex(_polygonRow, _vertexRow, _vertex, false);
  _model->addSelection(_selectionPolygonRow, _selectionVertexRow);
}



RemoveVertexCommand::RemoveVertexCommand(LevelDesignerModel* model, int polygonRow, int vertexRow, const Point2d& vertex, int selectionPolygonRow, int selectionVertexRow, QUndoCommand* parent):
  QUndoCommand(parent),
  _model(model),
  _polygonRow(polygonRow),
  _vertexRow(vertexRow),
  _vertex(vertex),
  _selectionPolygonRow(selectionPolygonRow),
  _selectionVertexRow(selectionVertexRow) {

  setText("Remove "+model->data(model->index(vertexRow, 1, model->index(polygonRow, 0))).toString());
}

void RemoveVertexCommand::undo(void) {
  _model->insertVertex(_polygonRow, _vertexRow, _vertex, false);
  _model->popSelection();
}

void RemoveVertexCommand::redo(void) {
  _model->removeVertex(_polygonRow, _vertexRow);
  _model->addSelection(_selectionPolygonRow, _selectionVertexRow);
}



AddPolygonCommand::AddPolygonCommand(LevelDesignerModel* model, int polygonRow, const poc::Polygon& polygon, int selectionPolygonRow, int selectionVertexRow, QUndoCommand* parent):
  QUndoCommand(parent),
  _model(model),
  _polygonRow(polygonRow),
  _polygon(polygon),
  _selectionPolygonRow(selectionPolygonRow),
  _selectionVertexRow(selectionVertexRow) {

  setText("Add Polygon"+QString::number(polygonRow));
}

void AddPolygonCommand::undo(void) {
  _model->removePolygon(_polygonRow);
  _model->popSelection();
}

void AddPolygonCommand::redo(void) {
  _model->insertPolygon(_polygonRow, _polygon);
  _model->addSelection(_selectionPolygonRow, _selectionVertexRow);
}



RemovePolygonCommand::RemovePolygonCommand(LevelDesignerModel* model, int polygonRow, const poc::Polygon& polygon, int selectionPolygonRow, int selectionVertexRow, QUndoCommand* parent):
  QUndoCommand(parent),
  _model(model),
  _polygonRow(polygonRow),
  _polygon(polygon),
  _selectionPolygonRow(selectionPolygonRow),
  _selectionVertexRow(selectionVertexRow) {

  setText("Remove Polygon"+QString::number(polygonRow));
}

void RemovePolygonCommand::undo(void) {
  _model->insertPolygon(_polygonRow, _polygon);
  _model->popSelection();
}

void RemovePolygonCommand::redo(void) {
  _model->removePolygon(_polygonRow);
  _model->addSelection(_selectionPolygonRow, _selectionVertexRow);
}



MovePolygonCommand::MovePolygonCommand(LevelDesignerModel* model, int polygonRow, int oldX, int oldY, int newX, int newY, int selectionPolygonRow, int selectionVertexRow, QUndoCommand* parent):
  QUndoCommand(parent),
  _model(model),
  _polygonRow(polygonRow),
  _direction(newX-oldX, newY-oldY),
  _selectionPolygonRow(selectionPolygonRow),
  _selectionVertexRow(selectionVertexRow) {

  setText("Move Polygon"+QString::number(_polygonRow)+" according to vector ("+QString::number(_direction.getX())+";"+QString::number(_direction.getY())+")");
}

void MovePolygonCommand::undo() {
  _model->translatePolygon(_polygonRow, -_direction);
  _model->popSelection();
}

void MovePolygonCommand::redo() {
  _model->translatePolygon(_polygonRow, _direction);
  _model->addSelection(_selectionPolygonRow, _selectionVertexRow);
}



MoveVertexCommand::MoveVertexCommand(LevelDesignerModel* model, int polygonRow, int vertexRow, int oldX, int oldY, int newX, int newY, int selectionPolygonRow, int selectionVertexRow, QUndoCommand* parent):
  QUndoCommand(parent),
  _model(model),
  _polygonRow(polygonRow),
  _vertexRow(vertexRow),
  _oldX(oldX),
  _oldY(oldY),
  _newX(newX),
  _newY(newY),
  _selectionPolygonRow(selectionPolygonRow),
  _selectionVertexRow(selectionVertexRow) {

  if (_newX == -1)
    _newX = _oldX;
  else if (_newY == -1)
    _newY = _oldY;

  setText("Move "+model->data(model->index(vertexRow, 1, model->index(polygonRow, 0))).toString()+" from ("+QString::number(_oldX)+";"+QString::number(_oldY)+") to ("+QString::number(_newX)+";"+QString::number(_newY)+")");
}

void MoveVertexCommand::undo(void) {
  _model->replaceVertex(_polygonRow, _vertexRow, Point2d(_oldX, _oldY));
  _model->popSelection();
}

void MoveVertexCommand::redo(void) {
  _model->replaceVertex(_polygonRow, _vertexRow, Point2d(_newX, _newY));
  _model->addSelection(_polygonRow, _vertexRow);
}




UpdateLinesCountCommand::UpdateLinesCountCommand(LevelDesignerModel* model, int oldLinesCount, int newLinesCount, QUndoCommand* parent):
  QUndoCommand(parent),
  _model(model),
  _oldLinesCount(oldLinesCount),
  _newLinesCount(newLinesCount) {

  setText("Update lines count: "+QString::number(_oldLinesCount)+" -> "+QString::number(_newLinesCount));
}

void UpdateLinesCountCommand::undo(void) {
  _model->setLinesCount(_oldLinesCount);
}

void UpdateLinesCountCommand::redo(void) {
  _model->setLinesCount(_newLinesCount);
}




UpdatePartsCountCommand::UpdatePartsCountCommand(LevelDesignerModel* model, int oldPartsCount, int newPartsCount, QUndoCommand* parent):
  QUndoCommand(parent),
  _model(model),
  _oldPartsCount(oldPartsCount),
  _newPartsCount(newPartsCount) {

  setText("Update parts count: "+QString::number(_oldPartsCount)+" -> "+QString::number(_newPartsCount));
}

void UpdatePartsCountCommand::undo(void) {
  _model->setPartsCount(_oldPartsCount);
}

void UpdatePartsCountCommand::redo(void) {
  _model->setPartsCount(_newPartsCount);
}




UpdateMaxGapToWinCommand::UpdateMaxGapToWinCommand(LevelDesignerModel* model, int oldMaxGapToWin, int newMaxGapToWin, QUndoCommand* parent):
  QUndoCommand(parent),
  _model(model),
  _oldMaxGapToWin(oldMaxGapToWin),
  _newMaxGapToWin(newMaxGapToWin) {

  setText("Update difficulty: "+QString::number(_oldMaxGapToWin)+" -> "+QString::number(_newMaxGapToWin));
}

void UpdateMaxGapToWinCommand::undo(void) {
  _model->setMaxGapToWin(_oldMaxGapToWin);
}

void UpdateMaxGapToWinCommand::redo(void) {
  _model->setMaxGapToWin(_newMaxGapToWin);
}
