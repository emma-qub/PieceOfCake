#include "LevelsModel.h"
#include "ParserXML.h"

#include <QDebug>

LevelsModel::LevelsModel(QObject* parent):
  QAbstractListModel(parent) {

  QDir levelsDir("../PieceOfCake/resources/levels");
  QStringList levelsNameList = levelsDir.entryList(QStringList() << "*.xml", QDir::Files | QDir::NoDotAndDotDot, QDir::Time);

  for (const QString& levelName: levelsNameList) {
    ParserXML parser(levelsDir.absoluteFilePath(levelName));
    QString levelImage = levelName.split(".xml").first()+".png";
    LevelInfo* levelInfo = new LevelInfo;
    levelInfo->setStars(QString::number(parser.getStarsCount()));
    levelInfo->setImage("../PieceOfCake/resources/levels/"+levelImage);
    _levelsInfo.append(levelInfo);
  }

  for (LevelInfo* li: _levelsInfo)
    qDebug() << li->image() << " " << li->stars();
}

int LevelsModel::rowCount(const QModelIndex& /*parent*/) const {
  return _levelsInfo.size();
}

QVariant LevelsModel::data(const QModelIndex& index, int role) const {
  return _levelsInfo.at(index.row())->data(role);
}
