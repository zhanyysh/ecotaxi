#include "greenbackgrounddelegate.h"
#include <QApplication>

GreenBackgroundDelegate::GreenBackgroundDelegate(QObject *parent)
    : QStyledItemDelegate(parent) {}

void GreenBackgroundDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QVariant value = index.data(Qt::DisplayRole);
    bool isZero = false;

    // Check if the value in the current cell is zero
    if (value.canConvert<double>()) {
        isZero = qFuzzyIsNull(value.toDouble());
    } else if (value.canConvert<int>()) {
        isZero = (value.toInt() == 0);
    } else {
        isZero = (value.toString().trimmed() == "0");
    }

    // Check the value in column 2 of the same row
    QModelIndex rentColumnIndex = index.sibling(index.row(), 2);  // Column 2
    QString rentValue = index.model()->data(rentColumnIndex).toString();
    bool isRent = (rentValue.trimmed() == "Аренда");

    QStyleOptionViewItem newOption = option;

    // Only apply green background if both conditions are true
    if (isZero && isRent) {
        newOption.backgroundBrush = QColor(0, 119, 0);  // Green background
    }

    // Draw the background
    QApplication::style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &newOption, painter);

    // Draw the text
    QString text = value.toString();
    painter->setPen(option.palette.color(QPalette::Text));
    painter->drawText(option.rect, option.displayAlignment, text);
}
