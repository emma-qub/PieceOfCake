#ifndef HOMEMENU_H
#define HOMEMENU_H

#include "QSFMLCanvas.h"

class HomeMenu: public QSFMLCanvas {
  Q_OBJECT

public:
  HomeMenu(const QPoint& position, const QSize& size, QWidget* parent = 0);
  virtual ~HomeMenu(void);

protected:
  virtual void OnInit();
  virtual void OnUpdate();

signals:
  void menuIndexSelected(int);

private:
  void drawMenuItem(sf::Text& text, bool isHightlighted);

private:
  sf::Texture myImage;
  sf::Sprite mySprite;
  sf::Text _title;
  sf::Text _play;
  sf::Text _editLevel;
  sf::Text _options;
  sf::Text _credits;
  sf::Text _quit;
  sf::Font _font;
  int _index;
};

void centerText(sf::Text& text);

#endif // HOMEMENU_H
