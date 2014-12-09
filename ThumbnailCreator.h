#ifndef THUMBNAILCREATOR_H
#define THUMBNAILCREATOR_H

#include <QImage>

#include "Polygon.h"

typedef QList<Polygon> PolygonList;

class ThumbnailCreator {
public:
  ThumbnailCreator(const PolygonList& polygonList);
  void makeThumbnail(const QString& fileName);

private:
  PolygonList _polygonList;
};

#endif // THUMBNAILCREATOR_H
