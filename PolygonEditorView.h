#ifndef POLYGONTREELISTVIEW_H
#define POLYGONTREELISTVIEW_H

#include <QtWidgets>

class PolygonEditorView : public QAbstractItemView {
    Q_OBJECT

public:
    explicit PolygonEditorView(QWidget* parent = 0);

    virtual void setModel(QAbstractItemModel* model);
    void calculateRectsIfNecessary(void) const;
    virtual QRect visualRect(const QModelIndex &index) const;
    virtual bool isIndexHidden(const QModelIndex&) const { return false; }
    virtual void scrollTo(const QModelIndex& index, QAbstractItemView::ScrollHint);
    virtual QModelIndex indexAt(const QPoint& p) const;

    virtual void rowsInserted(const QModelIndex& parent, int start, int end);
    virtual void rowsAboutToBeRemoved(const QModelIndex& parent, int start, int end);
    QModelIndex moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers);
    virtual int horizontalOffset() const;
    virtual int verticalOffset() const;
    virtual void scrollContentsBy(int dx, int dy);
    virtual void setSelection(const QRect &rect, QFlags<QItemSelectionModel::SelectionFlag> flags);
    virtual QRegion visualRegionForSelection(const QItemSelection& selection) const;
    virtual void paintEvent(QPaintEvent*);
    void paintRect(const QRectF& rect, const QModelIndex& index, QPainter *painter);
    virtual void resizeEvent(QResizeEvent*);
    virtual void updateGeometries(void);

public slots:
    virtual void dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight);

private:
    mutable int _idealHeight;
    mutable int _idealXWidth;
    mutable int _idealYWidth;
    mutable QHash<int, QRectF> _rectForRow;
    mutable QHash<int, QModelIndex> _indexForRow;
    mutable bool _hashIsDirty;
};

#endif // POLYGONTREELISTVIEW_H
