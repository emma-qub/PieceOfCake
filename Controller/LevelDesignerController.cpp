#include "LevelDesignerController.h"
#include "ParserXML.h"
#include "ThumbnailCreator.h"

#include <QDebug>

LevelDesignerController::LevelDesignerController(LevelDesignerModel *model, QUndoStack* undoStack, QObject* parent):
  AbstractController(model, undoStack, parent),
  _model(model) {

  addPolygon(0, Polygon());

  connect(_undoStack, SIGNAL(indexChanged(int)), this, SLOT(emitUpdate(int)));
}

void LevelDesignerController::addPolygon(int polygonRow, const Polygon& polygon) {
  if (polygonRow == -1)
    polygonRow = _model->rowCount();

  QUndoCommand* addPolygonCommand = new AddPolygonCommand(_model, polygonRow, polygon, polygonRow, -1);
  _undoStack->push(addPolygonCommand);

  if (_model->rowCount() == 1) {
    emit enableStats(true);
  }
}

void LevelDesignerController::removePolygon(int polygonRow, const Polygon& polygon) {
  int newPolygonRow = polygonRow;
  if (_model->rowCount() == 0) {
    newPolygonRow = -1;
  } else {
    if (polygonRow == _model->rowCount()-1) {
      newPolygonRow = polygonRow-1;
    }
  }

  QUndoCommand* removePolygonCommand = new RemovePolygonCommand(_model, polygonRow, polygon, newPolygonRow, -1);
  _undoStack->push(removePolygonCommand);

  if (_model->rowCount() == 0) {
    emit enableStats(false);
  }
}

void LevelDesignerController::movePolygon(int polygonRow, int oldX, int oldY, int newX, int newY, bool pushToStack) {
  if (pushToStack) {
    QUndoCommand* movePolygonCommand = new MovePolygonCommand(_model, polygonRow, oldX, oldY, newX, newY, polygonRow, -1);
    _undoStack->push(movePolygonCommand);
  } else {
    Polygon polygon(_model->polygonFromIndex(_model->index(polygonRow, 0)));
    polygon.translate(Vector2d(newX-oldX, newY-oldY));
    _model->replacePolygon(polygonRow, polygon);

    emitUpdate(0);
  }
}

void LevelDesignerController::addVertex(int polygonRow, int vertexRow, const Point2d& vertex) {
  QUndoCommand* addVertexCommand = new AddVertexCommand(_model, polygonRow, vertexRow, vertex, polygonRow, vertexRow);
  _undoStack->push(addVertexCommand);
}

void LevelDesignerController::removeVertex(int polygonRow, int vertexRow, const Point2d& vertex) {
  int newVertexRow = vertexRow;
  if (_model->rowCount(_model->index(polygonRow, 0)) == 0) {
    newVertexRow = -1;
  } else {
    if (vertexRow == _model->rowCount(_model->index(polygonRow, 0)) - 1) {
      newVertexRow = vertexRow-1;
    }
  }

  QUndoCommand* removeVertexCommand = new RemoveVertexCommand(_model, polygonRow, vertexRow, vertex, polygonRow, newVertexRow);
  _undoStack->push(removeVertexCommand);
}

void LevelDesignerController::moveVertex(int polygonRow, int vertexRow, int oldX, int oldY, int newX, int newY, bool pushToStack) {
  if (pushToStack) {
    QUndoCommand* moverVertexCommand = new MoveVertexCommand(_model, polygonRow, vertexRow, oldX, oldY, newX, newY, polygonRow, vertexRow);
    _undoStack->push(moverVertexCommand);
  } else {
    _model->replaceVertex(polygonRow, vertexRow, Point2d(newX, newY));

    emitUpdate(0);
  }
}

void LevelDesignerController::updateLinesCount(int linesCount) {
  QUndoCommand* updateLinesCountCommand = new UpdateLinesCountCommand(_model, _model->getLinesCount(), linesCount);
  _undoStack->push(updateLinesCountCommand);
}

void LevelDesignerController::updatePartsCount(int partsCount) {
  QUndoCommand* updatePartsCountCommand = new UpdatePartsCountCommand(_model, _model->getPartsCount(), partsCount);
  _undoStack->push(updatePartsCountCommand);
}

void LevelDesignerController::updateMaxGapToWin(int maxGapToWin) {
  QUndoCommand* updateMaxGapToWinCommand = new UpdateMaxGapToWinCommand(_model, _model->getMaxGapToWin(), maxGapToWin);
  _undoStack->push(updateMaxGapToWinCommand);
}

void LevelDesignerController::clear(void) {
  _model->clear();
  _undoStack->clear();
  emit updateReset();
  emitUpdate(0);
}

