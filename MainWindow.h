#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>

#include "Segment.h"
#include "PolygonTreeModel.h"
#include "PolygonTreeView.h"
#include "PolygonScribbleView.h"
#include "Commands.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = NULL);

    bool confirmErase(void);
    void initNewDocument(bool neverSavedBefore, const QString& fileName = "");

public slots:
    void updateSavingState(int index);
    void newFile(void);
    void clear(void);
    void openFile(void);
    void saveFile(void);
    void saveAsFile(void);
    void saveAndUpdate(void);
    void addPolygon(void);
    void alignToGrid(void);

private:
    QTabWidget* _tabWidget;
    QWidget* _levelWidget;

    PolygonController* _controller;
    PolygonTreeView* _treeView;
    PolygonScribbleView* _scribbleView;
    PolygonTreeModel* _model;

    QUndoStack* _undoStack;
    QUndoView* _undoView;
    QUndoView* _selectionsView;

    bool _fileSaved;
    bool _neverSavedBefore;
    QString _fileName;
//    int _indexWhereToSave;

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
