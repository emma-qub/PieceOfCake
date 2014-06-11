#include "QSFMLCanvas.h"

QSFMLCanvas::QSFMLCanvas(const QPoint& position, const QSize& size, QWidget* parent, unsigned int frameTime):
  QWidget(parent),
  RenderWindow(),
  _initialized(false) {

  // Setup options to render widget
  setAttribute(Qt::WA_PaintOnScreen);         // Don't use Qt painting, draw directly on widget
  setAttribute(Qt::WA_OpaquePaintEvent);
  setAttribute(Qt::WA_NoSystemBackground);    // Don't display widget background to avoid sparkling

  // Set focus policy to handle keyboard events
  setFocusPolicy(Qt::StrongFocus);

  // Set position and size
  move(position);
  resize(size);

  _timer.setInterval(frameTime);
}

QSFMLCanvas::~QSFMLCanvas(void) {
}

#ifdef Q_WS_X11
    #include <Qt/qx11info_x11.h>
    #include <X11/Xlib.h>
#endif

void QSFMLCanvas::onInit(void) {
  // Let the sub class define it
}

void QSFMLCanvas::onUpdate(void) {
  // Let the sub class define it
}

QPaintEngine* QSFMLCanvas::paintEngine(void) const {
  // Since we don't use Qt painting mechanism
  return NULL;
}

void QSFMLCanvas::showEvent(QShowEvent*) {
  if (_initialized) {
    // With X11, commands that have been sent to the server have to be checked
    // in order to make sure that the SFML will have the up-to-date window
#ifdef Q_WS_X11
      XFlush(QX11Info::display());
#endif

    // Create SFML window with the widget Id
    sf::Window::create(winId());

    // Initialize
    onInit();

    // Set timer to refresh with the right frequency
    connect(&_timer, SIGNAL(timeout()), this, SLOT(repaint()));
    _timer.start();

    _initialized = true;
  }
}

void QSFMLCanvas::paintEvent(QPaintEvent*) {
  // Let the sub class handle it
  onInit();

  // Refresh widget
  display();
}
