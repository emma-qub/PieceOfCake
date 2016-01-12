#include "GameModel.h"
#include <QDebug>

#define cerro(x) std::cerr << x << std::endl;

GameModel::GameModel(QObject* parent):
  QStandardItemModel(parent),
  _polygonsCount(-1),
  _tapesCount(-1),
  _mirrorsCount(-1),
  _portalsCount(-1),
  _refractorsCount(-1),
  _deviationsCount(-1) {

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

  _portalsItem = new QStandardItem("Portals");
  appendRow(_portalsItem);

  _refractorsItem = new QStandardItem("Refractors");
  appendRow(_refractorsItem);
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

void GameModel::setDeviationList(const DeviationList& deviationList) {
  // Remove every deviation from deviation list
  for (Deviation* deviation: _deviationList)
    delete deviation;
  _deviationList.clear();

  // Fill deviation list with new deviations
  _deviationList = deviationList;

  // Update deviation count
  _deviationsCount = _deviationList.size();

  // Populate model with new deviations
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

  _portalsItem = new QStandardItem("Portals");
  appendRow(_portalsItem);

  _refractorsItem = new QStandardItem("Refractors");
  appendRow(_refractorsItem);

  // Add every polygon to model
  for (const poc::Polygon& polygon: _polygonList) {
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

  // Add every portal to model
  for (const Portal& portal: _portalList) {
    appendPortal(portal);
  }

  // Add every refractor to model
  for (const Refractor& refractor: _refractorList) {
    appendRefractor(refractor);
  }
}

void GameModel::insertPolygon(int polygonRow, const poc::Polygon& polygon) {
  QStandardItem* polygonItem = new QStandardItem("poc::Polygon"+QString::number(polygonRow));

//  polygonItem->setData(_color, Qt::DecorationRole);
  _polygonsItem->insertRow(polygonRow, polygonItem);

  // Add every vertex
  std::vector<Point2d> vertices = polygon.getVertices();
  for (const Point2d& vertex: vertices) {
    insertVertex(polygonRow, polygonItem->rowCount(), vertex);
  }
}

void GameModel::appendPolygon(const poc::Polygon& polygon) {
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
  _mirrorsItem->insertRow(mirrorRow, mirrorItem);

  // Insert four rows to store x and y coordinates of mirror's bounds
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

void GameModel::insertPortal(int portalRow, const Portal& portal) {
  // Insert a row to store portal
  QStandardItem* portalItem = new QStandardItem("Portal"+QString::number(portalRow));
  _portalsItem->insertRow(portalRow, portalItem);

  // Insert eight rows to store x and y coordinates of in and out portals bounds
  Segment portalIn = portal.getIn();
  QStandardItem* xaInItem = new QStandardItem(QString::number(portalIn.getA().getX()));
  QStandardItem* yaInItem = new QStandardItem(QString::number(portalIn.getA().getY()));
  QStandardItem* xbInItem = new QStandardItem(QString::number(portalIn.getB().getX()));
  QStandardItem* ybInItem = new QStandardItem(QString::number(portalIn.getB().getY()));

  Segment portalOut = portal.getOut();
  QStandardItem* xaOutItem = new QStandardItem(QString::number(portalOut.getA().getX()));
  QStandardItem* yaOutItem = new QStandardItem(QString::number(portalOut.getA().getY()));
  QStandardItem* xbOutItem = new QStandardItem(QString::number(portalOut.getB().getX()));
  QStandardItem* ybOutItem = new QStandardItem(QString::number(portalOut.getB().getY()));

  portalItem->insertRows(0, QList<QStandardItem*>() << xaInItem << yaInItem << xbInItem << ybInItem << xaOutItem << yaOutItem << xbOutItem << ybOutItem);
}

void GameModel::appendPortal(const Portal& portal) {
  insertPortal(_portalsItem->rowCount(), portal);
}

void GameModel::insertRefractor(int refractorRow, const Refractor& refractor) {
  // Insert a row to store refractor
  QStandardItem* refractorItem = new QStandardItem("Refractor"+QString::number(refractorRow));
  _refractorsItem->insertRow(refractorRow, refractorItem);

  // Insert eight rows to store x and y coordinates of in and out refractors bounds
  Segment refractorLine = refractor.getRefractorLine();
  QStandardItem* xa = new QStandardItem(QString::number(refractorLine.getA().getX()));
  QStandardItem* ya = new QStandardItem(QString::number(refractorLine.getA().getY()));
  QStandardItem* xb = new QStandardItem(QString::number(refractorLine.getB().getX()));
  QStandardItem* yb = new QStandardItem(QString::number(refractorLine.getB().getY()));
  QStandardItem* indice = new QStandardItem(QString::number(refractor.getIndice()));

  refractorItem->insertRows(0, QList<QStandardItem*>() << xa << ya << xb << yb << indice);
}

void GameModel::appendRefractor(const Refractor& refractor) {
  insertRefractor(_refractorsItem->rowCount(), refractor);
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

void GameModel::clearPortals(void) {
  QStandardItemModel::clear();
  _portalList.clear();
  _portalsCount = 0;
}

void GameModel::clearRefractors(void) {
  QStandardItemModel::clear();
  _refractorList.clear();
  _refractorsCount = 0;
}

void GameModel::clearDeviations(void) {
  QStandardItemModel::clear();
  for (Deviation* deviation: _deviationList)
    delete deviation;
  _deviationList.clear();
  _deviationsCount = 0;
}
