#ifndef PDFMANAGER_H
#define PDFMANAGER_H

#include <QCoreApplication>
#include <QTextDocument>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QMimeData>
#include <QApplication>
#include <QClipboard>

#include <QPdfWriter>
#include <QPrinter>
#include <QStandardPaths>

#include <QDate>

#include "qstandarditemmodel.h"
#include "../scripts/columnselectiondialog.h"

class PDFmanager
{
public:
    PDFmanager();

    // general
    void static exportToPDF(QString title, QString dates, QList<QAbstractItemModel *> models, int start = 1);

    void static ToPDF(QString title, QString dates, QList<QAbstractItemModel *> models, int start = 1);

    QString static modelToHTML(QAbstractItemModel *model, int start);

private:
    QString static getAppDir();
    QString static getDesktopDir();

    QString static getStyleSheet();

    QString static getHeader(QDateTime time);
    QString static getFooter(QDateTime time);

    void static createPDF(QString html, QString title);
};

#endif // PDFMANAGER_H
