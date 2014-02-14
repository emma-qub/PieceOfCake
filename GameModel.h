#ifndef GAMEMODEL_HXX
#define GAMEMODEL_HXX

#include <QList>
#include <QStandardItemModel>

#include "Polygon.h"

typedef QList<Polygon> PolygonList;

class GameModel : public QStandardItemModel {
  Q_OBJECT

public:
  GameModel(QObject* parent = NULL);

  inline PolygonList getPolygonList(void) const { return _polygonList; }
  void setPolygonList(const PolygonList& polygonList);

  void populate(void);
  void insertPolygon(int polygonRow, const Polygon& polygon);
  void insertVertex(int polygonRow, int vertexRow, const Point2d& vertex);

private:
  PolygonList _polygonList;
};

#endif // GAMEMODEL_HXX
