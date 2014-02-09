#include "PolygonTreeModel.h"

#include <QDebug>
#include <QColor>

PolygonTreeModel::PolygonTreeModel(const QStringList& headers, const QString& data, QObject *parent) :
    TreeModel(headers, data, parent),
    _polygonList(),
    _selections() {

    _selections << QPair<int, int>(-1, -1);

    insertColumns(0, 4);
    setHeaderData(0, Qt::Horizontal, "Polygon");
    setHeaderData(1, Qt::Horizontal, "Vertex");
    setHeaderData(2, Qt::Horizontal, "X");
    setHeaderData(3, Qt::Horizontal, "Y");

    connect(this, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(debug(QModelIndex,QModelIndex)));
}

PolygonTreeModel::~PolygonTreeModel(void) {
}

void PolygonTreeModel::setPolygonList(const PolygonList& polygonList) {
    foreach (const Polygon& polygon, polygonList) {
        appendPolygon(polygon);
    }
}

void PolygonTreeModel::clear(void) {
    removeRows(0, rowCount());
    _polygonList.clear();
}

bool PolygonTreeModel::polygonHasVertex(int polygonRow) {
    return (_polygonList.at(polygonRow).getVertices().size() > 1 && hasChildren(index(polygonRow, 0)));
}

Polygon PolygonTreeModel::polygonFromIndex(const QModelIndex& polygonIndex) {
    if (_polygonList.size() > polygonIndex.row())
        return _polygonList.at(polygonIndex.row());
    else
        return Polygon();
}

Point2d PolygonTreeModel::vertexFromIndex(const QModelIndex& vertexIndex) {
    std::vector<Point2d> vertices = polygonFromIndex(vertexIndex.parent()).getVertices();
    if (vertices.size() > static_cast<unsigned int>(vertexIndex.row()))
        return vertices.at(vertexIndex.row());
    else
        return Point2d();
}

bool PolygonTreeModel::appendPolygon(const Polygon& polygon) {
    return insertPolygon(rowCount(), polygon);
}

bool PolygonTreeModel::insertPolygon(int polygonRow, const Polygon& polygon, bool fromUndo) {
    std::vector<Point2d> vertices = polygon.getVertices();

    int r = std::rand()%255;
    int g = std::rand()%255;
    int b = std::rand()%255;
    QColor randomColor(r, g, b);

    insertRows(polygonRow, 1);
    QModelIndex polygonIndex = index(polygonRow, 0);

    setData(polygonIndex, "Polygon "+QString::number(polygonRow));
    setData(polygonIndex, randomColor, Qt::DecorationRole);

    for (unsigned int k = 0; k < vertices.size(); k++) {
        Point2d vertex = vertices.at(k);
        insertVertex(polygonRow, k, vertex, fromUndo, true);
    }

    _polygonList.insert(polygonRow, polygon);

    updatePolygons();

    if (fromUndo) {
        _selections.pop();
        int newPolygonRow = _selections.last().first;
        for (unsigned int k = 0; k < vertices.size(); k++) {
            _selections << QPair<int, int>(newPolygonRow, k);
        }
//        _selections << QPair<int, int>(polygonRow, k);
    } else {
        _selections << QPair<int, int>(polygonRow, -1);
    }

    return true;
}

bool PolygonTreeModel::removePolygon(int polygonRow, bool fromUndo) {
    if (fromUndo) {
        _selections.pop();
    } else {
        if (rowCount() == 0) {
            _selections << QPair<int, int>(-1, -1);
        } else if (polygonRow == rowCount()-1) {
            _selections << QPair<int, int>(rowCount()-2, -1);
        } else {
            _selections << QPair<int, int>(polygonRow, -1);
        }
    }

    _polygonList.removeAt(polygonRow);

    return removeRow(polygonRow);
}

bool PolygonTreeModel::replacePolygon(int polygonRow, const Polygon& polygon) {
    bool res = true;

    std::vector<Point2d> vertices = polygon.getVertices();
    for (unsigned int k = 0; k < vertices.size(); ++k) {
        res = res && replaceVertex(polygonRow, k, vertices.at(k));
    }

    _polygonList.replace(polygonRow, polygon);

    return res;
}

bool PolygonTreeModel::translatePolygon(int polygonRow, const Vector2d& direction) {
    Polygon polygon(_polygonList.at(polygonRow));
    polygon.translate(direction);

    bool res = true;

    std::vector<Point2d> vertices = polygon.getVertices();
    for (unsigned int k = 0; k < polygon.getVertices().size(); ++k) {
        res = res && replaceVertex(polygonRow, k, vertices.at(k));
    }

    return res;
}



//bool PolygonTreeModel::appendVertex(int polygonRow, const Point2d& vertex) {
//    TreeItem* polygonItem = getItem(index(polygonRow, 0));
//    int vertexRow = polygonItem->childCount();
//    polygonItem->insertChildren(vertexRow, 1, 4);
//    return insertVertex(polygonRow, vertexRow, vertex);
//}

bool PolygonTreeModel::insertVertex(int polygonRow, int vertexRow, const Point2d& vertex, bool fromUndo, bool exist) {
    insertRow(vertexRow, index(polygonRow, 0));

    QModelIndex polygonIndex = index(polygonRow, 0);
    QModelIndex vertexNameIndex = index(vertexRow, 1, polygonIndex);
    QModelIndex vertexXIndex = index(vertexRow, 2, polygonIndex);
    QModelIndex vertexYIndex = index(vertexRow, 3, polygonIndex);

    setData(vertexNameIndex, QChar(65+vertexRow)+QString::number(polygonRow));
    setData(vertexXIndex, vertex.getX());
    setData(vertexYIndex, vertex.getY());

    if (!exist)
        _polygonList[polygonRow].insertVertex(vertex, vertexRow);

    updateVertices(polygonIndex);

    if (fromUndo) {
        _selections.pop();
    } else {
        _selections << QPair<int, int>(polygonRow, vertexRow);
    }

    return true;
}

bool PolygonTreeModel::removeVertex(int polygonRow, int vertexRow, bool fromUndo) {
    QModelIndex polygonIndex = index(polygonRow, 0);
    bool result = removeRow(vertexRow, polygonIndex);

    if (fromUndo) {
        _selections.pop();
    } else {
        if (rowCount(index(polygonRow, 0)) == 0) {
            _selections << QPair<int, int>(polygonRow, -1);
        } else if (vertexRow == rowCount(index(polygonRow, 0))) {
            _selections << QPair<int, int>(polygonRow, rowCount(index(polygonRow, 0))-1);
        } else {
            _selections << QPair<int, int>(polygonRow, vertexRow);
        }
    }

    if (rowCount(polygonIndex) > 0)
        updateVertices(polygonIndex);

    _polygonList[polygonRow].removeVertex(vertexRow);

    return result;
}

bool PolygonTreeModel::replaceVertex(int polygonRow, int vertexRow, const Point2d& vertex) {
    return removeVertex(polygonRow, vertexRow) && insertVertex(polygonRow, vertexRow, vertex);
}

void PolygonTreeModel::updatePolygons(void) {
    for (int k = 0; k < rowCount(); k++) {
        setData(index(k, 0), "Polygon "+QString::number(k));
        updateVertices(index(k, 0));
    }
}

void PolygonTreeModel::updateVertices(QModelIndex polygonIndex) {
    for (int k = 0; k < rowCount(polygonIndex); k++) {
        setData(index(k, 1, polygonIndex), QChar(65+k)+QString::number(polygonIndex.row()));
    }
}

void PolygonTreeModel::debug(QModelIndex,QModelIndex) {
    qDebug() << "><";
}
