#ifndef GAMEINFO_H
#define GAMEINFO_H

#include <QObject>

class GameInfo: public QObject {
    Q_OBJECT
    Q_PROPERTY(int linesDrawn READ linesDrawn WRITE setLinesDrawn NOTIFY linesDrawnChanged)
    Q_PROPERTY(int linesCount READ linesCount WRITE setLinesCount NOTIFY linesCountChanged)
    Q_PROPERTY(int partsCut READ partsCut WRITE setPartsCut NOTIFY partsCutChanged)
    Q_PROPERTY(int partsCount READ partsCount WRITE setPartsCount NOTIFY partsCountChanged)

public:
  GameInfo(void);

  int linesDrawn(void) const;
  int linesCount(void) const;
  int partsCut(void) const;
  int partsCount(void) const;

  void setLinesDrawn(int value);
  void setLinesCount(int value);
  void setPartsCut(int value);
  void setPartsCount(int value);

signals:
  void linesDrawnChanged(void);
  void linesCountChanged(void);
  void partsCutChanged(void);
  void partsCountChanged(void);

private:
  int _linesDrawn;
  int _linesCount;
  int _partsCut;
  int _partsCount;

};

#endif // GAMEINFO_H
