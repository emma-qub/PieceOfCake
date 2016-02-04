#include "TestLevelTreeItemDelegate.h"
#include "TestLevelModel.h"

TestLevelTreeItemDelegate::TestLevelTreeItemDelegate(QObject* parent):
  QStyledItemDelegate(parent) {
}

QWidget* TestLevelTreeItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& /*option*/, const QModelIndex& index) const {
  if ((index.column() != 2 && index.column() != 3) || !index.parent().isValid())
    return NULL;

  _index = index;

  _editor = new QSpinBox(parent);
  _editor->setFrame(true);
  _editor->setMinimum(0);
  _editor->setMaximum(1000);

  connect(_editor, SIGNAL(valueChanged(int)), this, SLOT(updateModel(int)));
  connect(this, SIGNAL(commitData(QWidget*)), this, SLOT(dataCommited(QWidget*)), Qt::UniqueConnection);

  return _editor;
}

void TestLevelTreeItemDelegate::setEditorData(QWidget* /*editor*/, const QModelIndex& index) const {
  int value = index.model()->data(index, Qt::EditRole).toInt();

  _editor->setValue(value);
}

void TestLevelTreeItemDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& /*index*/) const {
  editor->setGeometry(option.rect);
}

void TestLevelTreeItemDelegate::updateModel(int value) {
  emit moveVertex(_index, value);
}

void TestLevelTreeItemDelegate::dataCommited(QWidget*) {
  emit moveVertex(_index, _editor->value(), true);
}
