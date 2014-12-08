#ifndef ABSTRACTCONTROLLER_H
#define ABSTRACTCONTROLLER_H

#include <QObject>
#include <QUndoStack>
#include <QAbstractItemModel>

class AbstractController: public QObject {
  Q_OBJECT

public:
  AbstractController(QAbstractItemModel* model, QUndoStack* undoStack, QObject* parent = NULL);

public slots:
  virtual bool confirmErase(QWidget* parent);
  virtual void initNewDocument(bool neverSavedBefore, const QString& fileName = "");
  virtual void updateSavingState(int index);
  virtual void newFile(QWidget* parent);
  virtual void clear(void);
  virtual void openFile(QWidget* parent);
  virtual void saveFile(QWidget* parent);
  virtual void saveAsFile(QWidget* parent);
  virtual void saveAndUpdate(void);
  virtual void openLevel(const QString& fileName) = 0;
  virtual void saveLevel(const QString& fileName) = 0;

protected:
  virtual bool checkFileExists(const QString& fileName);

  QAbstractItemModel* _model;
  QUndoStack* _undoStack;
  bool _fileSaved;
  bool _neverSavedBefore;
  QString _fileName;
};

#endif // ABSTRACTCONTROLLER_H
