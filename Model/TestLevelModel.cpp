#include "TestLevelModel.h"

#include <QDebug>

TestLevelModel::TestLevelModel(const QStringList& headers, const QString& data, QObject* parent):
  TreeModel(headers, data, parent),
  _lineList(),
  _selections() {

  _selections << QPair<int, int>(-1, -1);

  int r = std::rand()%255;
  int g = std::rand()%255;
  int b = std::rand()%255;
  _randomColor = QColor(r, g, b);

  insertColumns(0, 4);
  setHeaderData(0, Qt::Horizontal, "Line");
  setHeaderData(1, Qt::Horizontal, "Vertex");
  setHeaderData(2, Qt::Horizontal, "X");
  setHeaderData(3, Qt::Horizontal, "Y");
}

TestLevelModel::~TestLevelModel() {
}

void TestLevelModel::setLineList(const LineList& lineList) {
  for (const Segment& line: lineList) {
    appendLine(line);
  }
}

void TestLevelModel::clear(void) {
  removeRows(0, rowCount());
  _lineList.clear();
}

Segment TestLevelModel::lineFromIndex(const QModelIndex& lineIndex) {
  if (_lineList.size() > lineIndex.row())
    return _lineList.at(lineIndex.row());
  else
    return Segment();
}

Point2d TestLevelModel::vertexFromIndex(const QModelIndex& vertexIndex) {
  std::vector<Point2d> vertices = lineFromIndex(vertexIndex.parent()).getVertices();
  if (vertices.size() > static_cast<unsigned int>(vertexIndex.row()))
    return vertices.at(vertexIndex.row());
  else
    return Point2d();
}

bool TestLevelModel::appendLine(const Segment& line) {
  return insertLine(rowCount(), line);
}

bool TestLevelModel::insertLine(int lineRow, const Segment& line) {
  std::vector<Point2d> vertices = line.getVertices();

  insertRows(lineRow, 1);
  QModelIndex lineIndex = index(lineRow, 0);

  setData(lineIndex, "Line "+QString::number(lineRow));
  setData(lineIndex, _randomColor, Qt::DecorationRole);

  for (unsigned int k = 0; k < vertices.size(); k++) {
    Point2d vertex = vertices.at(k);
    insertVertex(lineRow, k, vertex);
  }

  _lineList.insert(lineRow, line);

  updateLines();

  return true;
}

bool TestLevelModel::removeLine(int lineRow) {
  _lineList.removeAt(lineRow);

  bool success = removeRow(lineRow);
  if (success)
    updateLines();

  return success;
}

bool TestLevelModel::replaceLine(int lineRow, const Segment& line) {
  bool res = true;

  std::vector<Point2d> vertices = line.getVertices();
  for (unsigned int k = 0; k < vertices.size(); ++k) {
    res = res && replaceVertex(lineRow, k, vertices.at(k));
  }

  _lineList.replace(lineRow, line);

  return res;
}

bool TestLevelModel::translateLine(int lineRow, const Vector2d& direction) {
  Segment line(_lineList.at(lineRow));
  line.translate(direction);

  bool res = true;

  res = res && replaceLine(lineRow, line);

  return res;
}



bool TestLevelModel::insertVertex(int lineRow, int vertexRow, const Point2d& vertex) {
  insertRow(vertexRow, index(lineRow, 0));

  QModelIndex lineIndex = index(lineRow, 0);
  QModelIndex vertexNameIndex = index(vertexRow, 1, lineIndex);
  QModelIndex vertexXIndex = index(vertexRow, 2, lineIndex);
  QModelIndex vertexYIndex = index(vertexRow, 3, lineIndex);

  setData(vertexNameIndex, QChar(65+vertexRow)+QString::number(lineRow));
  setData(vertexXIndex, vertex.getX());
  setData(vertexYIndex, vertex.getY());

  updateVertices(lineIndex);

  return true;
}

bool TestLevelModel::replaceVertex(int lineRow, int vertexRow, const Point2d& vertex) {
  Segment newLine(_lineList.at(lineRow));
  if (vertexRow == 2)
    newLine.setA(vertex);
  else if (vertexRow == 3)
    newLine.setB(vertex);
  else
    return false;

  return removeLine(lineRow) && insertLine(lineRow, newLine);
}

void TestLevelModel::updateLines(void) {
  for (int k = 0; k < rowCount(); k++) {
    setData(index(k, 0), "Line "+QString::number(k));
    updateVertices(index(k, 0));
  }
}

void TestLevelModel::updateVertices(QModelIndex lineIndex) {
  for (int k = 0; k < rowCount(lineIndex); k++) {
    setData(index(k, 1, lineIndex), QChar(65+k)+QString::number(lineIndex.row()));
  }
}
