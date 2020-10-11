//
// Created by insane on 11.07.20.
//

#include "Logger.h"
#ifdef LOGGER_LOADABLE
#include "ui_logger.h"
#endif

Logger::Logger(QWidget *parent): QWidget(parent),
#ifdef LOGGER_LOADABLE
ui(new Ui::Logger),
#endif
logCurrentLines(0)
{
#ifdef LOGGER_LOADABLE
    ui->setupUi(this);
#endif
    settings = new QSettings("carpi", "logger", this);
    logDirectory = settings->value(KEY_LOG_DIRECTORY, QStandardPaths::writableLocation(QStandardPaths::AppDataLocation).append("/log/")).toString();
    logDateFormat = settings->value(KEY_LOG_DATE_FORMAT, DATETIME_FORMAT).toString();
    logMaxLines = settings->value(KEY_LOG_MAX_LINES, 50000).toInt();
    logPriority = (Priority) settings->value(KEY_LOG_PRIORITY, (int) Priority::LOG_VERBOSE).toInt();
    getCurrentLogFile();
}

Logger::~Logger()
{
    if(logFile != nullptr){
        logFile->close();
        delete logFile;
    }
    delete logStream;
}

void Logger::log(const QString& module, Priority priority, const QString &msg) {
    QString level;
    switch (priority) {
        case LOG_DEBUG:
            level = TAG_DEBUG; break;
        case LOG_VERBOSE:
            level = TAG_INFO; break;
        case LOG_WARNING:
            level = TAG_WARNING; break;
        case LOG_ERROR:
            level = TAG_ERROR; break;
    }

    QString o = "{" + QDateTime::currentDateTime().toString(DATETIME_FORMAT) + "} - [" + level + "] [" + module + "] > " + msg;
    if(logPriority >= priority) qDebug() << o;

    *logStream << o;
}

void Logger::error(const QString& module, const QString &msg) {
    log(module, LOG_ERROR, msg);
}

void Logger::warning(const QString& module, const QString &msg) {
    log(module, LOG_WARNING, msg);
}

void Logger::info(const QString& module, const QString &msg) {
    log(module, LOG_VERBOSE, msg);
}

void Logger::debug(const QString& module, const QString &msg) {
    log(module, LOG_DEBUG, msg);
}

void Logger::getCurrentLogFile(int suffix) {
    QDir dir(logDirectory);
    if(!dir.exists()) dir.mkpath(logDirectory);
    logFile = new QFile(QDateTime::currentDateTime().toString(DATETIME_FORMAT)
            .replace(" ", "_").replace(".", "_").replace(":", "_").append(QString::number(suffix)).append(".log"));
    if(logFile->exists()) {
        delete logFile;
        getCurrentLogFile(suffix++);
    } else {
        logFile->open(QIODevice::WriteOnly);
        logStream = new QTextStream(logFile);
    }
}

#ifdef LOGGER_LOADABLE
extern "C" LOGGER_EXPORT QWidget* create() {
    return new Logger();
}

extern "C" LOGGER_EXPORT char* getName() {
    return (char*) "Logger";
}

extern "C" LOGGER_EXPORT QStringList getSettingsKeys(){
    return QStringList() << KEY_LOG_DIRECTORY << KEY_LOG_DATE_FORMAT;
}

extern "C" LOGGER_EXPORT QStringList getDependencies(){
    return QStringList() << "settings";
}
#endif