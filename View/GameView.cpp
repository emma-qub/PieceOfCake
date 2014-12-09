#include "GameView.h"

GameView::GameView(GameController* controller, QWidget* parent):
  QWidget(parent),
  _scribbling(false),
  _myPenWidth(5),
  _myPenColor(Qt::black),
  _image(),
  _firstPoint(),
  _canErase(false),
  _goodSegment(false),
  _controller(controller) {

  _mousePositionLabel = new QLabel(this);
  _mousePositionLabel->setFixedSize(300, 50);

  _linesCountLabel = new QLabel(this);
  _linesCountLabel->setFixedSize(300, 50);
  _linesCountLabel->move(100, 0);

  _partsCountLabel = new QLabel(this);
  _partsCountLabel->setFixedSize(300, 50);
  _partsCountLabel->move(200, 0);

  resize(401, 401);

  setMouseTracking(true);

  connect(_controller, SIGNAL(update()), this, SLOT(drawFromModel()));
  connect(_controller, SIGNAL(levelEnd(QList<float>, GameController::Ranking)), this, SLOT(levelEnd(QList<float>, GameController::Ranking)));
}

void GameView::setSelectionModel(QItemSelectionModel* selectionModel) {
  _selectionModel = selectionModel;
  connect(_selectionModel, SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(currentChanged(QModelIndex,QModelIndex)));
}

void GameView::setModel(GameModel* model) {
  _model = model;
}


void GameView::mousePressEvent(QMouseEvent* event) {
  if (event->buttons() == Qt::LeftButton && !_scribbling) {
    _scribbling = true;
    _firstPoint = event->pos();
  }
}

void GameView::mouseMoveEvent(QMouseEvent* event) {
  emit position(event->pos().x(), event->pos().y());

  _mousePositionLabel->setText(QString::number(event->pos().x())+QString("   ")+QString::number(event->pos().y()));

  if (_scribbling) {
    QPoint currPoint(event->pos());

    Point2d firstPoint(_firstPoint.x(), _firstPoint.y());

    _goodSegment = false;

    Segment line(firstPoint, Point2d(currPoint.x(), currPoint.y()));
    GameController::LineType lineType = _controller->computeLineType(line);
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

void GameView::mouseReleaseEvent(QMouseEvent* event) {
  if (_scribbling) {
    clearImage();
    drawFromModel();
    if (_goodSegment) {
      Point2d A(_firstPoint.x(), _firstPoint.y());
      Point2d B(event->pos().x(), event->pos().y());
      Segment line(A, B);
      _controller->sliceIt(line);
    }
    _scribbling = false;
  }
}

void GameView::paintEvent(QPaintEvent* event) {
  QPainter painter(this);
  QRect dirtyRect = event->rect();
  painter.drawImage(dirtyRect, _image, dirtyRect);
}

void GameView::resizeEvent(QResizeEvent* event) {
  if (width() > _image.width() || height() > _image.height()) {
    int newWidth = qMax(width() + 128, _image.width());
    int newHeight = qMax(height() + 128, _image.height());
    resizeImage(&_image, QSize(newWidth, newHeight));
    update();
  }
  QWidget::resizeEvent(event);
}

void GameView::resizeImage(QImage* image, const QSize& newSize) {
  if (image->size() == newSize)
    return;

  QImage newImage(newSize, QImage::Format_RGB32);
  newImage.fill(qRgb(255, 255, 255));
  QPainter painter(&newImage);
  painter.drawImage(QPoint(0, 0), *image);
  *image = newImage;
}

void GameView::drawLineTo(const QPoint& endPoint) {
  drawLine(_firstPoint, endPoint, _myPenColor);
}

void GameView::drawLine(const QPoint& begin, const QPoint& end, const QColor& color, Qt::PenStyle penStyle) {
  QPainter painter(&_image);
  painter.setPen(QPen(color, _myPenWidth, penStyle, Qt::RoundCap, Qt::BevelJoin));
  painter.drawLine(begin, end);

  update();
}

void GameView::circlePoint(const QPoint& point, const QColor& color, Qt::PenStyle penStyle) {
  QPainter painter(&_image);
  painter.setPen(QPen(color, 2, penStyle, Qt::RoundCap, Qt::BevelJoin));
  painter.drawEllipse(point, 5, 5);

  update();
}

void GameView::drawFromModel(void) {
  clearImage();

  PolygonList polygons = _model->getPolygonList();
  _linesCountLabel->setText("Lines: "+QString::number(_controller->getLinesDrawn())+"/"+QString::number(_controller->getLinesCount()));
  _partsCountLabel->setText("Parts: "+QString::number(_controller->getPolygonsCount())+"/"+QString::number(_controller->getPartsCount()));

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

void GameView::drawText(const QPoint& position, const QString& text, const QColor &color) {
  QPainter painter(&_image);
  painter.setPen(QPen(color, _myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  painter.setFont(QFont("", 25));
  QFontMetrics fm = painter.fontMetrics();
  int leftShift = -fm.width(text)/2;
  int topShift = fm.height()/4;
  painter.drawText(position+QPoint(leftShift, topShift), text);
}

void GameView::clearImage(void) {
  _image.fill(Qt::white);
}

void GameView::currentChanged(QModelIndex currentIndex, QModelIndex /*previousIndex*/) {
  drawFromModel();
  if (currentIndex.parent() != QModelIndex()) {
    if (currentIndex.parent().parent() != QModelIndex()) {
      currentIndex = currentIndex.parent();
    }

    Point2d currentVertex = _model->getPolygonList().at(currentIndex.parent().row()).getVertices().at(currentIndex.row());
    circlePoint(QPoint(currentVertex.getX(), currentVertex.getY()));
  }
}

void GameView::drawAreaValues(QList<float> orientedAreas) {
  PolygonList polygons = _model->getPolygonList();
  for (int k = 0; k < polygons.size(); ++k) {
    Polygon polygon = polygons.at(k);
    Point2d areaPoint = polygon.barycenter();
    drawText(QPoint(areaPoint.getX(), areaPoint.getY()), QString::number(orientedAreas.at(k)), Qt::black);
  }
}

void GameView::levelEnd(QList<float> orientedAreas, GameController::Ranking ranking) {
  QString rankingMessage;
  QString starsMessage;

  drawAreaValues(orientedAreas);

  switch (ranking) {
  case GameController::fail:
    rankingMessage = "Fail";
    starsMessage = "";
    break;
  case GameController::poor:
    rankingMessage = "Poor";
    starsMessage = "*";
    break;
  case GameController::average:
    rankingMessage = "Average";
    starsMessage = "**";
    break;
  case GameController::nice:
    rankingMessage = "Nice";
    starsMessage = "***";
    break;
  case GameController::good:
    rankingMessage = "Good";
    starsMessage = "****";
    break;
  case GameController::great:
    rankingMessage = "Great";
    starsMessage = "*****";
    break;
  case GameController::perfect:
    rankingMessage = "Perfect";
    starsMessage = "***** P";
    break;
  default:
    rankingMessage = "Fail";
    break;
  }

  QMessageBox::StandardButton buttonClicked = QMessageBox::information(this, rankingMessage, starsMessage+"\nReplay?", QMessageBox::Yes, QMessageBox::No);

  if (buttonClicked == QMessageBox::Yes)
    _controller->replay();
  else
    _controller->clearGame();
}
