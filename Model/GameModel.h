#ifndef GAMEMODEL_HXX
#define GAMEMODEL_HXX

#include <QList>
#include <QStandardItemModel>

#include "Polygon.h"
#include "Tape.h"
#include "Mirror.h"

typedef QList<Polygon> PolygonList;
typedef QList<Tape> TapeList;
typedef QList<Mirror> MirrorList;

class GameModel: public QStandardItemModel {
  Q_OBJECT

public:
  GameModel(QObject* parent = NULL);

  inline int getPolygonsCount(void) const {return _polygonsCount; }
  inline QColor getColor(void) const { return _color; }

  inline PolygonList getPolygonList(void) const { return _polygonList; }
  void setPolygonList(const PolygonList& polygonList);

  inline TapeList getTapeList(void) const { return _tapeList; }
  void setTapeList(const TapeList& tapeList);

  inline MirrorList getMirrorList(void) const { return _mirrorList; }
  void setMirrorList(const MirrorList& mirrorList);

  void populate(void);

  void insertPolygon(int polygonRow, const Polygon& polygon);
  void appendPolygon(const Polygon& polygon);
  void insertVertex(int polygonRow, int vertexRow, const Point2d& vertex);

  void insertTape(int tapeRow, const Tape& tape);
  void appendTape(const Tape& tape);

  void insertMirror(int mirrorRow, const Mirror& mirror);
  void appendMirror(const Mirror& mirror);

  void clearPolygons(void);
  void clearTapes(void);
  void clearMirrors(void);

private:
  QColor _color;

  PolygonList _polygonList;
  TapeList _tapeList;
  MirrorList _mirrorList;

  int _polygonsCount;
  int _tapesCount;
  int _mirrorsCount;

  QStandardItem* _polygonsItem;
  QStandardItem* _tapesItem;
  QStandardItem* _mirrorsItem;
};

#endif // GAMEMODEL_HXX
