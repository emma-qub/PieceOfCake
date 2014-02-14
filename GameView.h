#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QtWidgets>

#include "GameModel.h"
#include "GameController.h"

class GameView : public QWidget {
    Q_OBJECT

public:
    GameView(GameController* controller, QWidget* parent = NULL);

    inline GameModel* model() const { return _model; }
    void setModel(GameModel* model);

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
    bool _canErase;
    bool _goodSegment;
    GameModel* _model;
    GameController* _controller;
};

#endif // GAMEVIEW_H