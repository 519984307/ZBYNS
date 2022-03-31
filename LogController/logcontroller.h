#ifndef LOGCONTROLLER_H
#define LOGCONTROLLER_H

#include <QObject>
#include <QDateTime>
#include "LogController_global.h"

class LOGCONTROLLER_EXPORT LogController:public QObject
{   
    Q_OBJECT

public:
    LogController(QString App,QObject *parent = nullptr);

signals:
    ///
    /// \brief singal_newLogText 新日志信息
    /// \param value 信息体
    ///
    void signal_newLogText(QtMsgType type,QDateTime time,QString value);
};

#endif // LOGCONTROLLER_H
