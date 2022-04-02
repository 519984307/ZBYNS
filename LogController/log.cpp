#include "log.h"

Log::Log(QString App, QObject *parent) : QObject(parent)
{
    this->setParent(parent);
    AppName=App;

    QDateTime dateTime = QDateTime::currentDateTime();
    QString stringDateTime = dateTime.toString( "yyyy_MM_dd_hh_mm_ss" );
    QString path = QStandardPaths::writableLocation( QStandardPaths::AppConfigLocation );

    QString logFolderName = path + "/" + "Debug_log";
    if( !QDir( logFolderName ).exists() )
    {
        QDir().mkpath( logFolderName );
    }

    logFile.setFileName( path + "/" + "Debug_log" + "/" + stringDateTime + ".log" );

    // If more 30 files, remove the last.
    QDir dir( logFolderName );
    dir.setFilter( QDir::Files );
    dir.setSorting( QDir::Name );
    QList<QFileInfo> list = dir.entryInfoList();
    if ( list.count() > 30 )
    {
        QFile file( list.at(0).absoluteFilePath() );
        file.remove();
    }

    connect( this, SIGNAL( signal_newLogText(QtMsgType,QDateTime ,QString ) ), this, SLOT( slot_addLog( QtMsgType,QDateTime ,QString) ) );
}

void Log::outPutMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    QString msgType=QString("");

    switch (type) {
    case QtDebugMsg:
        fprintf( stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function );
        msgType="Debug";
        break;
    case QtInfoMsg:
        fprintf( stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function );
        msgType="Info";
        break;
    case QtWarningMsg:
        fprintf( stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function );
        msgType="Warning";
        break;
    case QtCriticalMsg:
        fprintf( stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function );
        msgType="Critical";
        break;
    case QtFatalMsg:
        fprintf( stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function );
        msgType="Fatal";
        //abort();
    }
    fflush(stderr);

    QDateTime time=QDateTime::currentDateTime();
    writeToLog(QString("[%1] [%2] [%3] : %4").arg(AppName).arg(time.toString("yyyy-MM-dd hh:mm:ss zzz")).arg(msgType,8,QChar(' ')).arg(msg));

    emit signal_newLogText(type,time, msg );
}

void Log::slot_addLog(QtMsgType type, QDateTime time, QString value)
{
    Q_UNUSED(value)
    Q_UNUSED(time)
    Q_UNUSED(type)
}

void Log::writeToLog(QString value)
{
    lock.lockForWrite();

#ifdef Q_OS_LINUX
    QString eol = "\n";
#endif
#ifdef Q_OS_WIN
    QString eol = "\r\n";
#endif
    logFile.open( QIODevice::Append | QIODevice::Text | QIODevice::Unbuffered );
    logFile.write( value.toUtf8() );
    logFile.write( eol.toUtf8() );
    logFile.close();

    lock.unlock();
}
