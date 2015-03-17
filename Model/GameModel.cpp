#include "GameModel.h"
#include <QDebug>

#define cerro(x) std::cerr << x << std::endl;

GameModel::GameModel(QObject* parent):
  QStandardItemModel(parent),
  _polygonsCount(-1),
  _tapesCount(-1),
  _mirrorsCount(-1) {

  int r = std::rand()%255;
  int g = std::rand()%255;
  int b = std::rand()%255;
  _color = QColor(r, g, b);

  _polygonsItem = new QStandardItem("Polygons");
  appendRow(_polygonsItem);

  _tapesItem = new QStandardItem("Tapes");
  appendRow(_tapesItem);

  _mirrorsItem = new QStandardItem("Mirrors");
  appendRow(_mirrorsItem);
}

void GameModel::setPolygonList(const PolygonList& polygonList) {
  // Remove every polygon from polygon list
  _polygonList.clear();

  // Fill polygon list with new polygons
  _polygonList = polygonList;

  // Update polygons count
  _polygonsCount = _polygonList.size();

  // Populate model with new polygons
  populate();
}

void GameModel::setTapeList(const TapeList& tapeList) {
  // Remove every tape from tape list
  _tapeList.clear();

  // Fill tape list with new tapes
  _tapeList = tapeList;

  // Update tapes count
  _tapesCount = _tapeList.size();

  // Populate model with new tapes
  populate();
}

void GameModel::setMirrorList(const MirrorList& mirrorList) {
  // Remove every mirror from mirror list
  _mirrorList.clear();

  // Fill mirror list with new mirrors
  _mirrorList = mirrorList;

  // Update mirrors count
  _mirrorsCount = _mirrorList.size();

  // Populate model with new mirrors
  populate();
}

void GameModel::populate(void) {
  clear();

  _polygonsItem = new QStandardItem("Polygons");
  appendRow(_polygonsItem);

  _tapesItem = new QStandardItem("Tapes");
  appendRow(_tapesItem);

  _mirrorsItem = new QStandardItem("Mirrors");
  appendRow(_mirrorsItem);

  // Add every polygon to model
  for (const Polygon& polygon: _polygonList) {
    appendPolygon(polygon);
  }

  // Add every tape to model
  for (const Tape& tape: _tapeList) {
    appendTape(tape);
  }

  // Add every mirror to model
  for (const Mirror& mirror: _mirrorList) {
    appendMirror(mirror);
  }
}

void GameModel::insertPolygon(int polygonRow, const Polygon& polygon) {
  QStandardItem* polygonItem = new QStandardItem("Polygon"+QString::number(polygonRow));

//  polygonItem->setData(_color, Qt::DecorationRole);
  _polygonsItem->insertRow(polygonRow, polygonItem);

  // Add every vertex
  std::vector<Point2d> vertices = polygon.getVertices();
  for (const Point2d& vertex: vertices) {
    insertVertex(polygonRow, polygonItem->rowCount(), vertex);
  }
}

void GameModel::appendPolygon(const Polygon& polygon) {
  insertPolygon(_polygonsItem->rowCount(), polygon);
}

void GameModel::insertVertex(int polygonRow, int vertexRow, const Point2d& vertex) {
  // Insert a row to store vertex
  QStandardItem* vertexItem = new QStandardItem("Vertex"+QString::number(vertexRow));
  _polygonsItem->child(polygonRow, 0)->insertRow(vertexRow, vertexItem);

  // Insert two rows to store x and y coordinates
  QStandardItem* xItem = new QStandardItem(QString::number(vertex.getX()));
  QStandardItem* yItem = new QStandardItem(QString::number(vertex.getY()));
  vertexItem->insertRows(0, QList<QStandardItem*>() << xItem << yItem);
}

void GameModel::insertTape(int tapeRow, const Tape& tape) {
  // Insert a row to store tape
  QStandardItem* tapeItem = new QStandardItem("Tape"+QString::number(tapeRow));
  _tapesItem->insertRow(tapeRow, tapeItem);

  // Insert four rows to store x and y coordinates and width and height
  QStandardItem* xItem = new QStandardItem(QString::number(tape.getX()));
  QStandardItem* yItem = new QStandardItem(QString::number(tape.getY()));
  QStandardItem* wItem = new QStandardItem(QString::number(tape.getW()));
  QStandardItem* hItem = new QStandardItem(QString::number(tape.getH()));
  tapeItem->insertRows(0, QList<QStandardItem*>() << xItem << yItem << wItem << hItem);
}

void GameModel::appendTape(const Tape& tape) {
  insertTape(_tapesItem->rowCount(), tape);
}

void GameModel::insertMirror(int mirrorRow, const Mirror& mirror) {
  // Insert a row to store mirror
  QStandardItem* mirrorItem = new QStandardItem("Mirror"+QString::number(mirrorRow));
  _tapesItem->insertRow(mirrorRow, mirrorItem);

  // Insert four rows to store x and y coordinates and width and height
  Segment mirrorLine = mirror.getMirrorLine();
  QStandardItem* xaItem = new QStandardItem(QString::number(mirrorLine.getA().getX()));
  QStandardItem* yaItem = new QStandardItem(QString::number(mirrorLine.getA().getY()));
  QStandardItem* xbItem = new QStandardItem(QString::number(mirrorLine.getB().getX()));
  QStandardItem* ybItem = new QStandardItem(QString::number(mirrorLine.getB().getY()));
  mirrorItem->insertRows(0, QList<QStandardItem*>() << xaItem << yaItem << xbItem << ybItem);
}

void GameModel::appendMirror(const Mirror& mirror) {
  insertMirror(_mirrorsItem->rowCount(), mirror);
}

void GameModel::clearPolygons(void) {
  QStandardItemModel::clear();
  _polygonList.clear();
  _polygonsCount = 0;
}

void GameModel::clearTapes(void) {
  QStandardItemModel::clear();
  _tapeList.clear();
  _tapesCount = 0;
}

void GameModel::clearMirrors(void) {
  QStandardItemModel::clear();
  _mirrorList.clear();
  _mirrorsCount = 0;
}
