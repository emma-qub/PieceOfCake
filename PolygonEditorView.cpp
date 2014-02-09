#include "PolygonEditorView.h"

PolygonEditorView::PolygonEditorView(QWidget *parent) :
    QAbstractItemView(parent),
    _idealHeight(0),
    _idealXWidth(0),
    _idealYWidth(0),
    _rectForRow(),
    _hashIsDirty(false) {

    setFocusPolicy(Qt::WheelFocus);
    setFont(QApplication::font("QListView"));
    horizontalScrollBar()->setRange(0, 0);
    verticalScrollBar()->setRange(0, 0);
}

void PolygonEditorView::setModel(QAbstractItemModel* model) {
    QAbstractItemView::setModel(model);
    _hashIsDirty = true;
}

void PolygonEditorView::calculateRectsIfNecessary(void) const {
    if (!_hashIsDirty)
        return;

    _rectForRow.clear();
    _indexForRow.clear();

    const int extraHeight = 4;
    const int extraWidth = 30;

    QFontMetrics fm(font());
    const int rowHeight = fm.height() + extraHeight;
    int rowXWidth = 0;
    int rowYWidth = 0;
    int rowPWidth = 0;
    int rowVWidth = 0;

    for (int k = 0; k < model()->rowCount(); k++) {
        _idealHeight += rowHeight;

        QModelIndex index = model()->index(k, 0, rootIndex());

        QString polygonString = index.data(Qt::DisplayRole).toString();
        rowPWidth = qMax(rowPWidth, fm.width(polygonString));

        for (int i = 0; i < model()->rowCount(index); i++) {
            _idealHeight += rowHeight;

            QModelIndex childIndex = model()->index(i, 0, index);

            QString vertexName = model()->data(childIndex, Qt::DisplayRole).toString();
            rowVWidth = qMax(rowVWidth, fm.width(vertexName));

            QString vertexXString = model()->index(0, 0, childIndex).data(Qt::DisplayRole).toString();
            QString vertexYString = model()->index(1, 0, childIndex).data(Qt::DisplayRole).toString();

            rowXWidth = qMax(rowXWidth, fm.width(vertexXString));
            rowYWidth = qMax(rowYWidth, fm.width(vertexYString));
        }
    }

    int totalPolygons = 0;
    int totalVertices = 0;

    rowPWidth += extraWidth;
    rowXWidth += extraWidth;
    rowYWidth += extraWidth;

    for (int k = 0; k < model()->rowCount(); k++) {
        QModelIndex index = model()->index(k, 0, rootIndex());
        _rectForRow.insert(_rectForRow.size(), QRectF(0, (totalPolygons+totalVertices/2)*rowHeight, rowPWidth, rowHeight));
        _indexForRow.insert(_indexForRow.size(), index);

        totalPolygons++;
        for (int i = 0; i < model()->rowCount(index); i++) {
            int leftShift = 20+rowVWidth+20+fm.width("X:");
            QRectF rect;

            QModelIndex childIndex = model()->index(i, 0, index);

            if (!model()->index(0, 0, childIndex).data(Qt::DisplayRole).toString().isEmpty()) {
                rect = QRectF(leftShift, (totalPolygons+totalVertices/2)*rowHeight, rowXWidth, rowHeight);
                _rectForRow.insert(_rectForRow.size(), rect);
                _indexForRow.insert(_indexForRow.size(), model()->index(0, 0, childIndex));
            }

            totalVertices++;
            if (!model()->index(1, 0, childIndex).data(Qt::DisplayRole).toString().isEmpty()) {
                leftShift += rowXWidth+fm.width("Y:");
                rect = QRectF(leftShift, (totalPolygons+totalVertices/2)*rowHeight, rowYWidth, rowHeight);
                _rectForRow.insert(_rectForRow.size(), rect);
                _indexForRow.insert(_indexForRow.size(), model()->index(1, 0, childIndex));
            }

            totalVertices++;
        }
    }

    _idealHeight = rowHeight;
    _idealXWidth = rowXWidth + extraWidth;
    _idealYWidth = rowYWidth + extraWidth;
    _hashIsDirty = false;
}

QRect PolygonEditorView::visualRect(const QModelIndex& index) const {
    QRect rect;

    if (index.isValid()) {
        calculateRectsIfNecessary();

        QHashIterator<int, QModelIndex> it(_indexForRow);
        while (it.hasNext()) {
            it.next();
            if (it.value() == index)
                return _rectForRow.value(it.key()).toRect();
        }
    }

    return rect;
}

