#include "excelmanager.h"
#include <QTextStream>
#include <QDateTime>

ExcelManager::ExcelManager()
{
}

QString ExcelManager::getAppDir()
{
    return QCoreApplication::applicationDirPath();
}

QString ExcelManager::getDesktopDir()
{
    return QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
}

void ExcelManager::exportToExcel(QString title, QString type, QList<QAbstractItemModel *> models)
{
    createExcel(title, type, models);
}

void ExcelManager::createExcel(QString title, QString type, QList<QAbstractItemModel *> models)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

    QDateTime time = QDateTime::currentDateTime();
    QString appDir = getDesktopDir();
    QDir folder(appDir + "/отчеты");
    if (!folder.exists())
    {
        folder.mkdir(appDir + "/отчеты");
    }

    QString fileName = title + " " + type + " " + time.toString("dd.MM.yyyy HH-mm-ss") + ".csv";
    fileName.replace(" ", "_");

    QString filePath = appDir + "/отчеты/" + fileName;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(nullptr, "Ошибка", "Не удалось создать файл Excel");
        return;
    }

    QTextStream stream(&file);
    stream.setEncoding(QStringConverter::Utf8);

    // Write UTF-8 BOM
    stream << QChar(0xFEFF);

    // Write header
    stream << title << "\n";
    stream << type << "\n";
    stream << time.toString("dd.MM.yyyy HH:mm:ss") << "\n\n";

    // Write data for each model
    for (QAbstractItemModel* model : models)
    {
        // Write column headers
        for (int col = 0; col < model->columnCount(); ++col)
        {
            if (col > 0) stream << ";";
            stream << model->headerData(col, Qt::Horizontal).toString();
        }
        stream << "\n";

        // Write data rows
        for (int row = 0; row < model->rowCount(); ++row)
        {
            for (int col = 0; col < model->columnCount(); ++col)
            {
                if (col > 0) stream << ";";
                QString cellData = model->index(row, col).data(Qt::DisplayRole).toString();
                // Если есть ; или перевод строки или кавычки — экранируем
                if (cellData.contains(';') || cellData.contains('\n') || cellData.contains('"')) {
                    cellData.replace("\"", "\"\"");
                    cellData = "\"" + cellData + "\"";
                }
                stream << cellData;
            }
            stream << "\n";
        }
        stream << "\n";
    }

    file.close();

    QMimeData *mimeData = new QMimeData();
    mimeData->setUrls({QUrl::fromLocalFile(filePath)});

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setMimeData(mimeData);

    QApplication::restoreOverrideCursor();

    QMessageBox popup;
    popup.setTextFormat(Qt::MarkdownText);
    popup.setText("Отчет сохранен в папке отчеты на рабочем столе и скопирован в буфер обмена");
    popup.exec();
} 