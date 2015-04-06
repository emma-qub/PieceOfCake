#ifndef TESTLEVELCONTROLLER_HXX
#define TESTLEVELCONTROLLER_HXX

#include "GameController.h"
#include "TestLevelModel.h"
#include "LevelInfo.h"

class TestLevelController: public GameController {
  Q_OBJECT

public:
  TestLevelController(GameModel* model, TestLevelModel* lineModel, QUndoStack* undoStack, LevelInfo* levelInfo, QObject* parent = nullptr);
  virtual ~TestLevelController(void);

  inline void setTestIsOver(bool b) { _testIsOver = b; }

  inline PolygonList getPrevPolygonList(void) const { return _prevPolygonList; }
  inline void setPrevPolygonList(const PolygonList& prevPolygonList) { _prevPolygonList = prevPolygonList; }

  void resetGameInfo(void);

  void sliceItNot(const std::vector<Segment>& lines);

  void addLine(int lineRow, const Segment& line);
  void appendLine(const Segment& line);
  void removeLine(int lineRow);
  void moveLine(int lineRow, int oldX, int oldY, int newX, int newY);

  void addVertex(int lineRow, int vertexRow, const Point2d& vertex);
  void removeVertex(int lineRow, int vertexRow, const Point2d& vertex);
  void moveVertex(int lineRow, int vertexRow, int newX, int newY);

//  void clear(void);

public slots:
  void openLevel(const QString& fileName) final;
  //void saveLevel(const QString& fileName) final;
  void sliceIt(const std::vector<Segment>& lines) final;
  void addNewLine(const Segment& line);
  void computeOrientedArea(void);
  void checkWinning(void) override;
  void emitUpdate(int);

signals:
  void levelCanBeSaved(bool);
  void selection(void);

private:
  bool _testIsOver;
  LevelInfo* _levelInfo;
  PolygonList _prevPolygonList;
  TestLevelModel* _lineModel;
};

#endif // TESTLEVELCONTROLLER_HXX
