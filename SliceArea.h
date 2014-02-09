#ifndef SLICEAREA_H
#define SLICEAREA_H

#include <QtWidgets>

#include "Point2d.h"
#include "Segment.h"
#include "Vector2d.h"
#include "Polygon.h"

class SliceArea : public QWidget {
    Q_OBJECT

public:
    SliceArea(QWidget* parent = NULL);

    inline void setPolygons(const std::vector<Polygon>& polygons) { _polygons = polygons; }
    inline void addPolygon(const Polygon& polygon) { _polygons.push_back(polygon); }

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void paintEvent(QPaintEvent* event);
    void resizeEvent(QResizeEvent* event);

public:
    void resizeImage(QImage* image, const QSize& newSize);
    void drawLineTo(const QPoint& endPoint);
    void drawLine(const QPoint& begin, const QPoint& end, const QColor &color = Qt::black);
    void drawPolygon(void);
    void drawText(const QPoint& position, const QString& text, const QColor &color = QColor(0x777700));

public slots:
    void clearImage(void);

signals:
    void boundaries(QPoint, QPoint);
    void moving(QPoint);
    void position(int, int);

protected:
    bool _scribbling;
    int _myPenWidth;
    QColor _myPenColor;
    QImage _image;
    QPoint _firstPoint;
//    QPoint _lastPoint;
    bool _canErase;
    std::vector<Polygon> _polygons;
    bool _goodSegment;
};

#endif // SLICEAREA_H
