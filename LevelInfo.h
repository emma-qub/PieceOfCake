#ifndef LEVELINFO_H
#define LEVELINFO_H

#include <QObject>

#include "Polygon.h"

typedef QList<poc::Polygon> PolygonList;

class LevelInfo: public QObject {
  Q_OBJECT
  Q_PROPERTY(bool levelReadyToBeTested READ levelReadyToBeTested WRITE setLevelReadyToBeTested NOTIFY levelReadyToBeTestedChanged)
  Q_PROPERTY(bool levelReadyToBeCut READ levelReadyToBeCut WRITE setLevelReadyToBeCut NOTIFY levelReadyToBeCutChanged)

public:
  explicit LevelInfo(QObject *parent = nullptr);

  bool levelReadyToBeTested(void) const;
  void setLevelReadyToBeTested(bool value);
  void updateLevelReadyToBeTested(const PolygonList& polygonList);

  bool levelReadyToBeCut(void) const;
  void setLevelReadyToBeCut(bool value);
  void updateLevelReadyToBeCut(int linesDrawn);

signals:
  void levelReadyToBeTestedChanged(void);
  void levelReadyToBeCutChanged(void);

private:
  bool _levelReadyToBeTested;
  bool _levelReadyToBeCut;
};

#endif // LEVELINFO_H
