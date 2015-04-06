#include "TestLevelController.h"

#define cerro(x) std::cerr << x << std::endl;

TestLevelController::TestLevelController(GameModel* model, TestLevelModel* lineModel, QUndoStack* undoStack, LevelInfo* levelInfo, QObject* parent):
  GameController(model, undoStack, parent),
  _levelInfo(levelInfo),
  _lineModel(lineModel) {

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

void TestLevelController::sliceItNot(const std::vector<Segment>& lines) {
  _model->setPolygonList(_prevPolygonList);

  PolygonList newPolygonList;

  for (const Segment& line: lines) {
    // Browse every line and slice it!
    computeNewPolygonList(newPolygonList, line);
    _model->setPolygonList(newPolygonList);

    newPolygonList.clear();
  }
}

void TestLevelController::addLine(int lineRow, const Segment& line) {
  if (lineRow == -1)
    lineRow = _lineModel->rowCount();

  _lineModel->insertLine(lineRow, line);
}

void TestLevelController::appendLine(const Segment& line) {
  addLine(_lineModel->rowCount(), line);
}

void TestLevelController::removeLine(int lineRow) {
  int newLineRow = lineRow;
  if (_lineModel->rowCount() == 0) {
    newLineRow = -1;
  } else {
    if (lineRow == _lineModel->rowCount()-1) {
      newLineRow = lineRow-1;
    }
  }

  _lineModel->removeLine(newLineRow);
}

void TestLevelController::moveLine(int lineRow, int oldX, int oldY, int newX, int newY) {
  Segment line(_lineModel->lineFromIndex(_lineModel->index(lineRow, 0)));
  line.translate(newX-oldX, newY-oldY);
  _lineModel->replaceLine(lineRow, line);

  emitUpdate(0);
}

void TestLevelController::addVertex(int lineRow, int vertexRow, const Point2d& vertex) {
  _lineModel->insertVertex(lineRow, vertexRow, vertex);
}

void TestLevelController::removeVertex(int lineRow, int vertexRow, const Point2d& vertex) {
  int newVertexRow = vertexRow;
  if (_lineModel->rowCount(_lineModel->index(lineRow, 0)) == 0) {
    newVertexRow = -1;
  } else {
    if (vertexRow == _lineModel->rowCount(_lineModel->index(lineRow, 0)) - 1) {
      newVertexRow = vertexRow-1;
    }
  }

  _lineModel->insertVertex(lineRow, newVertexRow, vertex);
}

void TestLevelController::moveVertex(int lineRow, int vertexRow, int newX, int newY) {
  _lineModel->replaceVertex(lineRow, vertexRow, Point2d(newX, newY));

  emitUpdate(0);
}

//void TestLevelController::clear(void) {
//  AbstractController::clear();
//  _lineModel->clear();
//  emit updateReset();
//  emitUpdate(0);
//}

void TestLevelController::checkWinning(void) {
//  if (_testIsOver) {
  cerro("Lines: ");cerro(_gameInfo->linesDrawn());
  cerro("Parts: ");cerro(_gameInfo->partsCut());

  GameController::checkWinning();
  emit levelCanBeSaved(_gameInfo->stars() == 6);
//  }
}

void TestLevelController::openLevel(const QString& fileName) {
  GameController::openLevel(fileName);
  _prevPolygonList = _model->getPolygonList();
}

//void TestLevelController::saveLevel(const QString&) {
//}

void TestLevelController::sliceIt(const std::vector<Segment>& lines) {
  GameController::sliceIt(lines);
  _prevPolygonList = _model->getPolygonList();
}

void TestLevelController::addNewLine(const Segment& line) {
  _gameInfo->setLinesCount(_gameInfo->linesDrawn());
  _gameInfo->setPartsCount(_gameInfo->partsCut());

  _levelInfo->updateLevelReadyToBeCut(_gameInfo->linesDrawn());
  appendLine(line);
  std::cerr << "LevelReadyToBeCut: " << _levelInfo->levelReadyToBeCut() << std::endl;
}

void TestLevelController::emitUpdate(int) {
  emit selection();
  emit update();
}
