#include "MainWindow.h"

#include "Segment.h"
#include "Point2d.h"
#include "Vector2d.h"
#include "ParserXML.h"

#include <iostream>
#include <fstream>
#include <iomanip>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent) {

    // Create undo stack to manage undo/redo actions
    _undoStack = new QUndoStack;
    // Init level designer widgets
    _levelDesignerWidget = new QSplitter;
    _levelDesignerModel = new LevelDesignerModel;
    _levelDesignerController = new LevelDesignerController(_levelDesignerModel, _levelDesignerWidget, _undoStack, this);
    _levelDesignerTreeView = new LevelDesignerTreeView(_levelDesignerController, _levelDesignerWidget);
    _levelDesignerTreeView->setModel(_levelDesignerModel);
    _levelDesignerScribbleView = new LevelDesignerScribbleView(_levelDesignerController, _levelDesignerWidget);
    _levelDesignerScribbleView->setModel(_levelDesignerModel);
    _levelDesignerScribbleView->setSelectionModel(_levelDesignerTreeView->selectionModel());

    // Init current controller
    _currentController = _levelDesignerController;



    // Splitter game
    QSplitter* gameSplitter = new QSplitter;




    // Init game widgets
    _gameModel = new GameModel;
    _gameController = new GameController(_gameModel, _gameView, _undoStack, this);
    _gameView = new GameView(_gameController, gameSplitter);
    _gameView->setModel(_gameModel);




    QTreeView* gameView = new QTreeView(gameSplitter);
    gameView->setModel(_gameModel);




    // Init tab widget
    _tabWidget = new QTabWidget;
    //    _stackWidget = new QStackedWidget;
    _tabWidget->addTab(_levelDesignerWidget, "Level Designer");
    _tabWidget->addTab(gameSplitter, "Game");
//    _stackWidget->addWidget(_levelDesignerWidget);
//    _stackWidget->addWidget(_gameView);

    // Set central widget
    setCentralWidget(_tabWidget);
//    setCentralWidget(_stackWidget);

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

//    connect(_tabWidget, SIGNAL(currentChanged(int)), this, SLOT(updateMenuActions(int)));
//    connect(_tabWidget, SIGNAL(currentChanged(int)), this, SLOT(updateCurrentController(int)));
//    connect(_stackWidget, SIGNAL(currentChanged(int)), this, SLOT(updateMenuActions(int)));
//    connect(_stackWidget, SIGNAL(currentChanged(int)), this, SLOT(updateCurrentController(int)));

    // Change soft title
    setWindowTitle("Graphical tests");

    // Resize
    resize(1200, 756);
}

//bool MainWindow::confirmErase(void) {
//    bool res = true;

//    int msgBox = QMessageBox::warning(this, "Confirmation", "The document has been modified.\nDo you want to save your changes?", QMessageBox::Save, QMessageBox::Discard, QMessageBox::Cancel);

//    switch (msgBox) {
//    case QMessageBox::Save:
//        saveFile();
//        break;
//    case QMessageBox::Discard:
//        break;
//    case QMessageBox::Cancel:
//        res = false;
//        break;
//    }

//    return res;
//}

//void MainWindow::initNewDocument(bool neverSavedBefore, const QString& fileName) {
//    _undoStack->clear();
//    _neverSavedBefore = neverSavedBefore;
//    _fileSaved = true;
//    _fileName = fileName;
//    updateSavingState(_undoStack->index());
//    clear();
//}

//void MainWindow::updateSavingState(int index) {
//    if (_undoStack->cleanIndex() == index) {
//        QString baseName = _fileName.split("/").last();
//        if (_fileName.isEmpty())
//            baseName = "Untitled";
//        _tabWidget->setTabText(0, "Level - "+baseName);
//        _fileSaved = true;
//    } else {
//        if (!_tabWidget->tabText(0).endsWith("*"))
//            _tabWidget->setTabText(0, _tabWidget->tabText(0)+" *");
//        _fileSaved = false;
////        _tabWidget->setStyleSheet("font-weight: bold;");
//    }
//}

//void MainWindow::newFile(void) {
//    _currentController->newFile();
//}

//void MainWindow::clear(void) {
//    _currentController->clear();
//}

void MainWindow::openFile(void) {
    if (_tabWidget->currentIndex() == 0)
        _levelDesignerController->openFile();
    else
        _gameController->openFile();
}

//void MainWindow::saveFile(void) {
//    _currentController->saveFile();
//}

//void MainWindow::saveAsFile(void) {
//    _currentController->saveAsFile();
//}

//void MainWindow::saveAndUpdate(void) {
//    _levelDesignerController->saveLevel(_fileName);
//    _undoStack->setClean();
//    updateSavingState(_undoStack->index());
//    _neverSavedBefore = false;
//    _fileSaved = true;
//}

void MainWindow::addPolygon(void) {
    if (_currentController == _levelDesignerController)
        _levelDesignerController->addPolygon(_levelDesignerModel->rowCount(), Polygon());
}

//void MainWindow::alignToGrid(void) {
//    if (_currentController == _levelDesignerController)
//        _levelDesignerController->alignToGrid();
//}

//void MainWindow::updateMenuActions(int) {
//    bool isLevelDesigner = (_currentController == _levelDesignerController);

//    _undoAction->setEnabled(isLevelDesigner);
//    _redoAction->setEnabled(isLevelDesigner);
//    _addPolygonAction->setEnabled(isLevelDesigner);
//    _alignToGridAction->setEnabled(isLevelDesigner);
//}

//void MainWindow::updateCurrentController(int currentIndex) {
//    if (currentIndex == 0)
//        _currentController = _levelDesignerController;
//    else if (currentIndex == 1)
//        _currentController = _gameController;
//    else
//        _currentController = NULL;
//}
