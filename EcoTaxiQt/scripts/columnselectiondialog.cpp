#include "ColumnSelectionDialog.h"

ColumnSelectionDialog::ColumnSelectionDialog(const QList<QAbstractItemModel*> &models, QString title, QString dates, int start, QWidget *parent)
    : QDialog(parent), models_(models)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    this->title = title;
    this->dates = dates;
    this->start = start;

    // Create checkboxes for each model's columns
    for (int j = 0; j < models_.size(); ++j) {
        QHBoxLayout *modelLayout = new QHBoxLayout();
        QList<QCheckBox*> modelCheckBoxes; // Store checkboxes for this model

        for (int i = 0; i < models_[j]->columnCount(); ++i) {
            QCheckBox *checkBox = new QCheckBox(models_[j]->headerData(i, Qt::Horizontal).toString());
            checkBox->setChecked(true);  // Check all checkboxes by default

            // Hide the first checkbox
            if (i == 0 && (start != 0 || j != 0)) {
                checkBox->hide(); // Hide the first checkbox
            }

            modelLayout->addWidget(checkBox);
            modelCheckBoxes.append(checkBox); // Append to the model's checkbox list
        }

        checkBoxes_.append(modelCheckBoxes); // Append the model's checkboxes to the 2D list
        layout->addLayout(modelLayout);
    }

    // Export button
    exportButton_ = new QPushButton("Экспорт", this);
    connect(exportButton_, &QPushButton::clicked, this, &ColumnSelectionDialog::onExportClicked);
    layout->addWidget(exportButton_);

    setLayout(layout);
    setWindowTitle("Выбор столбцов для экспорта");
}

QList<QList<int>> ColumnSelectionDialog::getSelectedColumns() const
{
    QList<QList<int>> selectedColumns;
    for (int i = 0; i < models_.size(); ++i) {
        QList<int> selected;
        for (int j = 0; j < models_[i]->columnCount(); ++j) {
            // Correctly access the checkboxes from the 2D list
            if (checkBoxes_[i][j]->isChecked()) {
                selected.append(j);
            }
        }
        selectedColumns.append(selected);
    }
    return selectedColumns;
}

void ColumnSelectionDialog::onExportClicked()
{
    QList<QList<int>> selectedColumns = getSelectedColumns();

    if (selectedColumns.isEmpty()) {
        QMessageBox::warning(this, "Предупреждение", "Пожалуйста, выберите хотя бы один столбец для экспорта.");
        return;
    }

    QList<QAbstractItemModel *> selectedModels; // Change type here to QAbstractItemModel*
    for (int i = 0; i < models_.size(); ++i) {
        QStandardItemModel *filteredModel = new QStandardItemModel();
        const QList<int> &selected = selectedColumns[i];

        // Set headers for the filtered model
        for (int columnIndex = 0; columnIndex < selected.size(); ++columnIndex) {
            int originalIndex = selected[columnIndex];
            filteredModel->setHorizontalHeaderItem(columnIndex, new QStandardItem(models_[i]->headerData(originalIndex, Qt::Horizontal).toString()));
        }

        // Add selected columns data to the filtered model
        for (int row = 0; row < models_[i]->rowCount(); ++row) {
            for (int columnIndex = 0; columnIndex < selected.size(); ++columnIndex) {
                int originalIndex = selected[columnIndex];
                filteredModel->setItem(row, columnIndex, new QStandardItem(models_[i]->index(row, originalIndex).data(Qt::DisplayRole).toString()));
            }
        }
        selectedModels.append(filteredModel); // This works because filteredModel is a QStandardItemModel*
    }

    PDFmanager::ToPDF(this->title, this->dates, selectedModels, start); // Now this will work correctly

    accept(); // Close the dialog
}
