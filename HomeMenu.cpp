#include "HomeMenu.h"
#include "RoundedRectangleShape.h"

#include <iostream>

HomeMenu::HomeMenu(const QPoint& position, const QSize& size, QWidget* parent):
  QSFMLCanvas(position, size, parent),
  _index(0) {

  _font.loadFromFile("../PieceOfCake/fonts/edosz.ttf");
}

HomeMenu::~HomeMenu(void) {
}

void HomeMenu::OnInit(void) {

  int menuItemSize = 64;
  sf::Color black(0, 0, 0);
  float downShift = 75.f;

  // On charge une image
  myImage.loadFromFile("../PieceOfCake/resources/images/delahaye135mpray.jpg");
  myImage.setSmooth(true);

  // On paramètre le sprite
  mySprite.setTexture(myImage);

  mySprite.setColor(sf::Color(255, 255, 255, 16));

  mySprite.scale(1.7, 1.5);

  _title.setFont(_font);
  _title.setCharacterSize(100);
  _title.setString(sf::String("Piece of Cake"));
  _title.setColor(sf::Color(black));
  centerText(_title);

  _play.setFont(_font);
  _play.setCharacterSize(menuItemSize);
  _play.setString(sf::String("Play"));
  _play.setColor(black);
  centerText(_play);
  _play.move(0, 2*downShift);

  _editLevel.setFont(_font);
  _editLevel.setCharacterSize(menuItemSize);
  _editLevel.setString(sf::String("Edit Level"));
  _editLevel.setColor(sf::Color(black));
  centerText(_editLevel);
  _editLevel.move(0, 3*downShift);

  _options.setFont(_font);
  _options.setCharacterSize(menuItemSize);
  _options.setString(sf::String("Options"));
  _options.setColor(sf::Color(black));
  centerText(_options);
  _options.move(0, 4*downShift);

  _credits.setFont(_font);
  _credits.setCharacterSize(menuItemSize);
  _credits.setString(sf::String("Credits"));
  _credits.setColor(sf::Color(black));
  centerText(_credits);
  _credits.move(0, 5*downShift);

  _quit.setFont(_font);
  _quit.setCharacterSize(menuItemSize);
  _quit.setString(sf::String("Quit"));
  _quit.setColor(sf::Color(black));
  centerText(_quit);
  _quit.move(0, 6*downShift);
}

void HomeMenu::OnUpdate(void) {
  // On efface l'écran
  clear(sf::Color(255, 255, 255));

  // Et on l'affiche
  draw(mySprite);

  sf::Event event;
  pollEvent(event);
  if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::Up) {
      _index = (_index == 0) ? (_index + 4) : (_index - 1);
    } else if (event.key.code == sf::Keyboard::Down) {
      _index = (_index+1)%5;
    } else if (event.key.code == sf::Keyboard::Return) {
      emit menuIndexSelected(_index);
    }
  }

  draw(_title);

  drawMenuItem(_play, _index == 0);
  drawMenuItem(_editLevel, _index == 1);
  drawMenuItem(_options, _index == 2);
  drawMenuItem(_credits, _index == 3);
  drawMenuItem(_quit, _index == 4);
}

void HomeMenu::drawMenuItem(sf::Text& text, bool isHightlighted) {
  if (isHightlighted) {
    int border = 20;

    sf::FloatRect bbox = text.getGlobalBounds();
    sf::RoundedRectangleShape rect(sf::Vector2f(bbox.width+border, bbox.height+border), 10, 30);
    rect.setPosition(bbox.left-border/2, bbox.top-border/2+75.f);
    rect.setFillColor(sf::Color(0, 0, 0));
    draw(rect);
    text.setColor(sf::Color(255, 255, 255));
    draw(text);
  } else {
    text.setColor(sf::Color(0, 0, 0));
    draw(text);
  }
}
