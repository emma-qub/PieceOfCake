#include "GameInfo.h"

GameInfo::GameInfo(QObject* parent):
  QObject(parent),
  _linesDrawn(-1),
  _linesCount(-1),
  _partsCut(-1),
  _partsCount(-1),
  _stars(-1),
  _starsMax(-1) {
}

int GameInfo::linesDrawn(void) const {
  return _linesDrawn;
}

int GameInfo::linesCount(void) const {
  return _linesCount;
}

int GameInfo::partsCut(void) const {
  return _partsCut;
}

int GameInfo::partsCount(void) const {
  return _partsCount;
}

int GameInfo::stars(void) const {
  return _stars;
}

int GameInfo::starsMax(void) const {
  return _starsMax;
}

QString GameInfo::evaluation(void) const {
  return _evaluation;
}

void GameInfo::setLinesDrawn(int value) {
  if (value != _linesDrawn) {
    _linesDrawn = value;
    emit linesDrawnChanged();
  }
}

void GameInfo::setLinesCount(int value) {
  if (value != _linesCount) {
    _linesCount = value;
    emit linesCountChanged();
  }
}

void GameInfo::setPartsCut(int value) {
  if (value != _partsCut) {
    _partsCut = value;
    emit partsCutChanged();
  }
}

void GameInfo::setPartsCount(int value) {
  if (value != _partsCount) {
    _partsCount = value;
    emit partsCountChanged();
  }
}

void GameInfo::setStars(int value) {
  if (value != _stars) {
    _stars = value;
    emit starsChanged();
  }

  if (_stars > _starsMax) {
    setStarsMax(_stars);
  }

  if (value == -1)
    setEvaluation("");
  else if (value == 0)
    setEvaluation("FAILED");
  else if (0 < value && value <= 4)
    setEvaluation("CLEAR");
  else if (value == 5)
    setEvaluation("GREAT");
  else if (value == 6)
    setEvaluation("PERFECT!");
}

void GameInfo::setStarsMax(int value) {
  if (value != _starsMax) {
    _starsMax = value;
    emit starsMaxChanged();
  }
}

void GameInfo::setEvaluation(const QString& value) {
  if (value != _evaluation) {
    _evaluation = value;
    emit evaluationChanged();
  }
}
