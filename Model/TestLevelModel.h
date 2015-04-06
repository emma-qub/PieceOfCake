#ifndef TESTLEVELMODEL_H
#define TESTLEVELMODEL_H

#include <QStack>
#include <QColor>

#include "Segment.h"
#include "TreeItem.h"
#include "TreeModel.h"

typedef QList<Segment> LineList;

class TestLevelModel: public TreeModel {
  Q_OBJECT

public:
  TestLevelModel(const QStringList& headers = QStringList(), const QString& data = QString(), QObject* parent = NULL);
  ~TestLevelModel(void);

  inline LineList getLineList(void) const { return _lineList; }
  void setLineList(const LineList& lineList);

  inline QColor getRandomColor(void) const { return _randomColor; }

  inline QStack<QPair<int, int>> getSelections(void) const { return _selections; }
  inline void addSelection(QPair<int, int> selection) { _selections << selection; }
  inline void addSelection(int lineRow, int vertexRow) { addSelection(QPair<int, int>(lineRow, vertexRow)); }
  inline QPair<int, int> popSelection(void) { return _selections.pop(); }

  void clear(void);

  Segment lineFromIndex(const QModelIndex& lineIndex);
  Point2d vertexFromIndex(const QModelIndex& vertexIndex);

  bool appendLine(const Segment& line);
  bool insertLine(int lineRow, const Segment& line);
  bool removeLine(int lineRow);
  bool replaceLine(int lineRow, const Segment& line);
  bool translateLine(int lineRow, const Vector2d& direction);

  bool insertVertex(int lineRow, int vertexRow, const Point2d& vertex);
  bool replaceVertex(int lineRow, int vertexRow, const Point2d& vertex);

  void updateLines(void);
  void updateVertices(QModelIndex lineIndex);

protected:
  LineList _lineList;
  QStack<QPair<int, int>> _selections;
  QColor _randomColor;
};

#endif // TESTLEVELMODEL_H
