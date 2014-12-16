#ifndef POLYGONSCRIBBLEVIEW_H
#define POLYGONSCRIBBLEVIEW_H

#include <QtWidgets>

#include "LevelDesignerModel.h"
#include "LevelDesignerController.h"

class LevelDesignerScribbleView: public QWidget {
  Q_OBJECT

public:
  enum MainColors {
    Beige = 0xFAD7B9,
    Blue1 = 0x3990D5,
    Blue2 = 0x3AADE2,
    Blue3 = 0x64B4C6,
    Blue4 = 0x0D20B0,
    Blue5 = 0x4A90D9,
    Blue6 = 0x3665BD,
    Blue7 = 0x1C97E4,
    Blue8 = 0x1993DB,
    Blue9 = 0x1043A7,
    Blue10 = 0x509CC2,
    Brown = 0x97824F,
    Gold = 0xC4B028,
    Gray1 = 0x434343,
    Gray2 = 0x626262,
    Gray3 = 0x818181,
    Green1 = 0x0CB436,
    Green2 = 0x459129,
    Green3 = 0x5DBE14,
    Green4 = 0x01A20D,
    Green5 = 0x069224,
    Green6 = 0x36B62C,
    Green7 = 0xA5DB5E,
    Green8 = 0x06A235,
    Green9 = 0x3EA14F,
    Orange1 = 0xE88009,
    Orange2 = 0xEA7B03,
    Orange3 = 0xE54719,
    orange4 = 0xDB5D0D,
    Pink1 = 0xE6899C,
    Pink2 = 0xE391DF,
    Pink3 = 0xF6038B,
    Purple1 = 0xA30FAD,
    Purple2 = 0x8B23C3,
    Purple3 = 0xCA07F4,
    Red1 = 0xE04A1D,
    Red2 = 0xC81214,
    Red3 = 0x9A1D0C,
    Salmon = 0xD28A74,
    Violet = 0x9667DA,
    Yellow1 = 0xD6C742,
    Yellow2 = 0xF8F252,
    Yellow3 = 0xD7E71C
  };

  LevelDesignerScribbleView(LevelDesignerController* controller, QWidget* parent = 0);

  LevelDesignerModel* model() const { return _model; }
  void setModel(LevelDesignerModel *model);
  void setSelectionModel(QItemSelectionModel* selectionModel);

protected:
  virtual void mouseMoveEvent(QMouseEvent* event);
  virtual void mouseReleaseEvent(QMouseEvent* event);
  virtual void mousePressEvent(QMouseEvent*event);
  virtual void paintEvent(QPaintEvent* event);
  virtual void resizeEvent(QResizeEvent* event);
  void resizeImage(QImage* image, const QSize& newSize);
  void setVertex(Point2d& vertex, const QModelIndex& vertexIndex, int value) const;

public slots:
  void drawPoint(const QPoint& point, const QColor& color = QColor(0x2B74CF));
  void drawLine(const QPoint& a, const QPoint& b, const QColor &color);
  void drawText(const QString& text, const QPoint& position, int weight = 50);
  void drawGrid(void);
  void drawFromModel(const QModelIndex& vertexIndex = QModelIndex(), int value = -1);
  void drawPolygon(const QModelIndex& polygonIndex, const QModelIndex& vertexIndex, int value, const QColor& color = NOT_SELECTED_COLOR);
  void clearImage(void);
  //void newPolygon(void);
  void updateViewNotModel(QModelIndex vertexIndex, int value);
  void currentChanged(QModelIndex, QModelIndex previousIndex);
  void setMagnetism(bool b) { _isMagnetic = b; }
  void resetValues(void);
  void newFile(void);
  void openFile(void);
  void saveFile(void);
  void saveAsFile(void);
  void toggleLength(bool);
  void toggleAngle(bool);

protected:
  LevelDesignerModel* _model;
  QImage _image;
  bool _hasToDraw;
  QItemSelectionModel* _selectionModel;
  LevelDesignerController* _controller;
  QUndoStack* _undoStack;

  QAction* _newAction;
  QAction* _openAction;
  QAction* _saveAction;
  QAction* _saveAsAction;
  QAction* _undoAction;
  QAction* _redoAction;
  QAction* _addPolygonAction;
  QAction* _alignToGridAction;
  QAction* _onLengthAction;
  QAction* _onAngleAction;

  bool _isMagnetic;
  bool _isStuck;
  bool _nearToVertex;
  bool _nearToBarycenter;
  bool _movingVertex;
  bool _movingPolygon;
  bool _lengthOn;
  bool _angleOn;
  int _currPolygonRow;
  int _currVertexRow;
  int _beforeMovingVertexX;
  int _beforeMovingVertexY;
  int _beforeMovingPolygonX;
  int _beforeMovingPolygonY;
  int _currOldX;
  int _currOldY;

private:
  static const int PEN_WIDTH;
  static const QColor NOT_SELECTED_COLOR;
};

#endif // POLYGONSCRIBBLEVIEW_H
