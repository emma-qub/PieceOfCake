#ifndef COMMANDS_H
#define COMMANDS_H

#include <QUndoCommand>
#include <QWidget>

#include "PolygonTreeModel.h"
#include "Vector2d.h"

class AddVertexCommand : public QUndoCommand {
public:
    AddVertexCommand(PolygonTreeModel* model, int polygonRow, int vertexRow, const Point2d& vertex, QUndoCommand* parent = NULL);

    virtual void undo(void);
    virtual void redo(void);

private:
    PolygonTreeModel* _model;
    int _polygonRow;
    int _vertexRow;
    Point2d _vertex;
};



class RemoveVertexCommand : public QUndoCommand {
public:
    RemoveVertexCommand(PolygonTreeModel* model, int polygonRow, int vertexRow, const Point2d& vertex, QUndoCommand* parent = NULL);

    virtual void undo(void);
    virtual void redo(void);

private:
    PolygonTreeModel* _model;
    int _polygonRow;
    int _vertexRow;
    Point2d _vertex;
};



class AddPolygonCommand : public QUndoCommand {
public:
    AddPolygonCommand(PolygonTreeModel* model, int polygonRow, const Polygon& polygon, QUndoCommand* parent = NULL);

    virtual void undo(void);
    virtual void redo(void);

private:
    PolygonTreeModel* _model;
    int _polygonRow;
    Polygon _polygon;
};



class RemovePolygonCommand : public QUndoCommand {
public:
    RemovePolygonCommand(PolygonTreeModel* model, int polygonRow, const Polygon& polygon, QUndoCommand* parent = NULL);

    virtual void undo(void);
    virtual void redo(void);

private:
    PolygonTreeModel* _model;
    int _polygonRow;
    Polygon _polygon;
};



class MovePolygonCommand : public QUndoCommand {
public:
    MovePolygonCommand(PolygonTreeModel* model, int polygonRow, int oldX, int oldY, int newX, int newY, QUndoCommand* parent = NULL);

    virtual void undo(void);
    virtual void redo(void);

private:
    PolygonTreeModel* _model;
    int _polygonRow;
    Vector2d _direction;
};



class MoveVertexCommand : public QUndoCommand {
public:
    MoveVertexCommand(PolygonTreeModel* model, int polygonRow, int vertexRow, int oldX, int oldY, int newX, int newY, QUndoCommand* parent = NULL);

    virtual void undo(void);
    virtual void redo(void);

private:
    PolygonTreeModel* _model;
    int _polygonRow;
    int _vertexRow;
    int _oldX;
    int _oldY;
    int _newX;
    int _newY;
};



class AddSelectionCommand : public QUndoCommand {
public:
    AddSelectionCommand(PolygonTreeModel* model, int polygonRow, int vertexRow, QUndoCommand* parent = NULL);

    virtual void undo(void);
    virtual void redo(void);

private:
    PolygonTreeModel* _model;
    int _polygonRow;
    int _vertexRow;
};

#endif // COMMANDS_H