void PolygonEditorView::scrollTo(const QModelIndex &index, QAbstractItemView::ScrollHint) {
    QRect viewRect = viewport()->rect();
    QRect itemRect = visualRect(index);

    if (itemRect.left() < viewRect.left())
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() + itemRect.left() - viewRect.left());

    else if (itemRect.right() > viewRect.right())
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() + qMin(itemRect.right() - viewRect.right(), itemRect.left() - viewRect.left()));

    if (itemRect.top() < viewRect.top())
        verticalScrollBar()->setValue(verticalScrollBar()->value() +
                                      itemRect.top() - viewRect.top());
    else if (itemRect.bottom() > viewRect.bottom())
        verticalScrollBar()->setValue(verticalScrollBar()->value() + qMin(itemRect.bottom() - viewRect.bottom(), itemRect.top() - viewRect.top()));

    viewport()->update();
}

QModelIndex PolygonEditorView::indexAt(const QPoint& p) const {
    QPoint point(p);

    point.rx() += horizontalScrollBar()->value();
    point.ry() += verticalScrollBar()->value();

    calculateRectsIfNecessary();

    QHashIterator<int, QRectF> it(_rectForRow);
    while (it.hasNext()) {
        it.next();
        if (it.value().contains(point)) {
            QModelIndex index = _indexForRow.value(it.key());
            // If it's a vertex
            if (index.parent() != QModelIndex())
                return index;
            // If it's a polygon
            else
                return QModelIndex();
        }
    }

    return QModelIndex();
}

void PolygonEditorView::dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight) {
    _hashIsDirty = true;
    QAbstractItemView::dataChanged(topLeft, bottomRight);
}

void PolygonEditorView::rowsInserted(const QModelIndex& parent, int start, int end) {
    _hashIsDirty = true;
    QAbstractItemView::rowsInserted(parent, start, end);
}

void PolygonEditorView::rowsAboutToBeRemoved(const QModelIndex& parent, int start, int end) {
    _hashIsDirty = true;
    QAbstractItemView::rowsAboutToBeRemoved(parent, start, end);
}

QModelIndex PolygonEditorView::moveCursor(QAbstractItemView::CursorAction /*cursorAction*/, Qt::KeyboardModifiers) {
    QModelIndex index = currentIndex();

//    if (index.isValid()) {
//        if ((cursorAction == MoveLeft && index.row() > 0) || (cursorAction == MoveRight && index.row() + 1 < model()->rowCount())) {
//            const int offset = (cursorAction == MoveLeft ? -1 : 1);
//            index = model()->index(index.row() + offset, index.column(), index.parent());
//        } else if ((cursorAction == MoveUp && index.row() > 0) || (cursorAction == MoveDown && index.row() + 1 < model()->rowCount())) {
//            QFontMetrics fm(font());
//            const int RowHeight = (fm.height() + 4) * (cursorAction == MoveUp ? -1 : 1);
//            QRect rect = _rectForRow.value(index.row()).toRect();
//            QPoint point(rect.center().x(), rect.center().y() + RowHeight);
//            while (point.x() >= 0) {
//                index = indexAt(point);
//                if (index.isValid())
//                    break;
//                point.rx() -= fm.width("n");
//            }
//        }
//    }
    return index;
}

int PolygonEditorView::horizontalOffset() const {
    return horizontalScrollBar()->value();
}

int PolygonEditorView::verticalOffset() const {
    return verticalScrollBar()->value();
}

void PolygonEditorView::scrollContentsBy(int dx, int dy) {
    scrollDirtyRegion(dx, dy);
    viewport()->scroll(dx, dy);
}

void PolygonEditorView::setSelection(const QRect& rect, QFlags<QItemSelectionModel::SelectionFlag> flags) {
    QRect rectangle = rect.translated(horizontalScrollBar()->value(), verticalScrollBar()->value()).normalized();
    calculateRectsIfNecessary();

    int firstRow = model()->rowCount();
    int lastRow = -1;

    QHashIterator<int, QRectF> i(_rectForRow);
    while (i.hasNext()) {
        i.next();
        if (i.value().intersects(rectangle)) {
            firstRow = firstRow < i.key() ? firstRow : i.key();
            lastRow = lastRow > i.key() ? lastRow : i.key();
        }
    }

    if (firstRow != model()->rowCount() && lastRow != -1) {
        QItemSelection selection(model()->index(firstRow, 0, rootIndex()), model()->index(lastRow, 0, rootIndex()));
        selectionModel()->select(selection, flags);
    } else {
        QModelIndex invalid;
        QItemSelection selection(invalid, invalid);
        selectionModel()->select(selection, flags);
    }
}

