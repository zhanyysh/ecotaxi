#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QMutex>
#include <QDir>

class Logger : public QObject
{
    Q_OBJECT

public:
    // Singleton instance accessor
    static Logger* instance();

    // Log different types of messages
    void logInfo(const QString &message);
    void logWarning(const QString &message);
    void logError(const QString &message);

private:
    // Private constructor to ensure Singleton
    explicit Logger(QObject *parent = nullptr);
    ~Logger();

    QFile logFile;
    QTextStream logStream;
    QMutex mutex;  // To ensure thread-safety

    // Utility method to log message with a timestamp
    void logMessage(const QString &message, const QString &level);

    // Method to generate the log file name and ensure logs directory exists
    QString generateLogFileName() const;

    // Static instance pointer
    static Logger* m_instance;
    static QMutex m_instanceMutex;  // Mutex to guard instance creation
};

#endif // LOGGER_H
