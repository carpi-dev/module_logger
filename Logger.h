//
// Created by insane on 11.07.20.
//

#ifndef CARPI_QT_SYSTEMINFO_H
#define CARPI_QT_SYSTEMINFO_H

#ifdef LOGGER_LOADABLE
#include "Logger_global.h"
#endif

#include <QDir>
#include <QFile>
#include <QDebug>
#include <QWidget>
#include <QSettings>
#include <QDateTime>
#include <QStandardPaths>

#define KEY_LOG_DIRECTORY "logDirectory"
#define KEY_LOG_DATE_FORMAT "logDateFormat"
#define KEY_LOG_MAX_LINES "logMaxLines"
#define KEY_LOG_PRIORITY "logPriority"

#define DATETIME_FORMAT "dd.MM.yyyy hh:mm:ss"
#define TAG_ERROR   " ERROR "
#define TAG_WARNING "WARNING"
#define TAG_INFO    " INFO  "
#define TAG_DEBUG   " DEBUG "

#ifdef LOGGER_LOADABLE
QT_BEGIN_NAMESPACE
namespace Ui { class Logger; }
QT_END_NAMESPACE
#endif

enum Priority {
    LOG_DEBUG = 0, LOG_VERBOSE, LOG_WARNING, LOG_ERROR
};

class Logger : public QWidget
{
Q_OBJECT
private:
#ifdef LOGGER_LOADABLE
    Ui::Logger *ui;
#endif
    QSettings *settings;
    QString logDirectory;
    QString logDateFormat;
    int logMaxLines;
    int logCurrentLines;
    Priority logPriority;
    QFile *logFile;
    QTextStream *logStream;

public:
    explicit Logger(QWidget *parent = nullptr);
    ~Logger() override;

    void log(const QString& module, Priority priority, const QString& msg);
    void error(const QString& module, const QString& msg);
    void warning(const QString& module, const QString& msg);
    void info(const QString& module, const QString& msg);
    void debug(const QString& module, const QString& msg);

    void getCurrentLogFile(int suffix=0);
};

#ifdef LOGGER_LOADABLE
extern "C" {
    LOGGER_EXPORT char* getName();
    LOGGER_EXPORT QWidget* create();
    LOGGER_EXPORT QStringList getSettingsKeys();
    LOGGER_EXPORT QStringList getDependencies();
    LOGGER_EXPORT QSettings *getSettings();
};
#endif

#endif //CARPI_QT_SYSTEMINFO_H
