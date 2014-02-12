#include "PolygonController.h"
#include "ParserXML.h"

#include <QDebug>

PolygonController::PolygonController(PolygonTreeModel *model, QUndoStack* undoStack, QObject* parent) :
    QObject(parent),
    _model(model),
    _undoStack(undoStack) {

    connect(_undoStack, SIGNAL(indexChanged(int)), this, SLOT(emitUpdate(int)));
}

void PolygonController::addPolygon(int polygonRow, const Polygon& polygon) {
    QUndoCommand* addPolygonCommand = new AddPolygonCommand(_model, polygonRow, polygon, polygonRow, -1);
    _undoStack->push(addPolygonCommand);
}

void PolygonController::removePolygon(int polygonRow, const Polygon& polygon) {
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

void PolygonController::movePolygon(int polygonRow, int oldX, int oldY, int newX, int newY, bool pushToStack) {
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

void PolygonController::addVertex(int polygonRow, int vertexRow, const Point2d& vertex) {
    QUndoCommand* addVertexCommand = new AddVertexCommand(_model, polygonRow, vertexRow, vertex, polygonRow, vertexRow);
    _undoStack->push(addVertexCommand);
}

void PolygonController::removeVertex(int polygonRow, int vertexRow, const Point2d& vertex) {
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

void PolygonController::moveVertex(int polygonRow, int vertexRow, int oldX, int oldY, int newX, int newY, bool pushToStack) {
    if (pushToStack) {
        QUndoCommand* moverVertexCommand = new MoveVertexCommand(_model, polygonRow, vertexRow, oldX, oldY, newX, newY, polygonRow, vertexRow);
        _undoStack->push(moverVertexCommand);
    } else {
        _model->replaceVertex(polygonRow, vertexRow, Point2d(newX, newY));

        emitUpdate(0);
    }
}

void PolygonController::clear(void) {
    _model->clear();
    _undoStack->clear();
    _selectionsStack->clear();
    emit updateReset();
    emitUpdate(0);
}

void PolygonController::saveLevel(const QString& fileName) {
    ParserXML parser;
    parser.initFileName(fileName);

    PolygonList polygonList(_model->getPolygonList());
    foreach (const Polygon& polygon, polygonList) {
        parser.addPolygon(polygon);
    }
    parser.writeXML();
}

void PolygonController::openLevel(const QString& fileName) {
    clear();

    ParserXML parser(fileName);

    PolygonList polygonList(parser.createPolygonList());

    _model->setPolygonList(polygonList);
}

void PolygonController::alignToGrid(void) {
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

void PolygonController::emitUpdate(int) {
    emit selection();
    emit update();
}
