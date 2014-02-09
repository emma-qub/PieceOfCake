#include "SliceArea.h"

#include <QPropertyAnimation>

SliceArea::SliceArea(QWidget* parent) :
    QWidget(parent),
    _scribbling(false),
    _myPenWidth(7),
    _myPenColor(Qt::black),
    _image(QImage("../SliceIt/design/polygonBackground.png")),
    _firstPoint(),
//    _lastPoint(),
    _canErase(false),
    _polygons(),
    _goodSegment(false) {

    setAttribute(Qt::WA_StaticContents);
    setMouseTracking(true);

    setStyleSheet("background:transparent;");
//    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint);

    setFixedSize(381, 441);
}

void SliceArea::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton && !_scribbling) {
        _scribbling = true;
        _firstPoint = event->pos();
    }
}

void SliceArea::mouseMoveEvent(QMouseEvent* event) {
    emit position(event->pos().x(), event->pos().y());

    if (_scribbling) {
        QPoint currPoint(event->pos());

        clearImage();
        drawPolygon();

        Point2d firstPoint(_firstPoint.x(), _firstPoint.y());

        Segment line(firstPoint, Point2d(currPoint.x(), currPoint.y()));
        QColor color(QColor(0x639C45));

        _goodSegment = true;

        for (const Polygon& polygon: _polygons) {
            if (!polygon.isCrossing(line) && !polygon.isPointInside(firstPoint)) {
                color = QColor(Qt::gray);
//                _goodSegment = false;
            } else if (!polygon.isGoodSegment(line)) {
                color = QColor(QColor(0xB6492C));
//                _goodSegment = false;
            }
        }

        drawLine(_firstPoint, currPoint, color);
    }
}

void SliceArea::mouseReleaseEvent(QMouseEvent* event) {
    if (_scribbling) {
        clearImage();
        drawPolygon();
        if (_goodSegment) {
            drawLineTo(event->pos());
            emit boundaries(_firstPoint, event->pos());
        }
        _scribbling = false;
    }
}

void SliceArea::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, _image, dirtyRect);
//    painter.setPen(QColor(Qt::darkGray));

//    for (int k = 0; k <= 380; k += 20) {
//        painter.drawLine(QPoint(k, 0), QPoint(k, 440));
//    }

//    for (int k = 0; k <= 440; k += 20) {
//        painter.drawLine(QPoint(0, k), QPoint(380, k));
//    }
}

void SliceArea::resizeEvent(QResizeEvent* event) {
    if (width() > _image.width() || height() > _image.height()) {
        int newWidth = qMax(width() + 128, _image.width());
        int newHeight = qMax(height() + 128, _image.height());
        resizeImage(&_image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}

void SliceArea::resizeImage(QImage* image, const QSize& newSize) {
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}

void SliceArea::drawLineTo(const QPoint& endPoint) {
    drawLine(_firstPoint, endPoint, _myPenColor);
}

void SliceArea::drawLine(const QPoint& begin, const QPoint& end, const QColor& color) {
    QPainter painter(&_image);
    painter.setPen(QPen(color, _myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(begin, end);
    painter.setPen(QPen(QColor(Qt::black), 10, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

//    painter.drawText(begin, "("+QString::number(begin.x())+"; "+QString::number(begin.y())+")");
//    painter.drawText(end, "("+QString::number(end.x())+"; "+QString::number(end.y())+")");

    int rad = (1 / 2) + 2;
    update(QRect(begin, end).normalized().adjusted(-rad, -rad, +rad, +rad));
}

void SliceArea::drawPolygon(void) {
//    std::cerr << _polygons.size() << std::endl;

    for (const Polygon& polygon: _polygons) {
        std::vector<Point2d> vertices = polygon.getVertices();
        for (unsigned k = 0; k < vertices.size(); k++) {
            Point2d fstVertex(vertices.at(k));
            Point2d sndVertex(vertices.at((k+1)%vertices.size()));

//            drawLine(QPoint(fstVertex.getX(), fstVertex.getY()), QPoint(sndVertex.getX(), sndVertex.getY()), QColor(0x77B5FE));
            drawLine(QPoint(fstVertex.getX(), fstVertex.getY()), QPoint(sndVertex.getX(), sndVertex.getY()), QColor(0x2B74CF));
        }
    }
}

void SliceArea::drawText(const QPoint& position, const QString& text, const QColor &color) {
    QPainter painter(&_image);
    painter.setPen(QPen(color, _myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.setFont(QFont("", 3));
    painter.drawText(position, text);
}

void SliceArea::clearImage(void) {
    _image = QImage("../SliceIt/design/polygonBackground.png");
    QPainter painter(&_image);

    painter.setPen(QColor(Qt::darkGray));

    for (int k = 0; k <= 380; k += 20) {
        painter.drawLine(QPoint(k, 0), QPoint(k, 440));
    }

    for (int k = 0; k <= 440; k += 20) {
        painter.drawLine(QPoint(0, k), QPoint(380, k));
    }

    /*
    QBrush brush(Qt::green, QPixmap("../SliceIt/design/texturePattern.png"));
//    brush.setTexture(QPixmap("../SliceIt/design/texturePattern.png"));
//    painter.setBrush(brush);
    painter.setPen(QPen(brush, 18, Qt::TexturePattern));
    painter.drawRect(300, 400, 200, 50);
//    painter.drawText(400, 420, "Ok");*/

    update();
}
