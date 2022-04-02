#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "datainterchange_global.h"
#include "tcpclient.h"

#include <QSharedPointer>
#include <QDebug>
#include <QPointer>

class TcpServer : public QTcpServer
{
    Q_OBJECT

public:
    explicit TcpServer(QObject *parent=nullptr);
    ~TcpServer();

    ///
    /// \brief InitializationParameter 初始化参数
    ///
    void InitializationParameter();

    ///
    /// \brief setServiceType 设置TCP模式
    /// \param serviceType
    ///
    void setServiceType(int serviceType);

    ///
    /// \brief incomingConnection 重写客户端接入
    /// \param handle
    ///
    void incomingConnection(qintptr socketID);

private:

    QString eol;

    ///
    /// \brief serviceType TCP模式
    ///
    int serviceType;

    ///
    /// \brief clientSocketIdMap 客户端字典
    ///
    QMap<qintptr,TcpClient*> clientSocketIdMap;

    ///
    /// \brief clientChannelMap 通道字典
    ///
    QMultiMap<int,qintptr> clientChannelMap;

    ///
    /// \brief pTimerSendHeartPack 定时发送心跳包
    ///
    QTimer *pTimerSendHeartPack;

    ///
    /// \brief isHeartPack 发送心跳包状态
    ///
    bool isHeartPack;

    ///
    /// \brief resultOfMemory 保存上次发送的数据
    ///
    QString resultOfMemory;

    ///
    /// \brief newline 换行符
    ///
    int newline;

public slots:

    ///
    /// \brief disconnectedSlot 客户端断开信号(从客户端列表删除断开的客户端)
    ///
    void disconnectedSlot();

    ///
    /// \brief setClientChannelNumberSlot 设置通道号
    /// \param channel_number 通道号
    /// \param socktID 客户端ID
    ///
    void setClientChannelNumberSlot(int channel_number, qintptr socketID);

    ///
    /// \brief getLastResultSlot 获取最后一次结果
    /// \param socktID
    ///
    void getLastResultSlot(qintptr socktID);

    ///
    /// \brief toSendDataSlot 发送识别结果
    /// \param channel_number 通道号
    /// \param result 识别结果
    ///
    void toSendDataSlot(int channel_number,const QByteArray& result);

    ///
    /// \brief heartbeatSlot 心跳检测
    ///
    void heartbeatSlot();

    ///
    /// \brief setHeartPacketStateSlot 设置心跳包状态
    /// \param state
    ///
    void setHeartPacketStateSlot(bool state);

    ///
    /// \brief releaseResourcesSlot 释放动资源
    ///
    void releaseResourcesSlot();

    ///
    /// \brief slot_setMessageFormat 设置数据格式
    /// \param newline
    ///
    void slot_setMessageFormat(int newline);

signals:

    ///
    /// \brief messageSignal 日志信息
    /// \param type 日志类型
    /// \param msg 信息体
    ///
    void messageSignal(const QString &type,const QString &msg);

    ///
    /// \brief connectCountSignal 链接数量
    /// \param count
    ///
    void connectCountSignal(int count);

    ///
    /// \brief signal_sendDataSuccToLog 发送结果成功写入UI和log
    /// \param channel_number
    /// \param result
    ///
    void signal_sendDataSuccToLog(int channel_number, const QString &result);

    ///
    /// \brief linkStateSingal TCP链接状态
    /// \param address 地址
    /// \param state 状态
    ///
    void linkStateSingal(const QString &address,quint16 port,bool state);

    ///
    /// \brief signal_lifting 抬杆信号
    ///
    void signal_lifting();

    ///
    /// \brief toSendDataSignal 发送识别结果,内部调用
    /// \param channel 通道号
    /// \param result 识别结果
    ///
    void toSendDataSignal(int channel_number,const QString& result);
};

#endif // TCPSERVER_H
