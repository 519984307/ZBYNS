#ifndef LOG_H
#define LOG_H

#include <QObject>
#include <QTime>
#include <QDateTime>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QReadWriteLock>

class Log : public QObject
{
    Q_OBJECT

public:
    explicit Log(QString App, QObject *parent = nullptr);

    ///
    /// \brief outPutMessage 输出日志
    /// \param type 日志类型
    /// \param context 日志附加信息
    /// \param msg 日志信息
    ///
    void outPutMessage(QtMsgType type,const QMessageLogContext &context,const QString &msg);

signals:

    ///
    /// \brief singal_newLogText 新日志信息
    /// \param value 信息体
    ///
    void signal_newLogText(QtMsgType type,QDateTime time,QString value);

private slots:

    ///
    /// \brief slot_addLog 添加日志信息(回写到UI)
    /// \param value
    ///
    void slot_addLog(QtMsgType type,QDateTime time,QString value);

private:

    ///
    /// \brief writeToLog 日志写入本地
    /// \param value
    ///
    void writeToLog(QString value);

    ///
    /// \brief logFile 日志文件路径
    ///
    QFile logFile;

    QString AppName;

    QReadWriteLock lock;
};

#endif // LOG_H
