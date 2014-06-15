#include "LevelSelector.h"
#include "ParserXML.h"
#include "Vector2d.h"

#include <iostream>

LevelSelector::LevelSelector(const QString& levelsDirectory, const QPoint& position, const QSize& size, QWidget* parent):
  QSFMLCanvas(position, size, parent),
  _levelsDirectory(QDir("../PieceOfCake/levels/"+levelsDirectory)),
  _stopPress(false) {

  _font.loadFromFile("../PieceOfCake/fonts/edosz.ttf");
  _levelsEntryList = _levelsDirectory.entryList(QStringList() << "*.xml", QDir::Files, QDir::Time | QDir::Reversed);
}

void LevelSelector::OnInit() {
  _font.loadFromFile("../PieceOfCake/fonts/edosz.ttf");

  _title.setFont(_font);
  _title.setCharacterSize(50);
  _title.setString(sf::String("Select your level"));
  _title.setColor(sf::Color(0, 0, 0));
  centerText(_title);

  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 4; ++j) {
      sf::RectangleShape rectangle(sf::Vector2f(100, 100));
      rectangle.setPosition(150 + (200*i), 150 + (150*j));

      int levelIndex = i + 5*j;

      if (levelIndex < _levelsEntryList.size()) {
        std::cerr << rectangle.getPosition().x << " " << rectangle.getPosition().y << std::endl;
        computeThumbnail(rectangle, levelIndex);
      }
    }
  }
}

void LevelSelector::OnUpdate() {
  clear(sf::Color(255, 255, 255));
  draw(_title);

  sf::Vector2i mouse = sf::Mouse::getPosition(*this);

  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 4; ++j) {
      sf::RectangleShape rectangle(sf::Vector2f(100, 100));
      rectangle.setOutlineColor(sf::Color(0, 0, 0));
      rectangle.setPosition(150 + (200*i), 150 + (150*j));

      int mouseX = mouse.x;
      int mouseY = mouse.y;

      if (((150 + (200*i)) < mouseX) && (mouseX < (250 + (200*i)))
       && ((130 + (150*j)) < mouseY) && (mouseY < (230 + (150*j)))) {
        rectangle.setOutlineThickness(4);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !_stopPress) {
          int xLevel = (mouse.x - 150)/200;
          int yLevel = (mouse.y - 130)/150;
          _stopPress = true;
          emit levelIndexSelected(xLevel + 5*yLevel + 1);
        }
      } else {
        rectangle.setOutlineThickness(2);
      }

      draw(rectangle);
      for (const sf::VertexArray& vertexArray: _vertexArrays) {
        draw(&vertexArray[0], vertexArray.getVertexCount(), sf::LinesStrip);
      }
    }
  }
}

void LevelSelector::computeThumbnail(const sf::RectangleShape& rectangle, int indexLevel) {
  ParserXML parser("../PieceOfCake/levels/levelsPack1/"+_levelsEntryList.at(indexLevel));
  PolygonList polygons = parser.createPolygonList();

  Point2d origin;
  sf::RectangleShape frame = computePolygonFrame(polygons, origin);

  float scaleFactor = computeScaleFactor(frame, rectangle);

  for (Polygon& polygon: polygons) {
    polygon.homothetie(origin, scaleFactor);
    int line = indexLevel/5;
    int column = indexLevel%5;
    sf::Vector2f newOrigin((150.f + rectangle.getPosition().x+rectangle.getSize().x)/2.f + column*200/2,
                           (150.f + rectangle.getPosition().y+rectangle.getSize().y)/2.f + line*150/2);
    polygon.translate(Vector2d(newOrigin.x - origin.getX(), newOrigin.y - origin.getY()));

    std::vector<Point2d> vertices = polygon.getVertices();
    unsigned int vertexCount = vertices.size();
    sf::VertexArray vertexArray(sf::LinesStrip, vertexCount+1);
    int i = 0;
    for (const Point2d vertex: vertices) {
      vertexArray[i] = sf::Vertex(sf::Vector2f(vertex.getX(), vertex.getY()), sf::Color::Black);
      ++i;
    }
    vertexArray[vertexCount] = vertexArray[0];
    _vertexArrays.push_back(vertexArray);
  }
}

sf::RectangleShape LevelSelector::computePolygonFrame(const PolygonList& polygons, Point2d& origin) {
  float minX, minY, maxX, maxY;
  minX = minY = 10000.f;
  maxX = maxY = 0.f;

  for (const Polygon& polygon: polygons) {
    for (const Point2d& vertex: polygon.getVertices()) {
      float currX = vertex.getX();
      float currY = vertex.getY();

      minX = qMin(minX, currX);
      minY = qMin(minY, currY);
      maxX = qMax(maxX, currX);
      maxY = qMax(maxY, currY);
    }
  }

  origin.setX((maxX + minX)/2.f);
  origin.setY((maxY + minY)/2.f);

  return sf::RectangleShape(sf::Vector2f((maxX - minX)*1.50f, (maxY - minY)*1.50f));
}

float LevelSelector::computeScaleFactor(const sf::RectangleShape& frame, const sf::RectangleShape& finalFrame) {
  sf::Vector2f finalFrameSize = finalFrame.getSize();
  sf::Vector2f frameSize = frame.getSize();

  return qMin(finalFrameSize.x / frameSize.x, finalFrameSize.y / frameSize.y);
}
