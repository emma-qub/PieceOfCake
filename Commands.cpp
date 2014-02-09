#include "Commands.h"
#include <QDebug>

AddVertexCommand::AddVertexCommand(PolygonTreeModel* model, int polygonRow, int vertexRow, const Point2d& vertex, QUndoCommand* parent) :
    QUndoCommand(parent),
    _model(model),
    _polygonRow(polygonRow),
    _vertexRow(vertexRow),
    _vertex(vertex) {

    setText("Add new Vertex");
//    setText("Add "+model->data(model->index(vertexRow, 1, model->index(polygonRow, 0))).toString());
}

void AddVertexCommand::undo(void) {
    _model->removeVertex(_polygonRow, _vertexRow, true);
}

void AddVertexCommand::redo(void) {
    _model->insertVertex(_polygonRow, _vertexRow, _vertex, false);
}



RemoveVertexCommand::RemoveVertexCommand(PolygonTreeModel* model, int polygonRow, int vertexRow, const Point2d& vertex, QUndoCommand* parent) :
    QUndoCommand(parent),
    _model(model),
    _polygonRow(polygonRow),
    _vertexRow(vertexRow),
    _vertex(vertex) {

    setText("Remove "+model->data(model->index(vertexRow, 1, model->index(polygonRow, 0))).toString());
}

void RemoveVertexCommand::undo(void) {
    _model->insertVertex(_polygonRow, _vertexRow, _vertex, true);
}

void RemoveVertexCommand::redo(void) {
    _model->removeVertex(_polygonRow, _vertexRow, false);
}



AddPolygonCommand::AddPolygonCommand(PolygonTreeModel* model, int polygonRow, const Polygon& polygon, QUndoCommand* parent) :
    QUndoCommand(parent),
    _model(model),
    _polygonRow(polygonRow),
    _polygon(polygon) {

    setText("Add Polygon"+QString::number(polygonRow));
}

void AddPolygonCommand::undo(void) {
    _model->removePolygon(_polygonRow, true);
}

void AddPolygonCommand::redo(void) {
    _model->insertPolygon(_polygonRow, _polygon, false);
}



RemovePolygonCommand::RemovePolygonCommand(PolygonTreeModel* model, int polygonRow, const Polygon& polygon, QUndoCommand* parent) :
    QUndoCommand(parent),
    _model(model),
    _polygonRow(polygonRow),
    _polygon(polygon) {

    setText("Remove Polygon"+QString::number(polygonRow));
}

void RemovePolygonCommand::undo(void) {
    _model->insertPolygon(_polygonRow, _polygon, true);
}

void RemovePolygonCommand::redo(void) {
    _model->removePolygon(_polygonRow, false);
}



MovePolygonCommand::MovePolygonCommand(PolygonTreeModel* model, int polygonRow, int oldX, int oldY, int newX, int newY, QUndoCommand* parent) :
    QUndoCommand(parent),
    _model(model),
    _polygonRow(polygonRow),
    _direction(newX-oldX, newY-oldY) {

    setText("Move Polygon"+QString::number(_polygonRow)+" according to vector ("+QString::number(_direction.getX())+";"+QString::number(_direction.getY())+")");
}

void MovePolygonCommand::undo() {
    _model->translatePolygon(_polygonRow, -_direction);
}

void MovePolygonCommand::redo() {
    _model->translatePolygon(_polygonRow, _direction);
}



MoveVertexCommand::MoveVertexCommand(PolygonTreeModel* model, int polygonRow, int vertexRow, int oldX, int oldY, int newX, int newY, QUndoCommand* parent) :
    QUndoCommand(parent),
    _model(model),
    _polygonRow(polygonRow),
    _vertexRow(vertexRow),
    _oldX(oldX),
    _oldY(oldY),
    _newX(newX),
    _newY(newY) {

    if (_newX == -1)
        _newX = _oldX;
    else if (_newY == -1)
        _newY = _oldY;

    setText("Move "+model->data(model->index(vertexRow, 1, model->index(polygonRow, 0))).toString()+" from ("+QString::number(_oldX)+";"+QString::number(_oldY)+") to ("+QString::number(_newX)+";"+QString::number(_newY)+")");
}

void MoveVertexCommand::undo(void) {
    _model->replaceVertex(_polygonRow, _vertexRow, Point2d(_oldX, _oldY));
}

void MoveVertexCommand::redo(void) {
    _model->replaceVertex(_polygonRow, _vertexRow, Point2d(_newX, _newY));
}




AddSelectionCommand::AddSelectionCommand(PolygonTreeModel* model, int polygonRow, int vertexRow, QUndoCommand* parent) :
    QUndoCommand(parent),
    _model(model),
    _polygonRow(polygonRow),
    _vertexRow(vertexRow) {

    setText("("+QString::number(polygonRow)+", "+QString::number(vertexRow)+")");
}

void AddSelectionCommand::undo(void) {
    _model->popSelection();
}

void AddSelectionCommand::redo(void) {
    _model->addSelection(_polygonRow, _vertexRow);
}
