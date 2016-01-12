#include "ThumbnailCreator.h"

#include <QDebug>
#include <QPainter>
#include <QLabel>

ThumbnailCreator::ThumbnailCreator(const PolygonList& polygonList):
  _polygonList(polygonList) {
}

void ThumbnailCreator::makeThumbnail(const QString& fileName) {
  if (_polygonList.size() < 1)
    return;

  int xmin = _polygonList.at(0).getVertices().at(0).getX();
  int ymin = _polygonList.at(0).getVertices().at(0).getY();
  int xmax = xmin;
  int ymax = ymin;
  for (const poc::Polygon& polygon: _polygonList) {
    for (const Point2d& vertex: polygon.getVertices()) {
      xmin = qMin(xmin, static_cast<int>(vertex.getX()));
      ymin = qMin(ymin, static_cast<int>(vertex.getY()));
      xmax = qMax(xmax, static_cast<int>(vertex.getX()));
      ymax = qMax(ymax, static_cast<int>(vertex.getY()));
    }
  }

  int txmax = xmax - xmin;
  int tymax = ymax - ymin;
  bool isXLarger = txmax > tymax;

  int offsetmin = -1;
  int offsetmax = -1;
  int size = -1;
  if (isXLarger) {
    offsetmin = txmax / 6;
    size = 8 * offsetmin;
    offsetmax = (size - tymax) / 2;
  } else {
    offsetmin = tymax / 6;
    size = 8 * offsetmin;
    offsetmax = (size - txmax) / 2;
  }

  auto it = _polygonList.begin();
  for (; it != _polygonList.end(); ++it) {
    if (isXLarger)
      it->translate(offsetmin - xmin, offsetmax - ymin);
    else
      it->translate(offsetmax - xmin, offsetmin - ymin);
  }

  QImage image(QSize(size, size), QImage::Format_ARGB32);
  QPainter painter(&image);
  image.fill(qRgba(0, 0, 0, 0));
  painter.setPen(QPen(Qt::white, 20));
  painter.setRenderHint(QPainter::Antialiasing, true);
  for (const poc::Polygon& polygon: _polygonList) {
    std::vector<Point2d> vertices = polygon.getVertices();
    for (unsigned int k = 0; k < vertices.size(); ++k) {
      Point2d currVertex = vertices.at(k);
      Point2d nextVertex = vertices.at((k+1)%vertices.size());
      QPoint A(currVertex.getX(), currVertex.getY());
      QPoint B(nextVertex.getX(), nextVertex.getY());
      painter.drawLine(A, B);
    }
  }

  QImage thumbnail = image.scaled(50, 50);
  thumbnail.save(fileName, "PNG", 100);
}
