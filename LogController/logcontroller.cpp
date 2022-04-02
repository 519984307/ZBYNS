#include "logcontroller.h"
#include "log.h"

#include <QSharedPointer>

QSharedPointer<Log> vklog;

void myMessageOutput( QtMsgType type, const QMessageLogContext &context, const QString &msg )
{
    vklog->outPutMessage( type, context, msg );
}

LogController::LogController(QString App, QObject *parent) : QObject(parent)
{
    this->setParent(parent);
    vklog=QSharedPointer<Log>(new Log(App));
    connect(vklog.data(),SIGNAL(signal_newLogText(QtMsgType,QDateTime ,QString)),this,SIGNAL(signal_newLogText(QtMsgType,QDateTime ,QString)));

    qInstallMessageHandler( myMessageOutput );
}
