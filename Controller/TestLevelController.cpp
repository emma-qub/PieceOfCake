#include "TestLevelController.h"

TestLevelController::TestLevelController(GameModel* model, QUndoStack* undoStack, QObject* parent):
  GameController(model, undoStack, parent) {

  _levelRunning = true;
}

TestLevelController::~TestLevelController(void) {
}

void TestLevelController::checkWinning(void) {
  if (_testIsOver) {
    GameController::checkWinning();
    emit levelCanBeSaved(_gameInfo->stars() == 6);
  }
}

void TestLevelController::openLevel(const QString&) {
}

void TestLevelController::saveLevel(const QString&) {
}
