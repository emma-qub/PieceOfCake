#ifndef LEVELINFO_H
#define LEVELINFO_H

#include <QObject>

#include "Polygon.h"

typedef QList<Polygon> PolygonList;

class LevelInfo: public QObject {
  Q_OBJECT
  Q_PROPERTY(bool levelReady READ levelReady WRITE setLevelReady NOTIFY levelReadyChanged)

public:
  explicit LevelInfo(QObject *parent = nullptr);

  bool levelReady(void) const;
  void setLevelReady(bool value);
  void updateLevelReady(const PolygonList& polygonList);

signals:
  void levelReadyChanged(void);

private:
  bool _levelReady;
};

#endif // LEVELINFO_H
