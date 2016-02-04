#include "LevelDesignerScribbleView.h"
#include "Polygon.h"
#include "Commands.h"

const int LevelDesignerScribbleView::PEN_WIDTH = 7;
const QColor LevelDesignerScribbleView::NOT_SELECTED_COLOR = QColor(0xCCCCCC);

LevelDesignerScribbleView::LevelDesignerScribbleView(LevelDesignerController* controller, QWidget* parent):
  QWidget(parent),
  _model(0),
  _image(),
  _hasToDraw(true),
  _controller(controller),
  _undoStack(controller->getUndoStack()),
  _isMagnetic(false),
  _isStuck(false),
  _nearToVertex(false),
  _nearToBarycenter(false),
  _movingVertex(false),
  _movingPolygon(false),
  _lengthOn(true),
  _angleOn(false),
  _currPolygonRow(-1),
  _currVertexRow(-1),
  _beforeMovingVertexX(-1),
  _beforeMovingVertexY(-1),
  _beforeMovingPolygonX(-1),
  _beforeMovingPolygonY(-1),
  _currOldX(-1),
  _currOldY(-1) {

  resize(401, 401);

  setMouseTracking(true);

  connect(_controller, SIGNAL(update()), this, SLOT(drawFromModel()));
  connect(_controller, SIGNAL(updateReset()), this, SLOT(resetValues()));

  _newAction = new QAction("&New...", this);
  _newAction->setShortcut(QKeySequence::New);
  addAction(_newAction);
  connect(_newAction, SIGNAL(triggered()), this, SLOT(newFile(void)));

  _openAction = new QAction("&Open...", this);
  _openAction->setShortcut(QKeySequence::Open);
  addAction(_openAction);
  connect(_openAction, SIGNAL(triggered(void)), this, SLOT(openFile(void)));

  _saveAction = new QAction("&Save", this);
  _saveAction->setShortcut(QKeySequence::Save);
  addAction(_saveAction);
  connect(_saveAction, SIGNAL(triggered()), this, SLOT(saveFile(void)));

  _saveAsAction = new QAction("Save &as...", this);
  _saveAsAction->setShortcut(QKeySequence::SaveAs);
  addAction(_saveAsAction);
  connect(_saveAsAction, SIGNAL(triggered(void)), this, SLOT(saveAsFile(void)));

  _undoAction = _undoStack->createUndoAction(this, "&Undo");
  _undoAction->setShortcut(QKeySequence::Undo);
  addAction(_undoAction);

  _redoAction = _undoStack->createRedoAction(this, "&Redo");
  _redoAction->setShortcut(QKeySequence::Redo);
  addAction(_redoAction);

  _addPolygonAction = new QAction("&New poc::Polygon", this);
  _addPolygonAction->setShortcut(QKeySequence("CTRL+SHIFT+N"));
  addAction(_addPolygonAction);
  connect(_addPolygonAction, SIGNAL(triggered()), _controller, SLOT(appendPolygon()));

  _alignToGridAction = new QAction("&Align to grid", this);
  _alignToGridAction->setShortcut(QKeySequence("CTRL+I"));
  addAction(_alignToGridAction);
  connect(_alignToGridAction, SIGNAL(triggered()), _controller, SLOT(alignToGrid()));

  _onLengthAction = new QAction("Toggle length visible", this);
  _onLengthAction->setShortcut(QKeySequence("CTRL+SHIFT+ALT+L"));
  _onLengthAction->setCheckable(true);
  _onLengthAction->setChecked(_lengthOn);
  addAction(_onLengthAction);
  connect(_onLengthAction, SIGNAL(toggled(bool)), this, SLOT(toggleLength(bool)));

  _onAngleAction = new QAction("Toggle angle visible", this);
  _onAngleAction->setShortcut(QKeySequence("CTRL+SHIFT+ALT+A"));
  _onAngleAction->setCheckable(true);
  _onAngleAction->setChecked(_angleOn);
  addAction(_onAngleAction);
  connect(_onAngleAction, SIGNAL(toggled(bool)), this, SLOT(toggleAngle(bool)));
}

void LevelDesignerScribbleView::setModel(LevelDesignerModel* model) {
  _model = model;
}

