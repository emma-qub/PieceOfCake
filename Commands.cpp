#include "Commands.h"
#include <QDebug>

AddVertexCommand::AddVertexCommand(PolygonTreeModel* model, int polygonRow, int vertexRow, const Point2d& vertex, int selectionPolygonRow, int selectionVertexRow, QUndoCommand* parent) :
    QUndoCommand(parent),
    _model(model),
    _polygonRow(polygonRow),
    _vertexRow(vertexRow),
    _vertex(vertex),
    _selectionPolygonRow(selectionPolygonRow),
    _selectionVertexRow(selectionVertexRow) {

    setText("Add new Vertex");
//    setText("Add "+model->data(model->index(vertexRow, 1, model->index(polygonRow, 0))).toString());
}

void AddVertexCommand::undo(void) {
    _model->removeVertex(_polygonRow, _vertexRow, true);
    _model->popSelection();
}

void AddVertexCommand::redo(void) {
    _model->insertVertex(_polygonRow, _vertexRow, _vertex, false);
    _model->addSelection(_selectionPolygonRow, _selectionVertexRow);
}



RemoveVertexCommand::RemoveVertexCommand(PolygonTreeModel* model, int polygonRow, int vertexRow, const Point2d& vertex, int selectionPolygonRow, int selectionVertexRow, QUndoCommand* parent) :
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
    _model->insertVertex(_polygonRow, _vertexRow, _vertex, true);
    _model->popSelection();
}

void RemoveVertexCommand::redo(void) {
    _model->removeVertex(_polygonRow, _vertexRow, false);
    _model->addSelection(_selectionPolygonRow, _selectionVertexRow);
}



AddPolygonCommand::AddPolygonCommand(PolygonTreeModel* model, int polygonRow, const Polygon& polygon, int selectionPolygonRow, int selectionVertexRow, QUndoCommand* parent) :
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



RemovePolygonCommand::RemovePolygonCommand(PolygonTreeModel* model, int polygonRow, const Polygon& polygon, int selectionPolygonRow, int selectionVertexRow, QUndoCommand* parent) :
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



MovePolygonCommand::MovePolygonCommand(PolygonTreeModel* model, int polygonRow, int oldX, int oldY, int newX, int newY, int selectionPolygonRow, int selectionVertexRow, QUndoCommand* parent) :
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



MoveVertexCommand::MoveVertexCommand(PolygonTreeModel* model, int polygonRow, int vertexRow, int oldX, int oldY, int newX, int newY, int selectionPolygonRow, int selectionVertexRow, QUndoCommand* parent) :
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
