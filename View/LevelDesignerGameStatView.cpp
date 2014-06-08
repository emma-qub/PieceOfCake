#include "LevelDesignerGameStatView.h"

LevelDesignerGameStatView::LevelDesignerGameStatView(LevelDesignerController* controller, QWidget* parent):
  QWidget(parent),
  _model(0),
  _controller(controller) {

  // Lines count
  _linesCountSpinBox = new QSpinBox;
  _linesCountSpinBox->setRange(1, 49);
  QFormLayout* linesCountLayout = new QFormLayout;
  linesCountLayout->addRow("Lines", _linesCountSpinBox);

  // Parts count
  _partsCountSpinBox = new QSpinBox;
  _partsCountSpinBox->setRange(2, 50);
  QFormLayout* partsCountLayout = new QFormLayout;
  partsCountLayout->addRow("Parts", _partsCountSpinBox);

  // Max gap to win
  _maxGapToWinSlider = new QSlider(Qt::Horizontal);
  _maxGapToWinSlider->setRange(0, 100);
  _maxGapToWinSlider->setToolTip("0: hard. 100: trivial");
  _maxGapToWinSlider->setSliderPosition(10);
  QFormLayout* maxGapToWinLayout = new QFormLayout;
  maxGapToWinLayout->addRow("Difficulty", _maxGapToWinSlider);

  QVBoxLayout* mainLayout = new QVBoxLayout(this);
  mainLayout->addLayout(linesCountLayout);
  mainLayout->addLayout(partsCountLayout);
  mainLayout->addLayout(maxGapToWinLayout);
  mainLayout->setAlignment(Qt::AlignTop);

  // Connections
  connect(_linesCountSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateLinesCount(int)));
  connect(_partsCountSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updatePartsCount(int)));
  connect(_maxGapToWinSlider, SIGNAL(valueChanged(int)), this, SLOT(updateMaxGapToWin(int)));

  // Initialize model through controller
  _controller->updateLinesCount(_linesCountSpinBox->value());
  _controller->updatePartsCount(_partsCountSpinBox->value());
  _controller->updateMaxGapToWin(_maxGapToWinSlider->value());
}

void LevelDesignerGameStatView::setModel(LevelDesignerModel* model) {
  _model = model;
}

void LevelDesignerGameStatView::updateLinesCount(int linesCount) {
  _controller->updateLinesCount(linesCount);
}

void LevelDesignerGameStatView::updatePartsCount(int partsCount) {
  _controller->updatePartsCount(partsCount);
}

void LevelDesignerGameStatView::updateMaxGapToWin(int maxGapToWin) {
  _controller->updateMaxGapToWin(maxGapToWin);
}
