#ifndef GREENBACKGROUNDDELEGATE_H
#define GREENBACKGROUNDDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>

// GreenBackgroundDelegate is a custom delegate that will paint the background green
// if the value in the cell is 0.
class GreenBackgroundDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    explicit GreenBackgroundDelegate(QObject *parent = nullptr);

    // Override the paint method to customize the cell's appearance
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // GREENBACKGROUNDDELEGATE_H
