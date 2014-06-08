#ifndef LEVELDESIGNERGAMESTATVIEW_H
#define LEVELDESIGNERGAMESTATVIEW_H

#include <QtWidgets>

#include "LevelDesignerModel.h"
#include "LevelDesignerController.h"

class LevelDesignerGameStatView: public QWidget {
  Q_OBJECT

public:
  LevelDesignerGameStatView(LevelDesignerController* controller, QWidget* parent = 0);

  LevelDesignerModel* model() const { return _model; }
  void setModel(LevelDesignerModel *model);

public slots:
  void updateLinesCount(int linesCount);
  void updatePartsCount(int partsCount);
  void updateMaxGapToWin(int maxGapToWin);

protected:
  LevelDesignerModel* _model;
  LevelDesignerController* _controller;
  QSpinBox* _linesCountSpinBox;
  QSpinBox* _partsCountSpinBox;
  QSlider* _maxGapToWinSlider;
};

#endif // LEVELDESIGNERGAMESTATVIEW_H
