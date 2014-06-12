#include "LevelDesignerGameStatView.h"

LevelDesignerGameStatView::LevelDesignerGameStatView(LevelDesignerController* controller, QWidget* parent):
  QWidget(parent),
  _model(0),
  _controller(controller) {

  // Lines count
  _linesCountSpinBox = new QSpinBox;
  _linesCountSpinBox->setRange(1, 49);
  _linesCountSpinBox->setEnabled(false);
  QFormLayout* linesCountLayout = new QFormLayout;
  linesCountLayout->addRow("Lines", _linesCountSpinBox);

  // Parts count
  _partsCountSpinBox = new QSpinBox;
  _partsCountSpinBox->setRange(2, 50);
  _partsCountSpinBox->setEnabled(false);
  QFormLayout* partsCountLayout = new QFormLayout;
  partsCountLayout->addRow("Parts", _partsCountSpinBox);

  // Max gap to win
  _maxGapToWinSlider = new QSlider(Qt::Horizontal);
  _maxGapToWinSlider->setRange(0, 100);
  _maxGapToWinSlider->setToolTip("0: hard. 100: trivial");
  _maxGapToWinSlider->setSliderPosition(10);
  _maxGapToWinSlider->setEnabled(false);
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

  connect(_controller, SIGNAL(updateStats()), this, SLOT(updateFields()));
  connect(_controller, SIGNAL(updateReset()), this, SLOT(resetFields()));
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

void LevelDesignerGameStatView::updateFields(void) {
  disconnect(_linesCountSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateLinesCount(int)));
  disconnect(_partsCountSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updatePartsCount(int)));
  disconnect(_maxGapToWinSlider, SIGNAL(valueChanged(int)), this, SLOT(updateMaxGapToWin(int)));
  _linesCountSpinBox->setValue(_model->getLinesCount());
  _partsCountSpinBox->setValue(_model->getPartsCount());
  _maxGapToWinSlider->setValue(_model->getMaxGapToWin());
  connect(_linesCountSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateLinesCount(int)));
  connect(_partsCountSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updatePartsCount(int)));
  connect(_maxGapToWinSlider, SIGNAL(valueChanged(int)), this, SLOT(updateMaxGapToWin(int)));
  _linesCountSpinBox->setEnabled(true);
  _partsCountSpinBox->setEnabled(true);
  _maxGapToWinSlider->setEnabled(true);
}

void LevelDesignerGameStatView::resetFields(void) {
  disconnect(_linesCountSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateLinesCount(int)));
  disconnect(_partsCountSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updatePartsCount(int)));
  disconnect(_maxGapToWinSlider, SIGNAL(valueChanged(int)), this, SLOT(updateMaxGapToWin(int)));
  _linesCountSpinBox->setValue(1);
  _partsCountSpinBox->setValue(2);
  _maxGapToWinSlider->setValue(10);
  connect(_linesCountSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateLinesCount(int)));
  connect(_partsCountSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updatePartsCount(int)));
  connect(_maxGapToWinSlider, SIGNAL(valueChanged(int)), this, SLOT(updateMaxGapToWin(int)));
  _linesCountSpinBox->setEnabled(false);
  _partsCountSpinBox->setEnabled(false);
  _maxGapToWinSlider->setEnabled(false);
}
