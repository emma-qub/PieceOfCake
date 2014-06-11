#include "HomeMenu.h"

#include <QMessageBox>

HomeMenu::HomeMenu(const QPoint& position, const QSize& size, QWidget* parent):
  QSFMLCanvas(position, size, parent) {
}

HomeMenu::~HomeMenu(void) {
}

void HomeMenu::OnInit(void) {
  // On charge une image
  myImage.loadFromFile("../PieceOfCake/resources/images/delahaye135mpray.jpg");

  // On paramètre le sprite
  mySprite.setTexture(myImage);
}

void HomeMenu::OnUpdate(void) {
  // On efface l'écran
  clear(sf::Color(0, 128, 0));

  // Et on l'affiche
  draw(mySprite);
}
