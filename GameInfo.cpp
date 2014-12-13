#include "GameInfo.h"

GameInfo::GameInfo(void):
  _linesDrawn(-1),
  _linesCount(-1),
  _partsCut(-1),
  _partsCount(-1) {
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
