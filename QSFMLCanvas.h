#ifndef QSFMLCANVAS_H
#define QSFMLCANVAS_H

#include <QWidget>
#include <QTimer>

#include <SFML/Graphics.hpp>

class QSFMLCanvas: public QWidget, public sf::RenderWindow {
public:
  QSFMLCanvas(const QPoint& position, const QSize& size, QWidget* parent = 0, unsigned int frameTime = 0);
  virtual ~QSFMLCanvas(void);

protected:
  virtual void OnInit(void);
  virtual void OnUpdate(void);
  virtual QPaintEngine* paintEngine(void) const;
  virtual void showEvent(QShowEvent*);
  virtual void paintEvent(QPaintEvent*);

  QTimer _timer;
  bool _initialized;
  sf::Text _title;
  sf::Font _font;
};

void centerText(sf::Text& text);

#endif // QSFMLCANVAS_H
