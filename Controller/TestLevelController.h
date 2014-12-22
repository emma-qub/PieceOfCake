#ifndef TESTLEVELCONTROLLER_HXX
#define TESTLEVELCONTROLLER_HXX

#include "GameController.h"

class TestLevelController: public GameController {
  Q_OBJECT

public:
  TestLevelController(GameModel* model, QUndoStack* undoStack, QObject* parent = nullptr);
  virtual ~TestLevelController(void);

  void checkWinning(void) override;

  inline void setTestIsOver(bool b) { _testIsOver = b; }

public slots:
  void openLevel(const QString& fileName) final;
  //void saveLevel(const QString& fileName) final;

signals:
  void levelCanBeSaved(bool);

private:
  bool _testIsOver;

};

#endif // TESTLEVELCONTROLLER_HXX
