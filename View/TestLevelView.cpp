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

  connect(_controller, SIGNAL(update()), this, SLOT(drawFromModel()));
  resize(401, 401);
}

TestLevelView::~TestLevelView() {
}


void TestLevelView::setModels(GameModel* model, TestLevelModel* lineModel) {
  _model = model;
  _lineModel = lineModel;
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

    if (_goodSegment) {
      _controller->sliceItNot(lines);
    } else {
      _model->setPolygonList(_controller->getPrevPolygonList());
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
      emit newLineDrawn(line);
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

void TestLevelView::drawText(const QPoint& position, const QString& text, const QColor &color) {
  QPainter painter(&_image);
  painter.setPen(QPen(color, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  painter.setFont(QFont("", 12, 99));
  QFontMetrics fm = painter.fontMetrics();
  int leftShift = -fm.width(text)/2;
  int topShift = fm.height()/4;
  painter.drawText(position+QPoint(leftShift, topShift), text);
}

void TestLevelView::drawFromModel(void) {
  clearImage();

  PolygonList polygons = _model->getPolygonList();

  for (const poc::Polygon& polygon: polygons) {
    QColor color(_model->getColor());

    float area = _controller->computePolygonPercentageArea(polygon);
    Point2d areaPoint = polygon.barycenter();
    drawText(QPoint(areaPoint.getX(), areaPoint.getY()), QString::number(area), Qt::black);

    std::vector<Point2d> vertices = polygon.getVertices();
    for (unsigned k = 0; k < vertices.size(); k++) {
      Point2d fstVertex(vertices.at(k));
      Point2d sndVertex(vertices.at((k+1)%vertices.size()));

      //drawText(QPoint(fstVertex.getX(), fstVertex.getY()), QString::number(k));
      drawLine(QPoint(fstVertex.getX(), fstVertex.getY()), QPoint(sndVertex.getX(), sndVertex.getY()), color);
    }
  }

  LineList lines = _lineModel->getLineList();
  for (const Segment& line: lines) {
    drawLine(QPoint(line.getA().getX(), line.getA().getY()), QPoint(line.getB().getX(), line.getB().getY()), _lineModel->getRandomColor());
  }
}

void TestLevelView::clearImage(void) {
  _image.fill(Qt::white);
}
