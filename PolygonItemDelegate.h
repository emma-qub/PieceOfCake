#ifndef POLYGONITEMDELEGATE_H
#define POLYGONITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QtWidgets>

class PolygonItemDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    PolygonItemDelegate(QObject* parent = 0);

    virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;

    virtual void setEditorData(QWidget* editor, const QModelIndex& index) const;
//    virtual void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;

    virtual void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const;

public slots:
    void updateModel(int value);
    void dataCommited(QWidget*);

signals:
    void moveVertex(QModelIndex, int, bool editModel = false);

private:
    mutable QSpinBox* _editor;
    mutable QModelIndex _index;
};

#endif // POLYGONITEMDELEGATE_H
