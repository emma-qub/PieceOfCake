#ifndef HOMEMENU_H
#define HOMEMENU_H

#include "QSFMLCanvas.h"

class HomeMenu: public QSFMLCanvas {
public:
  HomeMenu(const QPoint& position, const QSize& size, QWidget* parent = 0);
  virtual ~HomeMenu(void);

protected:
  virtual void OnInit();
  virtual void OnUpdate();

private:
  sf::Texture myImage;
  sf::Sprite mySprite;
  sf::Text _title;
  sf::Font _font;
};

#endif // HOMEMENU_H
