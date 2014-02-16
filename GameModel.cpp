#include "GameModel.h"
#include <QDebug>

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
  int r = std::rand()%255;
  int g = std::rand()%255;
  int b = std::rand()%255;
  QColor randomColor(r, g, b);

  QStandardItem* polygonItem = new QStandardItem("Polygon"+QString::number(polygonRow));
  polygonItem->setData(randomColor, Qt::DecorationRole);
  insertRow(polygonRow, polygonItem);

  // Add every vertex
  std::vector<Point2d> vertices = polygon.getVertices();
  for (const Point2d& vertex: vertices) {
    insertVertex(polygonRow, rowCount(index(polygonRow, 0)), vertex);
  }
}

void GameModel::insertVertex(int polygonRow, int vertexRow, const Point2d& vertex) {
  // Insert a row to store vertex
  QStandardItem* vertexItem = new QStandardItem("Vertex"+QString::number(vertexRow));
  item(polygonRow, 0)->insertRow(vertexRow, vertexItem);

  // Insert two rows to store x and y coordinates
  QStandardItem* xItem = new QStandardItem(QString::number(vertex.getX()));
  QStandardItem* yItem = new QStandardItem(QString::number(vertex.getY()));
  vertexItem->insertRows(0, QList<QStandardItem*>() << xItem << yItem);
}
