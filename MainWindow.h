#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>

#include "Segment.h"
#include "LevelDesignerController.h"
#include "LevelDesignerModel.h"
#include "LevelDesignerTreeView.h"
#include "LevelDesignerScribbleView.h"
#include "LevelDesignerGameStatView.h"
#include "Commands.h"
#include "GameController.h"
#include "GameView.h"
#include "GameModel.h"
#include "HomeMenu.h"
#include "LevelSelector.h"

class MainWindow: public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = NULL);

public slots:
  //bool confirmErase(void);
  //void initNewDocument(bool neverSavedBefore, const QString& fileName = "");
  //void updateSavingState(int index);
  //void newFile(void);
  //void clear(void);
  void openFile(void);
  //void saveFile(void);
  //void saveAsFile(void);
  //void saveAndUpdate(void);
  void addPolygon(void);
  //void alignToGrid(void);
  //void updateMenuActions(int);
  //void updateCurrentController(int currentIndex);
  void switchView(int);
  void switchLevel(int);

private:
  AbstractController* _currentController;

  QUndoStack* _undoStack;
  QUndoView* _undoView;

  LevelDesignerController* _levelDesignerController;
  LevelDesignerTreeView* _levelDesignerTreeView;
  LevelDesignerScribbleView* _levelDesignerScribbleView;
  LevelDesignerGameStatView* _levelDesignerGameStatView;
  LevelDesignerModel* _levelDesignerModel;

  GameController* _gameController;
  GameView* _gameView;
  GameModel* _gameModel;

  HomeMenu* _homeMenu;
  LevelSelector* _levelSelector;

  QTabWidget* _tabWidget;
  //QStackedWidget* _stackWidget;
  QWidget* _levelDesignerWidget;

  QAction* _newAction;
  QAction* _openAction;
  QAction* _saveAction;
  QAction* _saveAsAction;
  QAction* _undoAction;
  QAction* _redoAction;
  QAction* _addPolygonAction;
  QAction* _magnetismAction;
  QAction* _alignToGridAction;
};

#endif // MAINWINDOW_H
