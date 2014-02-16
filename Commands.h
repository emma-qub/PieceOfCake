#ifndef COMMANDS_H
#define COMMANDS_H

#include <QUndoCommand>
#include <QWidget>

#include "LevelDesignerModel.h"
#include "Vector2d.h"

class AddVertexCommand : public QUndoCommand {
public:
    AddVertexCommand(LevelDesignerModel* model, int polygonRow, int vertexRow, const Point2d& vertex, int selectionPolygonRow, int selectionVertexRow, QUndoCommand* parent = NULL);

    virtual void undo(void);
    virtual void redo(void);

private:
    LevelDesignerModel* _model;
    int _polygonRow;
    int _vertexRow;
    Point2d _vertex;
    int _selectionPolygonRow;
    int _selectionVertexRow;
};



class RemoveVertexCommand : public QUndoCommand {
public:
    RemoveVertexCommand(LevelDesignerModel* model, int polygonRow, int vertexRow, const Point2d& vertex, int selectionPolygonRow, int selectionVertexRow, QUndoCommand* parent = NULL);

    virtual void undo(void);
    virtual void redo(void);

private:
    LevelDesignerModel* _model;
    int _polygonRow;
    int _vertexRow;
    Point2d _vertex;
    int _selectionPolygonRow;
    int _selectionVertexRow;
};



class AddPolygonCommand : public QUndoCommand {
public:
    AddPolygonCommand(LevelDesignerModel* model, int polygonRow, const Polygon& polygon, int selectionPolygonRow, int selectionVertexRow, QUndoCommand* parent = NULL);

    virtual void undo(void);
    virtual void redo(void);

private:
    LevelDesignerModel* _model;
    int _polygonRow;
    Polygon _polygon;
    int _selectionPolygonRow;
    int _selectionVertexRow;
};



class RemovePolygonCommand : public QUndoCommand {
public:
    RemovePolygonCommand(LevelDesignerModel* model, int polygonRow, const Polygon& polygon, int selectionPolygonRow, int selectionVertexRow, QUndoCommand* parent = NULL);

    virtual void undo(void);
    virtual void redo(void);

private:
    LevelDesignerModel* _model;
    int _polygonRow;
    Polygon _polygon;
    int _selectionPolygonRow;
    int _selectionVertexRow;
};



class MovePolygonCommand : public QUndoCommand {
public:
    MovePolygonCommand(LevelDesignerModel* model, int polygonRow, int oldX, int oldY, int newX, int newY, int selectionPolygonRow, int selectionVertexRow, QUndoCommand* parent = NULL);

    virtual void undo(void);
    virtual void redo(void);

private:
    LevelDesignerModel* _model;
    int _polygonRow;
    Vector2d _direction;
    int _selectionPolygonRow;
    int _selectionVertexRow;
};



class MoveVertexCommand : public QUndoCommand {
public:
    MoveVertexCommand(LevelDesignerModel* model, int polygonRow, int vertexRow, int oldX, int oldY, int newX, int newY, int selectionPolygonRow, int selectionVertexRow, QUndoCommand* parent = NULL);

    virtual void undo(void);
    virtual void redo(void);

private:
    LevelDesignerModel* _model;
    int _polygonRow;
    int _vertexRow;
    int _oldX;
    int _oldY;
    int _newX;
    int _newY;
    int _selectionPolygonRow;
    int _selectionVertexRow;
};

#endif // COMMANDS_H
