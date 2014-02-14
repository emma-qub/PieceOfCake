#include "GameModel.h"

GameModel::GameModel(QObject* parent) :
  QStandardItemModel(parent) {
}

void GameModel::setPolygonList(const PolygonList& polygonList) {
  // Remove every vertex from polygon list
  _polygonList.clear();

  // Fill polygon list with new polygons
  _polygonList = polygonList;

  // Populate model with new polygons
  populate();
}

void GameModel::populate(void) {
  // Remove every polygon from model
  clear();

  // Add every polygon to model
  for (const Polygon& polygon: _polygonList) {
    insertPolygon(rowCount(), polygon);
  }
}

void GameModel::insertPolygon(int polygonRow, const Polygon& polygon) {
  // Get polygon index
  QModelIndex polygonIndex = index(polygonRow, 0);

  // Add every vertex
  std::vector<Point2d> vertices = polygon.getVertices();
  for (const Point2d& vertex: vertices) {
    insertVertex(polygonRow, rowCount(polygonIndex), vertex);
  }
}

void GameModel::insertVertex(int polygonRow, int vertexRow, const Point2d& vertex) {
  // Get polygon index
  QModelIndex polygonIndex = index(polygonRow, 0);

  // Insert a row to store vertex
  insertRow(vertexRow, polygonIndex);

  // Get vertex index
  QModelIndex vertexIndex = index(vertexRow, 0, polygonIndex);

  // Insert two rows to store x and y coordinates
  insertRows(0, 2, vertexIndex);

  // Insert cooridnates
  setData(index(0, 0, vertexIndex), QString::number(vertex.getX()));
  setData(index(1, 0, vertexIndex), QString::number(vertex.getY()));
}
