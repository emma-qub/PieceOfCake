#ifndef POLYGONTREEMODEL_H
#define POLYGONTREEMODEL_H

#include <QStack>

#include "Polygon.h"
#include "TreeItem.h"
#include "TreeModel.h"

typedef QList<Polygon> PolygonList;

class  LevelDesignerModel: public TreeModel {
  Q_OBJECT

public:
  explicit LevelDesignerModel(const QStringList& headers = QStringList(), const QString& data = QString(), QObject* parent = 0);
  virtual ~ LevelDesignerModel(void);

  inline PolygonList getPolygonList(void) const { return _polygonList; }
  inline int getPartsCount() const { return _partsCount; }
  inline int getLinesCount() const { return _linesCount; }
  inline int getMaxGapToWin() const { return _maxGapToWin; }

  void setPolygonList(const PolygonList& polygonList);
  inline void setPartsCount(int partsCount) { _partsCount = partsCount; }
  inline void setLinesCount(int linesCount) { _linesCount = linesCount; }
  inline void setMaxGapToWin(int maxGapToWin) { _maxGapToWin = maxGapToWin; }

  inline QStack<QPair<int, int>> getSelections(void) const { return _selections; }
  inline void addSelection(QPair<int, int> selection) { _selections << selection; }
  inline void addSelection(int polygonRow, int vertexRow) { addSelection(QPair<int, int>(polygonRow, vertexRow)); }
  inline QPair<int, int> popSelection(void) { return _selections.pop(); }

  void clear(void);

  bool polygonHasVertex(int polygonRow);

  Polygon polygonFromIndex(const QModelIndex& polygonIndex);
  Point2d vertexFromIndex(const QModelIndex& vertexIndex);

  bool appendPolygon(const Polygon& polygon);
  bool insertPolygon(int polygonRow, const Polygon& polygon);
  bool removePolygon(int polygonRow);
  bool replacePolygon(int polygonRow, const Polygon& polygon);
  bool translatePolygon(int polygonRow, const Vector2d& direction);

  bool insertVertex(int polygonRow, int vertexRow, const Point2d& vertex, bool exist = false);
  bool removeVertex(int polygonRow, int vertexRow);
  bool replaceVertex(int polygonRow, int vertexRow, const Point2d& vertex);

  void updatePolygons(void);
  void updateVertices(QModelIndex polygonIndex);

//  void description(QString& desc, TreeItem* item, int shift);
//  void description(void);

public slots:
  void debug(QModelIndex,QModelIndex);

protected:
  PolygonList _polygonList;
  QStack<QPair<int, int>> _selections;
  int _partsCount;
  int _linesCount;
  int _maxGapToWin;
};

#endif // POLYGONTREEMODEL_H
