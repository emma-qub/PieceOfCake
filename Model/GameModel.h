#ifndef GAMEMODEL_HXX
#define GAMEMODEL_HXX

#include <QList>
#include <QStandardItemModel>

#include "Polygon.h"
#include "Tape.h"

typedef QList<Polygon> PolygonList;
typedef QList<Tape> TapeList;

class GameModel: public QStandardItemModel {
  Q_OBJECT

public:
  GameModel(QObject* parent = NULL);

  inline PolygonList getPolygonList(void) const { return _polygonList; }
  void setPolygonList(const PolygonList& polygonList);

  inline int getPolygonsCount(void) const {return _polygonsCount; }
  inline QColor getColor(void) const { return _color; }

  inline TapeList getTapeList(void) const { return _tapeList; }
  void setTapeList(const TapeList& tapeList);

  void populate(void);

  void insertPolygon(int polygonRow, const Polygon& polygon);
  void appendPolygon(const Polygon& polygon);
  void insertVertex(int polygonRow, int vertexRow, const Point2d& vertex);

  void insertTape(int tapeRow, const Tape& tape);
  void appendTape(const Tape& tape);

  void clearPolygons(void);
  void clearTapes(void);

private:
  QColor _color;
  PolygonList _polygonList;
  TapeList _tapeList;
  int _polygonsCount;
  int _tapesCount;
  QStandardItem* _rootItem;
  QStandardItem* _polygonsItem;
  QStandardItem* _tapesItem;
};

#endif // GAMEMODEL_HXX
