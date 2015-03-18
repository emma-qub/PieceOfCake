#include "TestLevelView.h"

TestLevelView::TestLevelView(TestLevelController* controller, QWidget* parent):
  QWidget(parent),
  _scribbling(false),
  _image(),
  _firstPoint(),
  _lastPoint(),
  _goodSegment(false),
  _runningSegment(false),
  _controller(controller) {

  resize(401, 401);

}

TestLevelView::~TestLevelView() {
}


void TestLevelView::setModel(GameModel* model) {
  _model = model;
}

void TestLevelView::mousePressEvent(QMouseEvent* event) {
  if (event->buttons() == Qt::LeftButton && !_scribbling && !_runningSegment) {
    _scribbling = true;
    _runningSegment = true;
    _firstPoint = event->pos();
    _lastPoint = event->pos();
  } else if (event->buttons() == Qt::LeftButton && !_scribbling && _runningSegment) {
    _runningSegment = true;
    _lastPoint = event->pos();
  } else if (event->buttons() == Qt::RightButton && !_scribbling && _runningSegment) {
    _runningSegment = true;
    _firstPoint = event->pos();
  }
}

void TestLevelView::mouseMoveEvent(QMouseEvent* event) {
  if (_scribbling) {
    QPoint currPoint(event->pos());

    Point2d firstPoint(_firstPoint.x(), _firstPoint.y());

    _goodSegment = false;

    Segment line(firstPoint, Point2d(currPoint.x(), currPoint.y()));
    std::vector<Segment> lines;
    _controller->computeDeviateLines(-1.f, line, lines);
    GameController::LineType lineType = _controller->computeLineType(lines);
    QColor color;
    switch (lineType) {
    case GameController::noCrossing:
      color = QColor(0x626262);
      break;
    case GameController::goodCrossing:
      _goodSegment = true;
      color = QColor(0x5DBE14);
      break;
    case GameController::badCrossing:
      color = QColor(0xC81214);
      break;
    default:
      break;
    }

    clearImage();
    drawFromModel();
    drawLine(_firstPoint, currPoint, color, Qt::DashLine);
  }
}

void TestLevelView::mouseReleaseEvent(QMouseEvent* event) {
  if (_scribbling) {
    clearImage();
    drawFromModel();
    if (_goodSegment) {
      Point2d A(_firstPoint.x(), _firstPoint.y());
      Point2d B(event->pos().x(), event->pos().y());
      Segment line(A, B);
      std::vector<Segment> lines;
      _controller->computeDeviateLines(-1.f, line, lines);
      _controller->sliceIt(lines);
      GameInfo* g = _controller->getGameInfo();
      std::cerr << g->linesCount() << std::endl;
      std::cerr << g->linesDrawn() << std::endl;
      std::cerr << g->partsCount() << std::endl;
      std::cerr << g->partsCut() << std::endl;
      emit newLineDrawn(line);
      std::cerr << "\n" << g->linesCount() << std::endl;
      std::cerr << g->linesDrawn() << std::endl;
      std::cerr << g->partsCount() << std::endl;
      std::cerr << g->partsCut() << std::endl;
      drawFromModel();
    }
    _scribbling = false;
    _runningSegment = false;
  }
}

void TestLevelView::paintEvent(QPaintEvent* event) {
  QPainter painter(this);
  QRect dirtyRect = event->rect();
  painter.drawImage(dirtyRect, _image, dirtyRect);
}

void TestLevelView::resizeEvent(QResizeEvent* event) {
  if (width() > _image.width() || height() > _image.height()) {
    int newWidth = qMax(width() + 128, _image.width());
    int newHeight = qMax(height() + 128, _image.height());
    resizeImage(&_image, QSize(newWidth, newHeight));
    update();
  }
  QWidget::resizeEvent(event);
}

void TestLevelView::resizeImage(QImage* image, const QSize& newSize) {
  if (image->size() == newSize)
    return;

  QImage newImage(newSize, QImage::Format_RGB32);
  newImage.fill(qRgb(255, 255, 255));
  QPainter painter(&newImage);
  painter.drawImage(QPoint(0, 0), *image);
  *image = newImage;
}

void TestLevelView::drawLine(const QPoint& begin, const QPoint& end, const QColor& color, Qt::PenStyle penStyle) {
  QPainter painter(&_image);
  painter.setPen(QPen(color, 5, penStyle, Qt::RoundCap, Qt::BevelJoin));
  painter.drawLine(begin, end);

  update();
}

void TestLevelView::drawFromModel(void) {
  clearImage();

  PolygonList polygons = _model->getPolygonList();

  for (const Polygon& polygon: polygons) {
    QColor color(_model->getColor());

    std::vector<Point2d> vertices = polygon.getVertices();
    for (unsigned k = 0; k < vertices.size(); k++) {
      Point2d fstVertex(vertices.at(k));
      Point2d sndVertex(vertices.at((k+1)%vertices.size()));

      //drawText(QPoint(fstVertex.getX(), fstVertex.getY()), QString::number(k));
      drawLine(QPoint(fstVertex.getX(), fstVertex.getY()), QPoint(sndVertex.getX(), sndVertex.getY()), color);
    }
  }
}

void TestLevelView::clearImage(void) {
  _image.fill(Qt::white);
}
