#include "Logger.h"
#include <QCoreApplication>
#include <QDebug>

Logger* Logger::m_instance = nullptr;
QMutex Logger::m_instanceMutex;

Logger::Logger(QObject *parent) : QObject(parent)
{
    QString logFileName = generateLogFileName();
    logFile.setFileName(logFileName);

    if (!logFile.open(QIODevice::Append | QIODevice::Text)) {
        qWarning() << "Could not open log file:" << logFileName;
        return;
    }
    logStream.setDevice(&logFile);
}

Logger::~Logger()
{
    if (logFile.isOpen()) {
        logFile.close();
    }
}

Logger* Logger::instance()
{
    // Thread-safe lazy initialization of singleton instance
    if (!m_instance) {
        QMutexLocker locker(&m_instanceMutex);
        if (!m_instance) {
            m_instance = new Logger();
        }
    }
    return m_instance;
}

QString Logger::generateLogFileName() const
{
    // Get the application's directory path
    QString appDir = QCoreApplication::applicationDirPath();

    // Create a "logs" directory inside the application directory if it doesn't exist
    QDir logDir(appDir + "/logs");
    if (!logDir.exists()) {
        logDir.mkpath(".");
    }

    // Generate a log file name based on the current date and time
    QString currentDate = QDateTime::currentDateTime().toString("yyyy.MM.dd_HH-mm-ss");
    QString logFileName = QString("log_%1.txt").arg(currentDate);

    // Return the full path to the log file
    return logDir.filePath(logFileName);
}

void Logger::logInfo(const QString &message)
{
    logMessage(message, "INFO");
}

void Logger::logWarning(const QString &message)
{
    logMessage(message, "WARNING");
}

void Logger::logError(const QString &message)
{
    logMessage(message, "ERROR");
}

void Logger::logMessage(const QString &message, const QString &level)
{
    QMutexLocker locker(&mutex);  // Ensure thread-safety for logging
    if (logFile.isOpen()) {
        QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
        logStream << timestamp << " [" << level << "] " << message << "\n";
        logStream.flush();
    }
}
