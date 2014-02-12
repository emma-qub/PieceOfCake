#include "MainWindow.h"

#include "Segment.h"
#include "Point2d.h"
#include "Vector2d.h"
#include "ParserXML.h"

#include <iostream>
#include <fstream>
#include <iomanip>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    _fileSaved(true),
    _neverSavedBefore(true),
    _fileName() {

    _levelWidget = new QSplitter;
    _model = new PolygonTreeModel;

    // Create undo stack to manage undo/redo actions
    _undoStack = new QUndoStack;
    connect(_undoStack, SIGNAL(indexChanged(int)), this, SLOT(updateSavingState(int)));

    _controller = new PolygonController(_model, _undoStack, this);

    _treeView = new PolygonTreeView(_controller, _levelWidget);
    _treeView->setModel(_model);
    _scribbleView = new PolygonScribbleView(_controller, _levelWidget);
    _scribbleView->setModel(_model);
    _scribbleView->setSelectionModel(_treeView->selectionModel());

    _tabWidget = new QTabWidget;
    _tabWidget->addTab(_levelWidget, "Level");

    setCentralWidget(_tabWidget);

    // File menu
    QMenu* fileMenu = menuBar()->addMenu("&File");

    _newAction = fileMenu->addAction("&New...");
    _newAction->setShortcut(QKeySequence::New);
    connect(_newAction, SIGNAL(triggered()), this, SLOT(newFile(void)));

    _openAction = fileMenu->addAction("&Open...");
    _openAction->setShortcut(QKeySequence::Open);
    connect(_openAction, SIGNAL(triggered(void)), this, SLOT(openFile(void)));

    fileMenu->addSeparator();

    _saveAction = fileMenu->addAction("&Save");
    _saveAction->setShortcut(QKeySequence::Save);
    connect(_saveAction, SIGNAL(triggered()), this, SLOT(saveFile(void)));

    _saveAsAction = fileMenu->addAction("Save &as...");
    _saveAsAction->setShortcut(QKeySequence::SaveAs);
    connect(_saveAsAction, SIGNAL(triggered(void)), this, SLOT(saveAsFile(void)));


    // Edit menu
    QMenu* editMenu = menuBar()->addMenu("&Edit");

    _undoAction = _undoStack->createUndoAction(this, "&Undo");
    editMenu->addAction(_undoAction);
    _undoAction->setShortcut(QKeySequence::Undo);

    _redoAction = _undoStack->createRedoAction(this, "&Redo");
    editMenu->addAction(_redoAction);
    _redoAction->setShortcut(QKeySequence::Redo);

    _undoView = new QUndoView(_undoStack, _levelWidget);
    _undoView->setWindowTitle(tr("Command List"));
    _undoView->setEmptyLabel("New Level");
    _undoView->setAlternatingRowColors(true);
    _undoView->show();
    _undoView->setAttribute(Qt::WA_QuitOnClose, false);

    editMenu->addSeparator();

    _addPolygonAction = editMenu->addAction("&New Polygon");
    _addPolygonAction->setShortcut(QKeySequence("CTRL+SHIFT+N"));
    connect(_addPolygonAction, SIGNAL(triggered()), this, SLOT(addPolygon()));

    editMenu->addSeparator();

    _magnetismAction = editMenu->addAction("&Magnetism");
    _magnetismAction->setShortcut(QKeySequence("CTRL+M"));
    _magnetismAction->setCheckable(true);
    _magnetismAction->setChecked(false);
    connect(_magnetismAction, SIGNAL(toggled(bool)), _scribbleView, SLOT(setMagnetism(bool)));

    _alignToGridAction = editMenu->addAction("&Align to grid");
    _alignToGridAction->setShortcut(QKeySequence("CTRL+I"));
    connect(_alignToGridAction, SIGNAL(triggered()), this, SLOT(alignToGrid()));

    // Connections
    connect(_treeView, SIGNAL(updateViewNotModel(QModelIndex,int)), _scribbleView, SLOT(drawFromModel(QModelIndex,int)));

    // Change soft title
    setWindowTitle("Graphical tests");

    resize(1200, 756);
}

void MainWindow::updateSavingState(int index) {
    if (_undoStack->cleanIndex() == index) {
        QString baseName = _fileName.split("/").last();
        if (_fileName.isEmpty())
            baseName = "Untitled";
        _tabWidget->setTabText(0, "Level - "+baseName);
        _fileSaved = true;
    } else {
        if (!_tabWidget->tabText(0).endsWith("*"))
            _tabWidget->setTabText(0, _tabWidget->tabText(0)+" *");
        _fileSaved = false;
//        _tabWidget->setStyleSheet("font-weight: bold;");
    }
}

bool MainWindow::confirmErase(void) {
    bool res = true;

    int msgBox = QMessageBox::warning(this, "Confirmation", "The document has been modified.\nDo you want to save your changes?", QMessageBox::Save, QMessageBox::Discard, QMessageBox::Cancel);

    switch (msgBox) {
    case QMessageBox::Save:
        saveFile();
        break;
    case QMessageBox::Discard:
        break;
    case QMessageBox::Cancel:
        res = false;
        break;
    }

    return res;
}

void MainWindow::initNewDocument(bool neverSavedBefore, const QString& fileName) {
    _undoStack->clear();
    _neverSavedBefore = neverSavedBefore;
    _fileSaved = true;
    _fileName = fileName;
    updateSavingState(_undoStack->index());
    clear();
}

void MainWindow::newFile(void) {
    if ((!_fileSaved && confirmErase()) || _fileSaved) {
        initNewDocument(true);
    }
}

void MainWindow::clear(void) {
    _controller->clear();
}

void MainWindow::openFile(void) {
    bool canOpen = true;
    if (!_fileSaved) {
        canOpen = confirmErase();
    }

    if (canOpen) {
        QString fileName = QFileDialog::getOpenFileName(this, "Open file", "../PieceOfCake/levels/", "XML Files (*.xml)");

        if (!fileName.isEmpty()) {
            initNewDocument(false, fileName);
            _controller->openLevel(fileName);
        }
    }
}

void MainWindow::saveFile(void) {
    if (_neverSavedBefore && _fileSaved) {
        // Nothing to do: the file is new and no action has been done yet
        return;
    } else if (_neverSavedBefore && !_fileSaved) {
        saveAsFile();
    } else if (!_neverSavedBefore && _fileSaved) {
        // Nothing to do: the file already exists and has just been saved
        return;
    } else if (!_neverSavedBefore && !_fileSaved) {
        saveAndUpdate();
    }
}

void MainWindow::saveAsFile(void) {
    QString fileName = QFileDialog::getSaveFileName(this, "Save file as", "../SliceIt/levels/", "XML files (*.xml)");
    if (!fileName.isEmpty()) {
        if (!fileName.endsWith(".xml", Qt::CaseInsensitive))
            fileName += ".xml";
        _fileName = fileName;
        saveAndUpdate();
    } else {
        _fileSaved = false;
    }
}

void MainWindow::saveAndUpdate(void) {
    _controller->saveLevel(_fileName);
    _undoStack->setClean();
    updateSavingState(_undoStack->index());
    _neverSavedBefore = false;
    _fileSaved = true;
}

void MainWindow::addPolygon(void) {
    _controller->addPolygon(_model->rowCount(), Polygon());
//    /// TODO: remove this line and handle selection properly
//    _treeView->setCurrentIndex(_model->index(_model->rowCount()-1, 0));
}

void MainWindow::alignToGrid(void) {
    _controller->alignToGrid();
}
