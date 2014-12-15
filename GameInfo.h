#ifndef GAMEINFO_H
#define GAMEINFO_H

#include <QObject>

class GameInfo: public QObject {
  Q_OBJECT
  Q_PROPERTY(int linesDrawn READ linesDrawn WRITE setLinesDrawn NOTIFY linesDrawnChanged)
  Q_PROPERTY(int linesCount READ linesCount WRITE setLinesCount NOTIFY linesCountChanged)
  Q_PROPERTY(int partsCut READ partsCut WRITE setPartsCut NOTIFY partsCutChanged)
  Q_PROPERTY(int partsCount READ partsCount WRITE setPartsCount NOTIFY partsCountChanged)
  Q_PROPERTY(int stars READ stars WRITE setStars NOTIFY starsChanged)
  Q_PROPERTY(int starsMax READ starsMax WRITE setStarsMax NOTIFY starsMaxChanged)

public:
  GameInfo(void);

  int linesDrawn(void) const;
  int linesCount(void) const;
  int partsCut(void) const;
  int partsCount(void) const;
  int stars(void) const;
  int starsMax(void) const;

  void setLinesDrawn(int value);
  void setLinesCount(int value);
  void setPartsCut(int value);
  void setPartsCount(int value);
  void setStars(int value);
  void setStarsMax(int value);

signals:
  void linesDrawnChanged(void);
  void linesCountChanged(void);
  void partsCutChanged(void);
  void partsCountChanged(void);
  void starsChanged(void);
  void starsMaxChanged(void);

private:
  int _linesDrawn;
  int _linesCount;
  int _partsCut;
  int _partsCount;
  int _stars;
  int _starsMax;
};

#endif // GAMEINFO_H
