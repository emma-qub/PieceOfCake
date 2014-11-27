#include "LevelDesignerWidget.h"

LevelDesignerWidget::LevelDesignerWidget(LevelDesignerController* controller, QWidget* parent):
  QWidget(parent),
  _controller(controller) {

  _levelDesignerTreeView = new LevelDesignerTreeView(_controller);

  QHBoxLayout* contentLayout = new QHBoxLayout;
  contentLayout->addWidget(_levelDesignerTreeView);
  contentLayout->addWidget(_levelDesignerScribleView);
  contentLayout->addWidget(_levelDesignerGameStatView);

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addLayout(contentLayout);
}
