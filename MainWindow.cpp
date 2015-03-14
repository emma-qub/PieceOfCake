#include "MainWindow.h"

#include "Segment.h"
#include "Point2d.h"
#include "Vector2d.h"
#include "ParserXML.h"

#include <iostream>
#include <fstream>
#include <iomanip>

#include <QQmlEngine>
#include <QQmlProperty>
#include <QtQml>

#define cerro(x) std::cerr << x << std::endl;

MainWindow::MainWindow(QWidget* parent):
  QMainWindow(parent) {

  // Create undo stack to manage undo/redo actions in level designer
  _levelDesignerUndoStack = new QUndoStack;

  // Create undo stack to manage undo/redo actions in game
  _gameUndoStack = new QUndoStack;

  initHome();

  // Set central widget
  setCentralWidget(_homeWidget);

  initGame();
  initLevelDesigner();
  initTestLevel();

  // Change window title
  setWindowTitle("Graphical tests");

  // Resize
  setFixedSize(1200, 756);
}

void MainWindow::openLevel(QString levelName) {
  _gameView->show();
  // Force the widget to draw
  QApplication::processEvents();
  _gameController->openLevel("../PieceOfCake/"+levelName);
}

void MainWindow::hideWidgets(void) {
  _levelDesignerTreeView->hide();
  _levelDesignerScribbleView->hide();
  _gameView->hide();
  _testLevelView->hide();
}

void MainWindow::showCreateLevel(void) {
  hideWidgets();
  _levelDesignerTreeView->show();
  _levelDesignerScribbleView->show();
}

void MainWindow::showTestLevel(void) {
  hideWidgets();
  _testLevelView->show();
  _testLevelModel->setPolygonList(_levelDesignerModel->getPolygonList());
  _testLevelView->drawFromModel();
  std::cerr << "TEST LEVEL REQUESTED" << std::endl;
}

void MainWindow::showSaveLevel(bool b) {
  if (b) {
    hideWidgets();
  } else {
    cerro("Improve your cutting skills.");
  }
  std::cerr << "SAVE LEVEL REQUESTED" << std::endl;
}

void MainWindow::updateQMLView(void) {
  QQuickItem* item = _homeWidget->rootObject();
//  std::cerr << item << std::endl;
  item->setProperty("state", "green");
}

void MainWindow::initHome(void) {
  _homeWidget = new QQuickWidget(QUrl::fromLocalFile("../PieceOfCake/main.qml"));

  QQuickItem* item = _homeWidget->rootObject();
  connect(item, SIGNAL(openLevelRequested(QString)), this, SLOT(openLevel(QString)));
  connect(item, SIGNAL(homePageRequested(void)), this, SLOT(hideWidgets(void)));
  connect(item, SIGNAL(createLevelRequested(void)), this, SLOT(showCreateLevel(void)));
  connect(item, SIGNAL(backToLevelsRequested(void)), this, SLOT(hideWidgets(void)));
  connect(item, SIGNAL(testLevelRequested(void)), this, SLOT(showTestLevel(void)));
}

void MainWindow::initGame(void) {
  // Game model
  _gameModel = new GameModel;
  // Game controller
  _gameController = new GameController(_gameModel, _gameUndoStack, this);
  // Game view
  _gameView = new GameView(_gameController, this);
  _gameView->setModel(_gameModel);
  _gameView->move(175, 150);
  _gameView->hide();

  connect(_gameController, SIGNAL(update()), this, SLOT(updateQMLView()));

  _homeWidget->rootContext()->setContextProperty("gameInfo", _gameController->getGameInfo());
  _homeWidget->engine()->addImportPath("../PieceOfCake");

  QQuickItem* item = _homeWidget->rootObject();
  connect(item, SIGNAL(refreshLevelRequested(void)), _gameController, SLOT(replay(void)));
}

void MainWindow::initLevelDesigner(void) {
  _levelDesignerModel = new LevelDesignerModel;

  _levelDesignerController = new LevelDesignerController(_levelDesignerModel, _levelDesignerUndoStack);

  _homeWidget->rootContext()->setContextProperty("levelInfo", _levelDesignerController->getLevelInfo());

  _levelDesignerTreeView = new LevelDesignerTreeView(_levelDesignerController, this);
  _levelDesignerTreeView->setModel(_levelDesignerModel);
  _levelDesignerTreeView->move(650, 100);
  _levelDesignerTreeView->hide();

  _levelDesignerScribbleView = new LevelDesignerScribbleView(_levelDesignerController, centralWidget());
  _levelDesignerScribbleView->setModel(_levelDesignerModel);
  _levelDesignerScribbleView->setSelectionModel(_levelDesignerTreeView->selectionModel());
  _levelDesignerScribbleView->move(175, 150);
  _levelDesignerScribbleView->hide();

  connect(_levelDesignerTreeView, SIGNAL(updateViewNotModel(QModelIndex,int)), _levelDesignerScribbleView, SLOT(drawFromModel(QModelIndex,int)));
  connect(_levelDesignerUndoStack, SIGNAL(indexChanged(int)), _levelDesignerController, SLOT(updateSavingState(int)));
}

void MainWindow::initTestLevel(void) {
  _testLevelModel = new GameModel;
  _testLevelModel->setPolygonList(_levelDesignerModel->getPolygonList());

  _testLevelController = new TestLevelController(_testLevelModel, new QUndoStack);

  _testLevelView = new TestLevelView(_testLevelController, this);
  _testLevelView->setModel(_testLevelModel);
  _testLevelView->move(175, 150);
  _testLevelView->hide();

  connect(_testLevelView, SIGNAL(newLineDrawn(Segment)), _testLevelController, SLOT(addNewLine(Segment)));

  QQuickItem* item = _homeWidget->rootObject();
  connect(item, SIGNAL(refreshLevelRequested(void)), _testLevelView, SLOT(replay(void)));
  connect(item, SIGNAL(saveLevelRequested(void)), _testLevelController, SLOT(checkWinning(void)));
  connect(_testLevelController, SIGNAL(levelCanBeSaved(bool)), this, SLOT(showSaveLevel(bool)));
}
