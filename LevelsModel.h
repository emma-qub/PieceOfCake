#ifndef LEVELSMODEL_HXX
#define LEVELSMODEL_HXX

#include <QAbstractListModel>
#include <QObject>

class LevelInfo: public QObject {
  Q_OBJECT

public:
  enum LevelRoles {
    StarsRole = Qt::UserRole + 1,
    ImageRole = Qt::UserRole + 2,
  };

  LevelInfo(QObject* parent = nullptr): QObject(parent) {}
  explicit LevelInfo(const QString& stars, const QString& image, QObject* parent = nullptr):
    QObject(parent), _stars(stars), _image(image) {}
  inline QString stars() const { return _stars; }
  inline QString image() const { return _image; }
  void setStars(const QString& stars) { _stars = stars; }
  void setImage(const QString& image) { _image = image; }
  QVariant data(int role) const
  {
    switch(role) {
    case StarsRole:
      return stars();
    case ImageRole:
      return image();
    default:
      return QVariant();
    }
  }

  QHash<int, QByteArray> roleNames() const {
    QHash<int, QByteArray> roles;
    roles[StarsRole] = "stars";
    roles[ImageRole] = "image";
    return roles;
  }

private:
  QString _stars;
  QString _image;
};

class LevelsModel : public QAbstractListModel {
  Q_OBJECT

public:
  virtual int rowCount(const QModelIndex& parent) const;
  virtual QVariant data(const QModelIndex& index, int role) const;

public:
  LevelsModel(QObject* parent = nullptr);

private:
  QList<LevelInfo*> _levelsInfo;
};

#endif // LEVELSMODEL_HXX