void LevelDesignerController::appendPolygon(void) {
  addPolygon(_model->rowCount(), Polygon());
}

void LevelDesignerController::saveLevel(const QString& fileName) {
  // Write xml file
  ParserXML parser;
  parser.initFileName(fileName);

  PolygonList polygonList(_model->getPolygonList());
  foreach (const Polygon& polygon, polygonList) {
    parser.addPolygon(polygon);
  }

  parser.setLinesCount(_model->getLinesCount());
  parser.setPartsCount(_model->getPartsCount());
  parser.setMaxGapToWin(_model->getMaxGapToWin());
  parser.setStarsCount(0);
  parser.setTolerances(0);

  parser.writeXML();

  // Rename fileName
  QString newFileName = "resources/levels/"+fileName.split("resources/levels/").last();

  // Create thumbbnail
  ThumbnailCreator thumbnailCreator(_model->getPolygonList());
  QString thumbnailName = newFileName;
  thumbnailName.replace(QRegExp(".xml"), ".png");
  thumbnailCreator.makeThumbnail("../PieceOfCake/"+thumbnailName);

  // Update levels file
  QStringList levelPackPath = newFileName.split("/");
  levelPackPath.removeLast();
  QString xmlFileName = "../PieceOfCake/"+levelPackPath.join("/")+"/levels.xml";

  QFile XMLDoc(xmlFileName);
  if (!XMLDoc.exists()) {
    qDebug() << "Error:" << xmlFileName << "file not found";
    return;
  }

  if(!XMLDoc.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qDebug() << "Cannot open XML file in LevelDesignerController::saveLevel(const QString& fileName)";
    return;
  }

  QDomDocument doc("PieceOfCakeLevelsML");
  if(!doc.setContent(&XMLDoc)) {
    XMLDoc.close();
    qDebug() << "Cannot set content of dom in LevelDesignerController::saveLevel(const QString& fileName)";
    return;
  }

  XMLDoc.close();

  QDomElement root = doc.firstChildElement("levels");

  QDomNodeList levelList = root.elementsByTagName("level");
  for (int k = 0; k < levelList.size(); ++k) {
    QDomElement level = levelList.at(k).toElement();
    // No need to register several times the same level,
    // especially if this method is called to update thumbnail and level.xml file
    if (level.attribute("image").split("/").last() == thumbnailName.split("/").last()) {
      qDebug() << "No need to update levels.xml";
      return;
    }
  }

  QDomElement level = doc.createElement("level");
  level.setAttribute("id", root.elementsByTagName("level").size());
  level.setAttribute("stars", "0");
  level.setAttribute("image", thumbnailName);
  level.setAttribute("name", newFileName);

  root.appendChild(level);

  if(!XMLDoc.open(QIODevice::WriteOnly | QIODevice::Text)) {
    qDebug() << "Cannot open XML file in LevelDesignerController::saveLevel(const QString& fileName)";
    return;
  }

  QTextStream inFile(&XMLDoc);
  inFile << doc.toString(2);

  XMLDoc.close();
}

void LevelDesignerController::openLevel(const QString& fileName) {
  clear();

  ParserXML parser(fileName);

  PolygonList polygonList(parser.createPolygonList());

  _model->setPolygonList(polygonList);
  _model->setLinesCount(parser.getLinesCount());
  _model->setPartsCount(parser.getPartsCount());
  _model->setMaxGapToWin(parser.getMaxGapToWin());
  emit updateStats();
  emitUpdate(0);
}

void LevelDesignerController::alignToGrid(void) {
  PolygonList polygonList = _model->getPolygonList();
  for (int k = 0; k < _model->rowCount(); ++k) {
    TreeItem* polygonItem = _model->getItem(_model->index(k, 0));
    std::vector<Point2d> vertices = polygonList.at(k).getVertices();
    for (int i = 0; i < polygonItem->childCount(); ++i) {
      Point2d currVertex = vertices.at(i);

      int oldX = currVertex.getX();
      int oldY = currVertex.getY();

      int extraX = static_cast<int>(oldX)%50;
      int extraY = static_cast<int>(oldY)%50;

      int newX = oldX;
      int newY = oldY;

      if (extraY < 50/3)
        newY = oldY - extraY;// + 5;
      else if (extraY > 50 - 50/3)
        newY = oldY + (50 - extraY);// + 5;

      if (extraX < 50/3)
        newX = oldX - extraX;// + 5;
      else if (extraX > 50 - 50/3)
        newX = oldX + (50 - extraX);// + 5;

      if (oldX != newX || oldY != newY)
        moveVertex(k, i, oldX, oldY, newX, newY);
    }
  }
}

void LevelDesignerController::emitUpdate(int) {
  emit selection();
  emit update();
}
