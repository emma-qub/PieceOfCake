#ifndef TESTLEVELCONTROLLER_HXX
#define TESTLEVELCONTROLLER_HXX

#include "GameController.h"
#include "LevelInfo.h"

class TestLevelController: public GameController {
  Q_OBJECT

public:
  TestLevelController(GameModel* model, QUndoStack* undoStack, LevelInfo* levelInfo, QObject* parent = nullptr);
  virtual ~TestLevelController(void);

  inline void setTestIsOver(bool b) { _testIsOver = b; }

  inline PolygonList getPrevPolygonList(void) const { return _prevPolygonList; }
  inline void setPrevPolygonList(const PolygonList& prevPolygonList) { _prevPolygonList = prevPolygonList; }

  void resetGameInfo(void);

  void sliceItNot(const std::vector<Segment>& lines);

public slots:
  void openLevel(const QString& fileName) final;
  //void saveLevel(const QString& fileName) final;
  void sliceIt(const std::vector<Segment>& lines) final;
  void addNewLine(const Segment& line);
  void computeOrientedArea(void);
  void checkWinning(void) override;

signals:
  void levelCanBeSaved(bool);

private:
  bool _testIsOver;
  LevelInfo* _levelInfo;
  PolygonList _prevPolygonList;

};

#endif // TESTLEVELCONTROLLER_HXX
