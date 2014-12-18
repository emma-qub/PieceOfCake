#include "LevelInfo.h"

LevelInfo::LevelInfo(QObject* parent):
  QObject(parent),
  _levelReady(false) {
}

bool LevelInfo::levelReady(void) const {
  return _levelReady;
}

void LevelInfo::setLevelReady(bool value) {
  if (value != _levelReady) {
    _levelReady = value;
    emit levelReadyChanged();
  }
}

void LevelInfo::updateLevelReady(const PolygonList& polygonList) {
  for (const Polygon& polygon: polygonList) {
    if (polygon.getVertices().size() < 3) {
      setLevelReady(false);
      return;
    }
  }

  setLevelReady(true);
}
