#ifndef TESTLEVELVIEW_H
#define TESTLEVELVIEW_H

#include <QtWidgets>

#include "GameModel.h"
#include "TestLevelController.h"

class TestLevelView: public QWidget
{
  Q_OBJECT
public:
  explicit TestLevelView(TestLevelController* controller, QWidget* parent = nullptr);
  ~TestLevelView();

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
  void drawLine(const QPoint& begin, const QPoint& end, const QColor &color = Qt::black, Qt::PenStyle penStyle = Qt::SolidLine);
  void drawText(const QPoint& position, const QString& text, const QColor &color);

public slots:
  void drawFromModel(void);
  void clearImage(void);
//  void clearImage(void);
//  void currentChanged(QModelIndex currentIndex, QModelIndex previousIndex);

signals:
  void newLineDrawn(Segment);
//  void boundaries(QPoint, QPoint);
//  void moving(QPoint);

protected:
  bool _scribbling;
  QImage _image;
  QPoint _firstPoint;
  QPoint _lastPoint;
  bool _goodSegment;
  bool _runningSegment;
  GameModel* _model;
  TestLevelController* _controller;


};

#endif // TESTLEVELVIEW_H
