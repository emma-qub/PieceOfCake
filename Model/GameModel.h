#ifndef GAMEMODEL_HXX
#define GAMEMODEL_HXX

#include <QList>
#include <QStandardItemModel>

#include "Polygon.h"
#include "Tape.h"
#include "Mirror.h"
#include "Portal.h"
#include "Refractor.h"

typedef QList<poc::Polygon> PolygonList;
typedef QList<Tape> TapeList;
typedef QList<Mirror> MirrorList;
typedef QList<Portal> PortalList;
typedef QList<Refractor> RefractorList;
typedef QList<Deviation*> DeviationList;

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

  inline DeviationList getDeviationList(void) const { return _deviationList; }
  void setDeviationList(const DeviationList& deviationList);

  void populate(void);

  void insertPolygon(int polygonRow, const poc::Polygon& polygon);
  void appendPolygon(const poc::Polygon& polygon);
  void insertVertex(int polygonRow, int vertexRow, const Point2d& vertex);

  void insertTape(int tapeRow, const Tape& tape);
  void appendTape(const Tape& tape);

  void insertMirror(int mirrorRow, const Mirror& mirror);
  void appendMirror(const Mirror& mirror);

  void insertPortal(int portalRow, const Portal& portal);
  void appendPortal(const Portal& portal);

  void insertRefractor(int refractorRow, const Refractor& refractor);
  void appendRefractor(const Refractor& refractor);

  void clearPolygons(void);
  void clearTapes(void);
  void clearMirrors(void);
  void clearPortals(void);
  void clearRefractors(void);
  void clearDeviations(void);

private:
  QColor _color;

  PolygonList _polygonList;
  TapeList _tapeList;
  MirrorList _mirrorList;
  PortalList _portalList;
  RefractorList _refractorList;
  DeviationList _deviationList;

  int _polygonsCount;
  int _tapesCount;
  int _mirrorsCount;
  int _portalsCount;
  int _refractorsCount;
  int _deviationsCount;

  QStandardItem* _polygonsItem;
  QStandardItem* _tapesItem;
  QStandardItem* _mirrorsItem;
  QStandardItem* _portalsItem;
  QStandardItem* _refractorsItem;
};

#endif // GAMEMODEL_HXX
