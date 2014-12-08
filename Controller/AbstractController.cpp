#include "AbstractController.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>

AbstractController::AbstractController(QAbstractItemModel* model, QUndoStack* undoStack, QObject* parent):
  QObject(parent),
  _model(model),
  _undoStack(undoStack),
  _fileSaved(true),
  _neverSavedBefore(true),
  _fileName() {
}

bool AbstractController::confirmErase(QWidget* parent) {
  bool res = true;

  int msgBox = QMessageBox::warning(parent, tr("Confirmation"), tr("The document has been modified.\nDo you want to save your changes?"), QMessageBox::Save, QMessageBox::Discard, QMessageBox::Cancel);

  switch (msgBox) {
  case QMessageBox::Save:
    saveFile(parent);
    break;
  case QMessageBox::Discard:
    break;
  case QMessageBox::Cancel:
    res = false;
    break;
  }

  return res;
}

void AbstractController::initNewDocument(bool neverSavedBefore, const QString& fileName) {
  _undoStack->clear();
  _neverSavedBefore = neverSavedBefore;
  _fileSaved = true;
  _fileName = fileName;
  updateSavingState(_undoStack->index());
  clear();
}

void AbstractController::updateSavingState(int index) {
  if (_undoStack->cleanIndex() == index) {
    QString baseName = _fileName.split("/").last();
    if (_fileName.isEmpty())
      baseName = "Untitled";
    //_tabWidget->setWindowTitle(_tabWidget->windowTitle()+" - "+baseName);
    //_tabWidget->setTabText(0, "Level - "+baseName);
    _fileSaved = true;
  } else {
//    if (!_tabWidget->windowTitle().endsWith("*"))
//      _tabWidget->setWindowTitle(_tabWidget->windowTitle()+" *");
    _fileSaved = false;
    //_tabWidget->setStyleSheet("font-weight: bold;");
  }
}

void AbstractController::newFile(QWidget* parent) {
  if ((!_fileSaved && confirmErase(parent)) || _fileSaved) {
    initNewDocument(true);
  }
}

void AbstractController::clear(void) {
  _undoStack->clear();
}

void AbstractController::openFile(QWidget* parent) {
  bool canOpen = true;
  if (!_fileSaved) {
    canOpen = confirmErase(parent);
  }

  if (canOpen) {
    QString fileName = QFileDialog::getOpenFileName(parent, "Open file", "../PieceOfCake/levels/", "XML Files (*.xml)");
    if (!checkFileExists(fileName)) {
      qDebug() << "Error:" << fileName << "file not found in AbstractController::openFile";
      return;
    }

    if (!fileName.isEmpty()) {
      initNewDocument(false, fileName);
      openLevel(fileName);
    }
  }
}

bool AbstractController::checkFileExists(const QString& fileName) {
  QFile file(fileName);
  return file.exists();
}

void AbstractController::saveFile(QWidget* parent) {
  if (_neverSavedBefore && _fileSaved) {
    // Nothing to do: the file is new and no action has been done yet
    return;
  } else if (_neverSavedBefore && !_fileSaved) {
    saveAsFile(parent);
  } else if (!_neverSavedBefore && _fileSaved) {
    // Nothing to do: the file already exists and has just been saved
    return;
  } else if (!_neverSavedBefore && !_fileSaved) {
    saveAndUpdate();
  }
}

void AbstractController::saveAsFile(QWidget* parent) {
  QString fileName = QFileDialog::getSaveFileName(parent, "Save file as", "../PieceOfCake/levels/", "XML files (*.xml)");
  if (!fileName.isEmpty()) {
    if (!fileName.endsWith(".xml", Qt::CaseInsensitive))
      fileName += ".xml";
    _fileName = fileName;
    saveAndUpdate();
  } else {
    _fileSaved = false;
  }
}

void AbstractController::saveAndUpdate(void) {
  saveLevel(_fileName);
  _undoStack->setClean();
  updateSavingState(_undoStack->index());
  _neverSavedBefore = false;
  _fileSaved = true;
}
