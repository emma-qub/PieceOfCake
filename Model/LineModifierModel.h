#ifndef LINEMODIFIERMODEL_H
#define LINEMODIFIERMODEL_H

#include "TreeModel.h"
#include "Tape.h"
#include "Mirror.h"
#include "Portal.h"
#include "Refractor.h"

typedef QList<Tape> TapeList;
typedef QList<Mirror> MirrorList;
typedef QList<Portal> PortalList;
typedef QList<Refractor> RefractorList;

class LineModifierModel: public TreeModel {
public:
  LineModifierModel(const QStringList& headers = QStringList(), const QString& data = QString(), QObject* parent = 0);
  ~LineModifierModel();

private:
};

#endif // LINEMODIFIERMODEL_H
