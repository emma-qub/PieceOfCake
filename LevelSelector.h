#ifndef LEVELSELECTOR_H
#define LEVELSELECTOR_H

#include "QSFMLCanvas.h"
#include <QDir>
#include "Polygon.h"

typedef QList<Polygon> PolygonList;

class LevelSelector: public QSFMLCanvas {
  Q_OBJECT
public:
  LevelSelector(const QString&  levelsDirectory, const QPoint& position, const QSize& size, QWidget* parent = 0);

  void setStopPress(bool stopPress) { _stopPress = stopPress; }

signals:
  void levelIndexSelected(QString);

protected:
  virtual void OnInit();
  virtual void OnUpdate();

private:
  void computeThumbnail(const sf::RectangleShape& rectangle, int indexLevel);
  sf::RectangleShape computePolygonFrame(const PolygonList& polygons, Point2d& origin);
  float computeScaleFactor(const sf::RectangleShape& frame, const sf::RectangleShape& finalFrame);

private:
  QDir _levelsDirectory;
  bool _stopPress;
  QStringList _levelsEntryList;
  std::vector<sf::VertexArray> _vertexArrays;
};

#endif // LEVELSELECTOR_H
