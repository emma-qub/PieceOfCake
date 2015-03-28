#include "LineModifierModel.h"

LineModifierModel::LineModifierModel(const QStringList& headers, const QString& data, QObject* parent):
  TreeModel(headers, data, parent) {

}

LineModifierModel::~LineModifierModel()
{

}

