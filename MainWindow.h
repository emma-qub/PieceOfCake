#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QQuickItem>
#include <QQuickView>
#include <QQuickWidget>
#include <QQmlContext>

// Core
#include "Segment.h"

// Levels Designer
#include "LevelDesignerController.h"
#include "LevelDesignerModel.h"
#include "LevelDesignerTreeView.h"
#include "LevelDesignerScribbleView.h"
#include "Commands.h"

// Game
#include "GameController.h"
#include "GameView.h"
#include "GameModel.h"

// Test Level
#include "TestLevelController.h"
#include "TestLevelView.h"

class MainWindow: public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = NULL);

public slots:
  void openLevel(QString levelName);
  void hideWidgets(void);
  void showCreateLevel(void);
  void showTestLevel(void);
  void showSaveLevel(bool);
  void updateQMLView(void);

private:
  void initHome(void);
  void initGame(void);
  void initLevelDesigner(void);
  void initTestLevel(void);

private:
  AbstractController* _currentController;

  QUndoStack* _levelDesignerUndoStack;
  QUndoStack* _gameUndoStack;
  QUndoView* _undoView;

  LevelDesignerController* _levelDesignerController;
  LevelDesignerTreeView* _levelDesignerTreeView;
  LevelDesignerScribbleView* _levelDesignerScribbleView;
  LevelDesignerModel* _levelDesignerModel;

  GameController* _gameController;
  GameView* _gameView;
  GameModel* _gameModel;

  TestLevelView* _testLevelView;
  TestLevelController* _testLevelController;
  GameModel* _testLevelModel;

  QWidget* _gameWidget;
  QWidget* _levelDesignerWidget;
  QQuickWidget* _selectLevelWidget;
  QQuickWidget* _homeWidget;
};

#endif // MAINWINDOW_H