void LevelDesignerScribbleView::mouseMoveEvent(QMouseEvent* event) {
  _movingVertex = (event->buttons() == Qt::LeftButton && _nearToVertex);
  _movingPolygon = (event->buttons() == Qt::LeftButton && _nearToBarycenter);

  if (event->buttons() == Qt::NoButton && _model->rowCount() > 0) {
    Point2d currPos(event->pos().x(), event->pos().y());

    QModelIndex currIndex = _selectionModel->currentIndex();
    if (!currIndex.isValid()) {
      qDebug() << "now";
      _selectionModel->setCurrentIndex(_model->index(0, 0), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
      currIndex = _selectionModel->currentIndex();
    }

    if (currIndex.column() != 0)
      currIndex = _model->index(currIndex.row(), 0, currIndex.parent());

    QString fstData = _model->itemData(currIndex).value(Qt::DisplayRole).toString();

    // This is a vertex
    if (!fstData.startsWith("Polygon")) {
      currIndex = currIndex.parent();
      // This is the header
    } else if (fstData == "Polygon") {
      currIndex = _model->index(0, 0);
      // This is a polygon
    } else if (fstData.startsWith("Polygon ")) {
    } else {
      qDebug() << currIndex << fstData;
      return;
    }

    poc::Polygon currPolygon = _model->polygonFromIndex(currIndex);
    std::vector<Point2d> vertices = currPolygon.getVertices();
    _nearToVertex = false;
    _nearToBarycenter = false;
    Point2d barycenter;
    if (_model->rowCount(currIndex) > 2) {
      //            qDebug() << "Compute polygon barycenter from LevelDesignerScribbleView::mouseMoveEvent";
      barycenter = currPolygon.barycenter();
      _nearToBarycenter = (Point2d::distance(currPos, barycenter) < 20);
    }
    for (unsigned int k = 0; k < vertices.size(); ++k) {
      Point2d vertex = vertices.at(k);
      if (Point2d::distance(currPos, vertex) < 10 || _nearToBarycenter) {
        _nearToVertex = !_nearToBarycenter;
        _currPolygonRow = currIndex.row();
        _currVertexRow = k;
        _currOldX = event->pos().x();
        _currOldY = event->pos().y();
        _beforeMovingVertexX = vertex.getX();
        _beforeMovingPolygonX = barycenter.getX();
        _beforeMovingVertexY = vertex.getY();
        _beforeMovingPolygonY = barycenter.getY();
        break;
      }
    }
    if (_nearToVertex || _nearToBarycenter)
      setCursor(Qt::OpenHandCursor);
    else
      setCursor(Qt::ArrowCursor);
  }

  if (event->buttons() == Qt::LeftButton && !_nearToVertex && _nearToBarycenter) {
    int newX = event->pos().x();
    int newY = event->pos().y();
    _controller->movePolygon(_currPolygonRow, _currOldX, _currOldY, newX, newY, false);
    _currOldX = newX;
    _currOldY = newY;
  }

  if (event->buttons() == Qt::LeftButton && _nearToVertex && !_nearToBarycenter) {
    int newX = event->pos().x();
    int newY = event->pos().y();
    _controller->moveVertex(_currPolygonRow, _currVertexRow, _currOldX, _currOldY, newX, newY, false);
    _currOldX = newX;
    _currOldY = newY;
  }

  if (event->buttons() == Qt::LeftButton && _isMagnetic) {
    QPoint globalPosition = QCursor::pos();

    const int oldX = globalPosition.x();
    int newX = oldX;
    const int oldY = globalPosition.y();
    int newY = oldY;

    int extraX = event->pos().x()%50;
    int extraY = event->pos().y()%50;

    int delta = 0;
    if (!_isStuck)
      delta = 3;

    if (extraX < delta)
      newX -= extraX - 5;
    else if (extraX > 50-delta)
      newX += 4 + 50-extraX;

    if (extraY < delta)
      newY -= extraY - 5;
    else if (extraY > 50-delta)
      extraY += 4 + 50-extraY;

    _isStuck = (newX != oldX || newY != oldY);

    QCursor::setPos(newX, newY);
  }
}

void LevelDesignerScribbleView::mouseReleaseEvent(QMouseEvent* event) {
  if (_model->rowCount() < 1)
    return;

  if (_nearToVertex || _nearToBarycenter)
    setCursor(Qt::OpenHandCursor);

  QModelIndex polygonIndex;
  QModelIndex vertexIndex;
  int vertexRow = -1;

  QModelIndex currIndex = _selectionModel->currentIndex();
  if (!currIndex.isValid()) {
    qDebug() << "right now";
    return;
  }

  if (currIndex.column() != 0)
    currIndex = _model->index(currIndex.row(), 0, currIndex.parent());

  QString fstData = _model->itemData(currIndex).value(Qt::DisplayRole).toString();

  // This is a vertex
  if (!fstData.startsWith("Polygon")) {
    vertexIndex = currIndex;
    polygonIndex = currIndex.parent();
    vertexRow = vertexIndex.row()+1;
    // This is the header
  } else if (fstData == "Polygon") {
    qDebug() << "There";
    _controller->addPolygon(_model->rowCount(), poc::Polygon());
    polygonIndex = _model->index(_model->rowCount()-1, 0);
    vertexRow = 0;
    // This is a polygon
  } else if (fstData.startsWith("Polygon ")) {
    polygonIndex = currIndex;
    vertexRow = _model->rowCount(polygonIndex);
  } else {
    qDebug() << "###" << fstData;
    return;
  }

  if (_movingVertex) {
    if (_beforeMovingVertexX != event->pos().x() || event->pos().y() != _beforeMovingVertexY) {
      _controller->moveVertex(_currPolygonRow, _currVertexRow, _beforeMovingVertexX, _beforeMovingVertexY, event->pos().x(), event->pos().y());
    }
    _beforeMovingVertexX = -1;
    _beforeMovingVertexY = -1;
    return;
  } else if (_movingPolygon) {
    poc::Polygon polygon = _model->polygonFromIndex(polygonIndex);
    //        qDebug() << "Compute polygon barycenter from LevelDesignerScribbleView::mouseReleaseEvent";
    Point2d barycenter = polygon.barycenter();
    if (barycenter.getX() != _beforeMovingPolygonX || barycenter.getY() != _beforeMovingPolygonY) {
      _controller->movePolygon(_currPolygonRow, barycenter.getX(), barycenter.getY(), _beforeMovingPolygonX, _beforeMovingPolygonY, false);
      _controller->movePolygon(_currPolygonRow, _beforeMovingPolygonX, _beforeMovingPolygonY, barycenter.getX(), barycenter.getY());
    }
    _beforeMovingPolygonX = -1;
    _beforeMovingPolygonY = -1;
    return;
  } else {
    _controller->addVertex(polygonIndex.row(), vertexRow, Point2d(event->pos().x(), event->pos().y()));
  }
}

void LevelDesignerScribbleView::mousePressEvent(QMouseEvent* event) {
  if (event->buttons() == Qt::LeftButton && _nearToVertex) {
    QPoint pos = mapToGlobal(QPoint(_beforeMovingVertexX, _beforeMovingVertexY));
    QCursor::setPos(pos);
    setCursor(Qt::ClosedHandCursor);
  }
}

void LevelDesignerScribbleView::paintEvent(QPaintEvent* event) {
  QPainter painter(this);
  QRect dirtyRect = event->rect();
  painter.drawImage(dirtyRect, _image, dirtyRect);
  if (_hasToDraw)
    drawFromModel();
}

void LevelDesignerScribbleView::resizeEvent(QResizeEvent* event) {
  if (width() > _image.width() || height() > _image.height()) {
    int newWidth = qMax(width() + 128, _image.width());
    int newHeight = qMax(height() + 128, _image.height());
    resizeImage(&_image, QSize(newWidth, newHeight));
    update();
  }
  QWidget::resizeEvent(event);
}

void LevelDesignerScribbleView::resizeImage(QImage* image, const QSize& newSize) {
  if (image->size() == newSize)
    return;

  QImage newImage(newSize, QImage::Format_RGB32);
  newImage.fill(qRgb(255, 255, 255));
  QPainter painter(&newImage);
  painter.drawImage(QPoint(0, 0), *image);
  *image = newImage;
}

void LevelDesignerScribbleView::setVertex(Point2d& vertex, const QModelIndex& vertexIndex, int value) const {
  if (vertexIndex.column() == 2)
    vertex.setX(value);
  else if (vertexIndex.column() == 3)
    vertex.setY(value);
  else
    qDebug() << "Error within LevelDesignerScribbleView::setVertex: column not editable.";
}

void LevelDesignerScribbleView::setSelectionModel(QItemSelectionModel* selectionModel) {
  _selectionModel = selectionModel;
  connect(_selectionModel, SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(currentChanged(QModelIndex,QModelIndex)));
}

void LevelDesignerScribbleView::drawPoint(const QPoint& point, const QColor& color) {
  QPainter painter(&_image);
  painter.setPen(QPen(color, PEN_WIDTH, Qt::SolidLine, Qt::RoundCap));
  painter.drawPoint(point);
}

void LevelDesignerScribbleView::drawLine(const QPoint& a, const QPoint& b, const QColor& color) {
  QPainter painter(&_image);
  painter.setPen(QPen(color, PEN_WIDTH/2));
  painter.drawLine(a, b);
}

void LevelDesignerScribbleView::drawText(const QString& text, const QPoint& position, int weight) {
  QPainter painter(&_image);
  painter.setFont(QFont("", 12, weight));
  painter.drawText(position, text);
}

void LevelDesignerScribbleView::drawGrid(void) {
  QPainter painter(&_image);

  int xMin = 0, xMax = 401;
  int yMin = 0, yMax = 401;
  int subSubCaseSize = 10;
  painter.setPen(QPen(Qt::lightGray, 1, Qt::DashLine));
  for (int i = xMin; i <= xMax; i += subSubCaseSize) {
    painter.drawLine(QPoint(i, yMin), QPoint(i, yMax));
  }
  for (int j = yMin; j <= yMax; j += subSubCaseSize) {
    painter.drawLine(QPoint(xMin, j), QPoint(xMax, j));
  }

  int subCaseSize = 50;
  painter.setPen(QPen(Qt::gray, 1, Qt::SolidLine));
  for (int i = xMin; i <= xMax; i += subCaseSize) {
    painter.drawLine(QPoint(i, yMin), QPoint(i, yMax));
  }
  for (int j = yMin; j <= yMax; j += subCaseSize) {
    painter.drawLine(QPoint(xMin, j), QPoint(xMax, j));
  }

  int caseSize = 100;
  painter.setPen(QPen(Qt::darkGray, 1));
  for (int i = xMin; i <= xMax; i += caseSize) {
    painter.drawLine(QPoint(i, yMin), QPoint(i, yMax));
  }
  for (int j = yMin; j <= yMax; j += caseSize) {
    painter.drawLine(QPoint(xMin, j), QPoint(xMax, j));
  }

}

void LevelDesignerScribbleView::drawPolygon(const QModelIndex& polygonIndex, const QModelIndex& vertexIndex, int value, const QColor& color) {
  if (!polygonIndex.isValid()) {
    qDebug() << "Polygon index invalid";
    return;
  }

  if (_model->rowCount() < 1) {
    qDebug() << "No polygon to draw";
    return;
  }

  int vertexCount = _model->rowCount(polygonIndex);
  for (int i = 0; i < vertexCount; i++) {
    QModelIndex vertexIndex1 = _model->index(i, 0, polygonIndex);
    QModelIndex vertexIndex2 = _model->index((i+1)%(vertexCount), 0, polygonIndex);

    Point2d fstVertex(_model->vertexFromIndex(vertexIndex1));
    Point2d sndVertex(_model->vertexFromIndex(vertexIndex2));

    if (value != -1) {
      if (vertexIndex1.internalId() == vertexIndex.internalId()) {
        setVertex(fstVertex, vertexIndex, value);
      } else if (vertexIndex2.internalId() == vertexIndex.internalId()) {
        setVertex(sndVertex, vertexIndex, value);
      }
    }

    QPoint a(fstVertex.getX(), fstVertex.getY());
    QPoint b(sndVertex.getX(), sndVertex.getY());

    QString vertexName = model()->data(model()->index(i, 1, polygonIndex), Qt::DisplayRole).toString();
    if (color != NOT_SELECTED_COLOR)
      drawText(vertexName, a+QPoint(3, 3));

    drawPoint(a);
    drawLine(a, b, color);

    if (_lengthOn && vertexCount > 1) {
      QString length = QString::number(Point2d::distance(fstVertex, sndVertex), 'g', 4);
      Point2d middle = Point2d::middle(fstVertex, sndVertex);
      QPoint lengthPos(middle.getX(), middle.getY());
      drawText(length, lengthPos, 75);
    }

    if (_angleOn && vertexCount > 2) {
      int prevIndex = (i-1)%(vertexCount);
      if (prevIndex < 0)
        prevIndex += vertexCount;

      Point2d prevVertex = _model->vertexFromIndex(_model->index(prevIndex, 0, polygonIndex));
      Vector2d AB(fstVertex, prevVertex);
      Vector2d AC(fstVertex, sndVertex);
      QString angle = QString::number(std::abs((Vector2d::angle(Vector2d(fstVertex, prevVertex), Vector2d(fstVertex, sndVertex))) * 180 / M_PI), 'g', 4);

      Vector2d AD = (AB.normalized() + AC.normalized()) * 20.f;
      Point2d D = fstVertex.applyVector(AD);

      drawText(angle, QPoint(D.getX(), D.getY()), 75);
    }
  }

  if (polygonIndex.isValid() && _model->rowCount(polygonIndex) > 2) {
    poc::Polygon polygon(_model->polygonFromIndex(polygonIndex));
    //        qDebug() << "Compute polygon barycenter from LevelDesignerScribbleView::drawPolygon";
    Point2d barycenter(polygon.barycenter());
    drawPoint(QPoint(barycenter.getX(), barycenter.getY()), color);
  }
}

void LevelDesignerScribbleView::drawFromModel(const QModelIndex& vertexIndex, int value) {
  clearImage();
  drawGrid();

  QModelIndex currIndex = _selectionModel->currentIndex();

  int polygonSelectedNumber = 0;
  for (int k = 0; k < _model->rowCount(); k++) {
    QModelIndex polygonIndex = _model->index(k, 0);
    if (polygonIndex.internalId() == currIndex.internalId()) {
      polygonSelectedNumber = k;
      break;
    }
    for (int i = 0; i < _model->rowCount(polygonIndex); i++) {
      QModelIndex vertexIndex = _model->index(i, 0, polygonIndex);
      if (vertexIndex.internalId() == currIndex.internalId()) {
        polygonSelectedNumber = k;
        break;
      }
    }
  }

  // Draw every polygon not selected
  for (int k = 0; k < _model->rowCount(); k++) {
    QModelIndex polygonIndex = _model->index(k, 0);

    if (k != polygonSelectedNumber)
      drawPolygon(polygonIndex, vertexIndex, value);
  }
  // Draw selected polygon
  QModelIndex polygonIndex = _model->index(polygonSelectedNumber, 0);
  drawPolygon(polygonIndex, vertexIndex, value, polygonIndex.data(Qt::DecorationRole).value<QColor>());

  _hasToDraw = false;
  update();
}

void LevelDesignerScribbleView::clearImage(void) {
  _image.fill(Qt::white);
}

//void LevelDesignerScribbleView::newPolygon(void) {
//    _model->appendPolygon(poc::Polygon());
//    QModelIndex currIndex = _model->index(_model->rowCount()-1, 0);
//    _selectionModel->setCurrentIndex(currIndex, QItemSelectionModel::ClearAndSelect);
//}

void LevelDesignerScribbleView::updateViewNotModel(QModelIndex vertexIndex, int value) {
  drawFromModel(vertexIndex, value);
}

void LevelDesignerScribbleView::currentChanged(QModelIndex currentIndex, QModelIndex previousIndex) {
  if ((!currentIndex.parent().isValid()) || (currentIndex.parent().isValid() && currentIndex.parent() != previousIndex.parent()))
    drawFromModel();
}

void LevelDesignerScribbleView::resetValues(void) {
  setCursor(Qt::ArrowCursor);
  _isStuck = false;
  _nearToVertex = false;
  _nearToBarycenter = false;
  _movingVertex = false;
  _movingPolygon = false;
  _currPolygonRow = -1;
  _currVertexRow = -1;
  _beforeMovingVertexX = -1;
  _beforeMovingVertexY = -1;
  _beforeMovingPolygonX = -1;
  _beforeMovingPolygonY = -1;
  _currOldX = -1;
  _currOldY = -1;
}

void LevelDesignerScribbleView::newFile(void) {
  if (isVisible())
    _controller->newFile(this);
}

void LevelDesignerScribbleView::openFile(void) {
  if (isVisible())
    _controller->openFile(this);
}

void LevelDesignerScribbleView::saveFile(void) {
  if (isVisible())
    _controller->saveFile(this);
}

void LevelDesignerScribbleView::saveAsFile(void) {
  if (isVisible())
    _controller->saveAsFile(this);
}

void LevelDesignerScribbleView::toggleLength(bool b) {
  _lengthOn = b;
  drawFromModel();
}

void LevelDesignerScribbleView::toggleAngle(bool b) {
  _angleOn = b;
  drawFromModel();
}
