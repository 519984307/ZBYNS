#ifndef DATAINTERCHANGE_H
#define DATAINTERCHANGE_H

#include "datainterchange_global.h"
#include "datainterchangeinterface.h"
#include "tcpserver.h"

class DATAINTERCHANGESHARED_EXPORT DataInterchange:public DataInterchangeInterface
{
    Q_OBJECT
    Q_INTERFACES(DataInterchangeInterface)
    Q_PLUGIN_METADATA(IID DataInterchangeInterfaceIID)

public:
    DataInterchange(QObject *parent=nullptr);
    ~DataInterchange()Q_DECL_OVERRIDE;

    ///
    /// \brief InterfaceType 插件类型，多插件调用统一接口
    /// \return
    ///
    QString InterfaceType()Q_DECL_OVERRIDE;

private:

    QString eol;

    ///
    /// \brief isConnected 客户端链接状态
    ///
    bool isConnected;

    ///
    /// \brief isHeartBeat 心跳包状态
    ///
    int isHeartBeat;

    ///
    /// \brief channel 通道号
    ///
    int channel;

    ///
    /// \brief address 地址
    ///
    QString address;

    ///
    /// \brief port 端口
    ///
    quint16 port;

    ///
    /// \brief pTcpClient 客户端
    ///
    QTcpSocket *pTcpClient;

    ///
    /// \brief pTcpServer 服务器
    ///
    TcpServer* pTcpServer;

    ///
    /// \brief pTimerLinkState 定时心跳包检测
    ///
    QTimer *pTimerLinkState;

    ///
    /// \brief pTimerAutoLink 定时自动链接
    ///
    QTimer *pTimerAutoLink;

    ///
    /// \brief resultOfMemory 保存上次发送的数据
    ///
    QString resultOfMemory;

    ///
    /// \brief shortLink 短链接状态
    ///
    int shortLink;

    ///
    /// \brief newline 换行符
    ///
    int newline;

private slots:

    ///
    /// \brief startLinkSlot 链接到服务器,客户端模式
    ///
    void startLinkSlot();

    ///
    /// \brief startListenSlot 开始监听,服务器模式
    ///
    void startListenSlot();

    ///
    /// \brief heartbeatSlot 心跳检测
    ///
    void heartbeatSlot();

    ///
    /// \brief connectedSlot 链接成功
    ///
    void connectedSlot();

    ///
    /// \brief receiveDataSlot 接收数据
    ///
    void receiveDataSlot();

    ///
    /// \brief disconnectedSlot 链接断开
    ///
    void disconnectedSlot();

    ///
    /// \brief displayErrorSlot 链接错误
    /// \param socketError 链接状态码
    ///
    void displayErrorSlot(QAbstractSocket::SocketError socketError);

    ///
    /// \brief setHeartPacketStateSlot 设置心跳包状态
    /// \param state
    ///
    void setHeartPacketStateSlot(bool state);

public:

    ///
    /// \brief InitializationParameterSlot 初始化参数
    /// \param address 地址
    /// \param port 端口
    /// \param serviceType 服务类型
    /// \param heartBeat 心跳包 状态
    /// \param serviceMode 服务模式
    /// \param shortLink 短链接状态
    /// \param newline 换行符
    ///
    void  InitializationParameterSlot(const QString& address, const quint16& port, const int& serviceType,const bool& heartBeat, const int& serviceMode,const int& shortLink,const int& newline)Q_DECL_OVERRIDE;

    ///
    /// \brief toSendDataSlot 发送数据
    /// \param data 数据体
    ///
    void toSendDataSlot(int channel_number,const QByteArray& data)Q_DECL_OVERRIDE;

    ///
    /// \brief releaseResourcesSlot 释放动资源
    ///
    void releaseResourcesSlot()Q_DECL_OVERRIDE;

signals:

    ///
    /// \brief signal_setMessageFormat 设置数据格式
    /// \param newline
    ///
    void signal_setMessageFormat(int newline);

    ///
    /// \brief signal_releaseResources 释放TCP服务资源
    ///
    void signal_releaseResources();
};

#endif // DATAINTERCHANGE_H
