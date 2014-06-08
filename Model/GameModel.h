#ifndef GAMEMODEL_HXX
#define GAMEMODEL_HXX

#include <QList>
#include <QStandardItemModel>

#include "Polygon.h"

typedef QList<Polygon> PolygonList;

class GameModel: public QStandardItemModel {
  Q_OBJECT

public:
  GameModel(QObject* parent = NULL);

  inline PolygonList getPolygonList(void) const { return _polygonList; }
  void setPolygonList(const PolygonList& polygonList);

  inline int getPolygonsCount(void) const {return _polygonsCount; }
  inline QColor getColor(void) const { return _color; }

  void populate(void);
  void insertPolygon(int polygonRow, const Polygon& polygon);
  void insertVertex(int polygonRow, int vertexRow, const Point2d& vertex);

  void clearPolygons(void);

private:
  QColor _color;
  PolygonList _polygonList;
  int _polygonsCount;
};

#endif // GAMEMODEL_HXX
