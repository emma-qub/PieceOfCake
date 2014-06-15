#include "LevelsImageGenerator.h"
#include "ParserXML.h"
#include "Vector2d.h"

LevelsImageGenerator::LevelsImageGenerator() {
}

//void LevelsImageGenerator::createLevelsImage(const QString& levelsDirectoryName, sf::RenderWindow* window) {
//  int k = 0;

//  QDir levelsDirectory(levelsDirectoryName);
//  QStringList levelsList = levelsDirectory.entryList(QStringList() << "*.xml", QDir::Files, QDir::Time);


//  for (const QString& level: levelsList) {
//    ParserXML parser(level);
//    PolygonList polygons = parser.createPolygonList();

//    Point2d origin;
//    sf::RectangleShape frame = computePolygonFrame(polygons, origin);

//    sf::RectangleShape currFrame(sf::Vector2f(100, 100));
//    float scaleFactor = computeScaleFactor(frame, currFrame);

//    for (Polygon& polygon: polygons) {
//      scalePolygon(polygon, origin, scaleFactor);
//      translatePolygon(polygon, Vector2d(50.f, 50.f + k*100.f));

//      std::vector<Point2d> vertices = polygon.getVertices();
//      unsigned int vertexCount = vertices.size();
//      sf::VertexArray vertexArray(sf::LinesStrip, vertexCount);
//      int i = 0;
//      for (const Point2d vertex: vertices) {
//        vertexArray[i] = sf::Vertex(sf::Vector2f(vertex.getX(), vertex.getY()), sf::Color::Black);
//        ++i;
//      }
//      window->draw(vertexArray, vertexCount, sf::LinesStrip);
//    }
//    ++k;
//  }
//}

//sf::RectangleShape LevelsImageGenerator::computePolygonFrame(const PolygonList& polygons, Point2d& origin) {
//  float minX, minY, maxX, maxY;
//  minX = minY = 10000.f;
//  maxX = maxY = 0.f;

//  for (const Polygon& polygon: polygons) {
//    for (const Point2d& vertex: polygon.getVertices()) {
//      float currX = vertex.getX();
//      float currY = vertex.getY();

//      minX = qMin(minX, currX);
//      minY = qMin(minY, currY);
//      maxX = qMax(minX, currX);
//      maxY = qMax(minY, currY);
//    }
//  }

//  origin.setX((maxX + minX)/2.f);
//  origin.setY((maxY + minY)/2.f);

//  return sf::RectangleShape(sf::Vector2f((maxX - minX)*1.05f, (maxY - minY)*1.05f));
//}

//float LevelsImageGenerator::computeScaleFactor(const sf::RectangleShape& frame, const sf::RectangleShape& finalFrame) {
//  sf::Vector2f finalFrameSize = finalFrame.getSize();
//  sf::Vector2f frameSize = frame.getSize();

//  return qMax(frameSize.x / finalFrameSize.x, frameSize.y / finalFrameSize.y);
//}

//void LevelsImageGenerator::scalePolygon(Polygon& polygon, const Point2d& origin, float scaleFactor) {
//  polygon.homothetie(origin, scaleFactor);
//}

//void LevelsImageGenerator::translatePolygon(Polygon& polygon, const Vector2d& direction) {
//  polygon.translate(direction);
//}
