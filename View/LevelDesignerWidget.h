#ifndef LEVELDESIGNERWIDGET_H
#define LEVELDESIGNERWIDGET_H

#include <QtWidgets>

#include "LevelDesignerTreeView.h"
#include "LevelDesignerScribbleView.h"
#include "LevelDesignerGameStatView.h"

class LevelDesignerWidget: QWidget {
public:
  LevelDesignerWidget(LevelDesignerController* controller, QWidget* parent = 0);

private:
  LevelDesignerController* _controller;
  LevelDesignerTreeView* _levelDesignerTreeView;
  LevelDesignerScribbleView* _levelDesignerScribleView;
  LevelDesignerGameStatView* _levelDesignerGameStatView;
};

#endif // LEVELDESIGNERWIDGET_H
