#ifndef LEVELSIMAGEGENERATOR_H
#define LEVELSIMAGEGENERATOR_H

#include <QString>
#include "Polygon.h"
#include <SFML/Graphics.hpp>
#include <QDir>

typedef QList<Polygon> PolygonList;

class LevelsImageGenerator
{
public:
  LevelsImageGenerator();

  void createLevelsImage(const QString& levelsDirectoryName, sf::RenderWindow* window);

private:
  sf::RectangleShape computePolygonFrame(const PolygonList& polygons, Point2d& origin);
  float computeScaleFactor(const sf::RectangleShape& frame, const sf::RectangleShape& finalFrame);
  void scalePolygon(Polygon& polygon, const Point2d& origin, float scaleFactor);
  void translatePolygon(Polygon& polygon, const Vector2d& direction);
};

#endif // LEVELSIMAGEGENERATOR_H
