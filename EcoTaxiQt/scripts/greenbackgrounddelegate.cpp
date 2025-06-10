#include "greenbackgrounddelegate.h"
#include <QApplication>

GreenBackgroundDelegate::GreenBackgroundDelegate(QObject *parent)
    : QStyledItemDelegate(parent) {}

void GreenBackgroundDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QVariant value = index.data(Qt::DisplayRole);
    bool isPositive = false;

    // Check if the value in the current cell is positive
    if (value.canConvert<double>()) {
        isPositive = (value.toDouble() > 0.0);
    } else if (value.canConvert<int>()) {
        isPositive = (value.toInt() > 0);
    } else {
        isPositive = (value.toString().trimmed().toDouble() > 0.0);
    }

    // Check the value in column 2 of the same row
    QModelIndex rentColumnIndex = index.sibling(index.row(), 2);  // Column 2
    QString rentValue = index.model()->data(rentColumnIndex).toString();
    bool isRent = (rentValue.trimmed() == "Аренда");

    QStyleOptionViewItem newOption = option;

    // Only apply green background if both conditions are true
    if (isPositive && isRent) {
        newOption.backgroundBrush = QColor(0, 119, 0);  // Green background
    }

    // Draw the background
    QApplication::style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &newOption, painter);

    // Draw the text
    QString text = value.toString();
    painter->setPen(option.palette.color(QPalette::Text));
    painter->drawText(option.rect, option.displayAlignment, text);
}
