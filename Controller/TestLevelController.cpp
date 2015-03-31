#include "TestLevelController.h"

#define cerro(x) std::cerr << x << std::endl;

TestLevelController::TestLevelController(GameModel* model, QUndoStack* undoStack, LevelInfo* levelInfo, QObject* parent):
  GameController(model, undoStack, parent),
  _levelInfo(levelInfo) {

  _levelRunning = true;
  _gameInfo->setLinesCount(0);
  _gameInfo->setLinesDrawn(0);
}

TestLevelController::~TestLevelController(void) {
}

void TestLevelController::resetGameInfo(void) {
  _gameInfo->reset();
}

void TestLevelController::computeOrientedArea(void) {
  for (const Polygon& polygon: _model->getPolygonList()) {
    _orientedAreaTotal += polygon.orientedArea();
  }
}

void TestLevelController::checkWinning(void) {
//  if (_testIsOver) {
  cerro("Lines: ");cerro(_gameInfo->linesDrawn());
  cerro("Parts: ");cerro(_gameInfo->partsCut());

  GameController::checkWinning();
  emit levelCanBeSaved(_gameInfo->stars() == 6);
//  }
}

void TestLevelController::openLevel(const QString&) {
}

//void TestLevelController::saveLevel(const QString&) {
//}

void TestLevelController::addNewLine(const Segment& line) {
  _gameInfo->setLinesCount(_gameInfo->linesDrawn());
  _gameInfo->setPartsCount(_gameInfo->partsCut());

  _levelInfo->updateLevelReadyToBeCut(_gameInfo->linesDrawn());
  std::cerr << "LevelReadyToBeCut: " << _levelInfo->levelReadyToBeCut() << std::endl;
}
