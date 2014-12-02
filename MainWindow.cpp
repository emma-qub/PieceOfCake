#include "MainWindow.h"

#include "Segment.h"
#include "Point2d.h"
#include "Vector2d.h"
#include "ParserXML.h"

#include <iostream>
#include <fstream>
#include <iomanip>

#include <QQuickItem>
#include <QQuickView>
#include <QQuickWidget>
#include <QQmlContext>

MainWindow::MainWindow(QWidget* parent):
  QMainWindow(parent) {

  // Create undo stack to manage undo/redo actions
  _undoStack = new QUndoStack;

  // Init level designer widgets
  _levelDesignerWidget = new QSplitter;
  QSplitter* levelDesignerSpliter = new QSplitter(Qt::Vertical, _levelDesignerWidget);
  // Level designer model
  _levelDesignerModel = new LevelDesignerModel;
  // Level designer controller
  _levelDesignerController = new LevelDesignerController(_levelDesignerModel, _levelDesignerWidget, _undoStack, this);
  // Level designer game stat view
  _levelDesignerGameStatView = new LevelDesignerGameStatView(_levelDesignerController, levelDesignerSpliter);
  _levelDesignerGameStatView->setModel(_levelDesignerModel);
  // Level designer tree view
  _levelDesignerTreeView = new LevelDesignerTreeView(_levelDesignerController, levelDesignerSpliter);
  _levelDesignerTreeView->setModel(_levelDesignerModel);
  // Level designer scrible view
  _levelDesignerScribbleView = new LevelDesignerScribbleView(_levelDesignerController, _levelDesignerWidget);
  _levelDesignerScribbleView->setModel(_levelDesignerModel);
  _levelDesignerScribbleView->setSelectionModel(_levelDesignerTreeView->selectionModel());

  // Init game widgets
  QSplitter* gameSplitter = new QSplitter;
  // Game model
  _gameModel = new GameModel;
  // Game controller
  _gameController = new GameController(_gameModel, gameSplitter, _undoStack, this);
  // Game view
  _gameView = new GameView(_gameController, gameSplitter);
  _gameView->setModel(_gameModel);
  // Game tree view
  QTreeView* gameView = new QTreeView(gameSplitter);
  gameView->setModel(_gameModel);
  _gameView->setSelectionModel(gameView->selectionModel());

  // Init current controller
  _currentController = _levelDesignerController;


  _levelsModel = new LevelsModel;
  QQuickWidget* qmlView = new QQuickWidget(QUrl::fromLocalFile("../PieceOfCake/main.qml"));
  QQmlContext* ctxt = qmlView->rootContext();
  ctxt->setContextProperty("levelsModel", _levelsModel);
  QQuickItem* item = qmlView->rootObject();
  connect(item, SIGNAL(qmlSignal(QString)), this, SLOT(getQMLSignal(QString)));

  // Set central widget
  //setCentralWidget(_homeMenu);
  setCentralWidget(qmlView);

  // File menu
  QMenu* fileMenu = menuBar()->addMenu("&File");
  _newAction = fileMenu->addAction("&New...");
  _newAction->setShortcut(QKeySequence::New);
  connect(_newAction, SIGNAL(triggered()), _levelDesignerController, SLOT(newFile(void)));
  _openAction = fileMenu->addAction("&Open...");
  _openAction->setShortcut(QKeySequence::Open);
  connect(_openAction, SIGNAL(triggered(void)), this, SLOT(openFile(void)));
  fileMenu->addSeparator();
  _saveAction = fileMenu->addAction("&Save");
  _saveAction->setShortcut(QKeySequence::Save);
  connect(_saveAction, SIGNAL(triggered()), _levelDesignerController, SLOT(saveFile(void)));
  _saveAsAction = fileMenu->addAction("Save &as...");
  _saveAsAction->setShortcut(QKeySequence::SaveAs);
  connect(_saveAsAction, SIGNAL(triggered(void)), _levelDesignerController, SLOT(saveAsFile(void)));

  // Edit menu
  QMenu* editMenu = menuBar()->addMenu("&Edit");
  _undoAction = _undoStack->createUndoAction(this, "&Undo");
  _undoAction->setShortcut(QKeySequence::Undo);
  editMenu->addAction(_undoAction);
  _redoAction = _undoStack->createRedoAction(this, "&Redo");
  _redoAction->setShortcut(QKeySequence::Redo);
  editMenu->addAction(_redoAction);
  editMenu->addSeparator();
  _addPolygonAction = editMenu->addAction("&New Polygon");
  _addPolygonAction->setShortcut(QKeySequence("CTRL+SHIFT+N"));
  connect(_addPolygonAction, SIGNAL(triggered()), this, SLOT(addPolygon()));
  editMenu->addSeparator();
  _magnetismAction = editMenu->addAction("&Magnetism");
  _magnetismAction->setShortcut(QKeySequence("CTRL+M"));
  _magnetismAction->setCheckable(true);
  _magnetismAction->setChecked(false);
  connect(_magnetismAction, SIGNAL(toggled(bool)), _levelDesignerScribbleView, SLOT(setMagnetism(bool)));
  _alignToGridAction = editMenu->addAction("&Align to grid");
  _alignToGridAction->setShortcut(QKeySequence("CTRL+I"));
  connect(_alignToGridAction, SIGNAL(triggered()), _levelDesignerController, SLOT(alignToGrid()));

  // Connections
  connect(_levelDesignerTreeView, SIGNAL(updateViewNotModel(QModelIndex,int)), _levelDesignerScribbleView, SLOT(drawFromModel(QModelIndex,int)));
  connect(_undoStack, SIGNAL(indexChanged(int)), _levelDesignerController, SLOT(updateSavingState(int)));

  // Change soft title
  setWindowTitle("Graphical tests");

  // Resize
  setFixedSize(1200, 756);

  setStyleSheet(" \
  QSplitter::handle:vertical { \
      width: 1px; \
      color: black; \
  }"
                );

}

void MainWindow::getQMLSignal(QString mess) {
  if (mess == "play") {
//    setCentralWidget();
  } else if (mess == "createLevels") {
    setCentralWidget(_levelDesignerScribbleView);
  } else if (mess == "options") {
//    setCentralWidget();
  } else if (mess == "credit") {
//    setCentralWidget();
  } else if (mess == "followUs") {
//    setCentralWidget();
  }
}

void MainWindow::openFile(void) {
//  if (_tabWidget->currentIndex() == 0)
//    _levelDesignerController->openFile();
//  else
    _gameController->openFile();
}

void MainWindow::addPolygon(void) {
  if (_currentController == _levelDesignerController)
    _levelDesignerController->addPolygon(_levelDesignerModel->rowCount(), Polygon());
}
