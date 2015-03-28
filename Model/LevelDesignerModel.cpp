#include "LevelDesignerModel.h"

#include <QDebug>
#include <QColor>

LevelDesignerModel::LevelDesignerModel(const QStringList& headers, const QString& data, QObject *parent):
  TreeModel(headers, data, parent),
  _polygonList(),
  _selections(),
  _partsCount(2),
  _linesCount(1),
  _maxGapToWin(10) {

  _selections << QPair<int, int>(-1, -1);

  insertColumns(0, 4);
  setHeaderData(0, Qt::Horizontal, "Polygon");
  setHeaderData(1, Qt::Horizontal, "Vertex");
  setHeaderData(2, Qt::Horizontal, "X");
  setHeaderData(3, Qt::Horizontal, "Y");

  connect(this, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(debug(QModelIndex,QModelIndex)));
}

LevelDesignerModel::~LevelDesignerModel(void) {
}

void LevelDesignerModel::setPolygonList(const PolygonList& polygonList) {
  for (const Polygon& polygon: polygonList) {
    appendPolygon(polygon);
  }
}

void LevelDesignerModel::clear(void) {
  removeRows(0, rowCount());
  _polygonList.clear();
}

bool LevelDesignerModel::polygonHasVertex(int polygonRow) {
  return (_polygonList.at(polygonRow).getVertices().size() > 1 && hasChildren(index(polygonRow, 0)));
}

Polygon LevelDesignerModel::polygonFromIndex(const QModelIndex& polygonIndex) {
  if (_polygonList.size() > polygonIndex.row())
    return _polygonList.at(polygonIndex.row());
  else
    return Polygon();
}

Point2d LevelDesignerModel::vertexFromIndex(const QModelIndex& vertexIndex) {
  std::vector<Point2d> vertices = polygonFromIndex(vertexIndex.parent()).getVertices();
  if (vertices.size() > static_cast<unsigned int>(vertexIndex.row()))
    return vertices.at(vertexIndex.row());
  else
    return Point2d();
}

bool LevelDesignerModel::appendPolygon(const Polygon& polygon) {
  return insertPolygon(rowCount(), polygon);
}

bool LevelDesignerModel::insertPolygon(int polygonRow, const Polygon& polygon) {
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
    insertVertex(polygonRow, k, vertex, true);
  }

  _polygonList.insert(polygonRow, polygon);

  updatePolygons();

  return true;
}

bool LevelDesignerModel::removePolygon(int polygonRow) {
  _polygonList.removeAt(polygonRow);

  bool success = removeRow(polygonRow);
  if (success)
    updatePolygons();

  return success;
}

bool LevelDesignerModel::replacePolygon(int polygonRow, const Polygon& polygon) {
  bool res = true;

  std::vector<Point2d> vertices = polygon.getVertices();
  for (unsigned int k = 0; k < vertices.size(); ++k) {
    res = res && replaceVertex(polygonRow, k, vertices.at(k));
  }

  _polygonList.replace(polygonRow, polygon);

  return res;
}

bool LevelDesignerModel::translatePolygon(int polygonRow, const Vector2d& direction) {
  Polygon polygon(_polygonList.at(polygonRow));
  polygon.translate(direction);

  bool res = true;

  std::vector<Point2d> vertices = polygon.getVertices();
  for (unsigned int k = 0; k < polygon.getVertices().size(); ++k) {
    res = res && replaceVertex(polygonRow, k, vertices.at(k));
  }

  return res;
}



bool LevelDesignerModel::insertVertex(int polygonRow, int vertexRow, const Point2d& vertex, bool exist) {
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

  return true;
}

bool LevelDesignerModel::removeVertex(int polygonRow, int vertexRow) {
  QModelIndex polygonIndex = index(polygonRow, 0);
  bool result = removeRow(vertexRow, polygonIndex);

  if (rowCount(polygonIndex) > 0)
    updateVertices(polygonIndex);

  _polygonList[polygonRow].removeVertex(vertexRow);

  return result;
}

bool LevelDesignerModel::replaceVertex(int polygonRow, int vertexRow, const Point2d& vertex) {
  return removeVertex(polygonRow, vertexRow) && insertVertex(polygonRow, vertexRow, vertex);
}

void LevelDesignerModel::updatePolygons(void) {
  for (int k = 0; k < rowCount(); k++) {
    setData(index(k, 0), "Polygon "+QString::number(k));
    updateVertices(index(k, 0));
  }
}

void LevelDesignerModel::updateVertices(QModelIndex polygonIndex) {
  for (int k = 0; k < rowCount(polygonIndex); k++) {
    setData(index(k, 1, polygonIndex), QChar(65+k)+QString::number(polygonIndex.row()));
  }
}

void LevelDesignerModel::debug(QModelIndex,QModelIndex) {
  qDebug() << "><";
}
