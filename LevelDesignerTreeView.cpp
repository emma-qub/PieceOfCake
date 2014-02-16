#include "LevelDesignerTreeView.h"
#include "LevelDesignerTreeItemDelegate.h"
#include "LevelDesignerModel.h"
#include "Commands.h"

LevelDesignerTreeView::LevelDesignerTreeView(LevelDesignerController* controller, QWidget* parent) :
    QTreeView(parent),
    _controller(controller),
    _oldX(-1),
    _oldY(-1) {

    _delegate = new LevelDesignerTreeItemDelegate;
    setItemDelegate(_delegate);

    setStyleSheet("QTreeView:branch { border-image: url(none.png); }");
    setItemsExpandable(false);

    connect(_delegate, SIGNAL(moveVertex(QModelIndex,int,bool)), this, SLOT(movingVertex(QModelIndex,int,bool)));
    connect(_controller, SIGNAL(update(void)), this, SLOT(expandAll(void)));
    connect(_controller, SIGNAL(selection(void)), this, SLOT(selection(void)));

    setFixedWidth(400);
}

void LevelDesignerTreeView::setModel(LevelDesignerModel* model) {
    QTreeView::setModel(model);
    _model = model;

    setColumnWidth(0, 120);
    for (int k = 1; k < 4; k++)
        setColumnWidth(k, 80);

    expandAll();

    connect(_model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(debugDataChanged(QModelIndex,QModelIndex)));
}

void LevelDesignerTreeView::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Delete) {

        QModelIndex polygonIndex;
        QModelIndex vertexIndex;

        QModelIndex currIndex = currentIndex();
        if (!currIndex.isValid()) {
            qDebug() << "Error within LevelDesignerTreeView::keyPressEvent: index is not valid";
            return;
        }

        if (currIndex.column() != 0)
            currIndex = _model->index(currIndex.row(), 0, currIndex.parent());

        QString fstData = _model->itemData(currIndex).value(Qt::DisplayRole).toString();

        // This is a vertex
        if (!fstData.startsWith("Polygon")) {
            vertexIndex = currIndex;
            polygonIndex = vertexIndex.parent();
            _controller->removeVertex(polygonIndex.row(), vertexIndex.row(), _model->vertexFromIndex(vertexIndex));
        // This is the header
        } else if (fstData == "Polygon") {
           return;
        // This is a polygon
        } else if (fstData.startsWith("Polygon ")) {
            _controller->removePolygon(currIndex.row(), _model->polygonFromIndex(currIndex));
        } else {
            qDebug() << "Error within LevelDesignerTreeView::keyPressEvent: cannot know whether it's a polygon or a vertex";
            return;
        }
    }
}

void LevelDesignerTreeView::movingVertex(QModelIndex vertexIndex, int value, bool editModel) {
    int column = vertexIndex.column();
    TreeItem* currVertex = _model->getItem(vertexIndex);

//    for (int k = 0; k < 4; k++)
//        qDebug() << currVertex->data(k);

    int newX = -1;
    int newY = -1;

    if (column == 2) {
        newX = value;
        newY = _oldY;
    } else if (column == 3) {
        newX = _oldX;
        newY = value;
    } else
        qDebug() << "Error: cannot edit this column within LevelDesignerTreeView::movingVertex";

    if (editModel) {
        _controller->moveVertex(vertexIndex.parent().row(), vertexIndex.row(), _oldX, _oldY, newX, newY);
        _oldX = -1;
        _oldY = -1;
    } else {
        if (_oldX == -1 || _oldY == -1) {
            _oldX = currVertex->data(2).toInt();
            _oldY = currVertex->data(3).toInt();
        }
        emit updateViewNotModel(vertexIndex, value);
    }
}

void LevelDesignerTreeView::debugDataChanged(QModelIndex, QModelIndex) {
    qDebug() << "debugDataChanged";
}

void LevelDesignerTreeView::updateAll(void) {
    collapseAll();
    expandAll();
}

void LevelDesignerTreeView::selection(void) {
    QStack<QPair<int, int>> selections = _model->getSelections();

    if (selections.isEmpty()) {
        qDebug() << "No no no, it can't be.";
        return;
    }

    int polygonRow = selections.last().first;
    int vertexRow = selections.last().second;

    qDebug() << "###" << selections.size() << polygonRow << vertexRow;

    QModelIndex currIndex;
    if (polygonRow == -1 && vertexRow != -1) {
        qDebug() << "Impossiburu";
        return;
    }
    else if (polygonRow == -1 && vertexRow == -1) {
        qDebug() << "No polygon here.";
        return;
    } else if (polygonRow != -1 && vertexRow == -1) {
        currIndex = _model->index(polygonRow, 0);
    } else {
        currIndex = _model->index(vertexRow, 0, _model->index(polygonRow, 0));
    }

    if (currIndex.isValid()) {
        selectionModel()->setCurrentIndex(currIndex, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    } else {
        qDebug() << "currIndex is still not valid!";
    }
}