QRegion PolygonEditorView::visualRegionForSelection(const QItemSelection& selection) const {
    QRegion region;

    foreach (const QItemSelectionRange& range, selection) {
        for (int row = range.top(); row <= range.bottom(); ++row) {
            for (int column = range.left(); column < range.right(); ++column) {
                QModelIndex index = _indexForRow.value(row);
//                QModelIndex index = model()->index(row, column, rootIndex());
                region += visualRect(index);
            }
        }
    }

    return region;
}

void PolygonEditorView::paintEvent(QPaintEvent*) {
    QPainter painter(viewport());
    painter.setRenderHints(QPainter::Antialiasing| QPainter::TextAntialiasing);

    _hashIsDirty = true;
    calculateRectsIfNecessary();

    int totalPolygons = 0;
    int totalVertices = 0;

    QFontMetrics fm(font());
    int coordsXWidth = fm.width("X:");
    int coordsYWidth = fm.width("Y:");

    for (int row = 0; row < model()->rowCount(rootIndex()); ++row) {
        QModelIndex polygonIndex = model()->index(row, 0, rootIndex());
        QRectF rect = _rectForRow.value(totalPolygons+totalVertices);

        totalPolygons++;

        for (int i = 0; i < model()->rowCount(polygonIndex); i++) {
            QModelIndex vertexIndex = model()->index(i, 0, polygonIndex);
            QModelIndex vertexXIndex = model()->index(0, 0, vertexIndex);

            QRectF childRect = _rectForRow.value(totalPolygons+totalVertices);

            QString vertexName = vertexIndex.data(Qt::DisplayRole).toString();
            painter.drawText(QRectF(20, childRect.top()+2, childRect.left()-40, childRect.height()), vertexName);

            paintRect(childRect, vertexXIndex, &painter);
            painter.drawText(QRectF(childRect.left()-coordsXWidth, childRect.top()+2, coordsXWidth, childRect.height()), "X:");
//            painter.drawRect(childRect);

            totalVertices++;

            QModelIndex vertexYIndex = model()->index(1, 0, vertexIndex);
            childRect = _rectForRow.value(totalPolygons+totalVertices);

            paintRect(childRect, vertexYIndex, &painter);
            painter.drawText(QRectF(childRect.left()-coordsYWidth, childRect.top()+2, coordsYWidth, childRect.height()), "Y:");
//            painter.drawRect(childRect);

            totalVertices++;

        }

        paintRect(rect, polygonIndex, &painter);
//        painter.drawRect(rect);
    }
}

void PolygonEditorView::paintRect(const QRectF& rect, const QModelIndex& index, QPainter* painter) {
    if (!rect.isValid()/* || rect.bottom() < 0 || rect.y() > viewport()->height()*/)
        return;

    QStyleOptionViewItem option = viewOptions();
    option.rect = rect.toRect();

    if (selectionModel()->isSelected(index))
        option.state |= QStyle::State_Selected;

    if (currentIndex() == index)
        option.state |= QStyle::State_HasFocus;

    itemDelegate()->paint(painter, option, index);
}

void PolygonEditorView::resizeEvent(QResizeEvent*) {
    _hashIsDirty = true;
    calculateRectsIfNecessary();
    updateGeometries();
}

void PolygonEditorView::updateGeometries() {
    QFontMetrics fm(font());
    const int RowHeight = fm.height() + 4;

    horizontalScrollBar()->setSingleStep(fm.width("n"));
    horizontalScrollBar()->setPageStep(viewport()->width());
    horizontalScrollBar()->setRange(0, qMax(0, _idealXWidth+_idealYWidth - viewport()->width()));

    verticalScrollBar()->setSingleStep(RowHeight);
    verticalScrollBar()->setPageStep(viewport()->height());
    verticalScrollBar()->setRange(0, qMax(0, _idealHeight - viewport()->height()));
}
