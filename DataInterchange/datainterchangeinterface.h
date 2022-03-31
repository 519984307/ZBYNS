#ifndef DATAINTERCHANGEINTERFACE_H
#define DATAINTERCHANGEINTERFACE_H

#include <QObject>

class DataInterchangeInterface : public QObject
{
    Q_OBJECT
public:
    ~ DataInterchangeInterface(){}

    ///
    /// \brief InterfaceType 插件类型，多插件调用统一接口
    /// \return
    ///
    virtual QString InterfaceType()=0;

signals:

    ///
    /// \brief messageSignal 日志信息
    /// \param type 日志类型
    /// \param msg 信息体
    ///
    void messageSignal(const QString &type,const QString &msg);

    ///
    /// \brief linkStateSingal TCP链接状态
    /// \param address 地址
    /// \param state 状态
    ///
    void linkStateSingal(const QString &address,quint16 port,bool state);

    ///
    /// \brief connectCountSignal 链接数量
    /// \param count
    ///
    void connectCountSignal(int count);

    ///
    /// \brief toSendDataSignal 发送识别结果,内部调用
    /// \param channel 通道号
    /// \param result 识别结果
    ///
    void toSendDataSignal(int channel_number,const QString& result);

    ///
    /// \brief setHeartbeatPackStateSignal 心跳包状态
    /// \param state
    ///
    void setHeartbeatPackStateSignal(bool state);

    ///
    /// \brief signal_sendDataSuccToLog 发送结果成功写入UI和log
    /// \param channel_number
    /// \param result
    ///
    void signal_sendDataSuccToLog(int channel_number,  QString result);

    ///
    /// \brief signal_lifting 抬杆信号
    ///
    void signal_lifting();

public slots:

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
    virtual void  InitializationParameterSlot(const QString& address,const quint16& port,const int& serviceType,const bool& heartBeat, const int& serviceMode,const int& shortLink,const int& newline)=0;

    ///
    /// \brief toSendDataSlot 发送数据
    /// \param data 数据体
    ///
    virtual void toSendDataSlot(int channel_number, const QString &data)=0;

    ///
    /// \brief releaseResourcesSlot 释放资源
    ///
    virtual void releaseResourcesSlot()=0;

};

#define DataInterchangeInterfaceIID "ZBY.ContainerServer.DataInterchangeInterface/1.2.4.4"
Q_DECLARE_INTERFACE(DataInterchangeInterface,DataInterchangeInterfaceIID);

#endif // DATAINTERCHANGEINTERFACE_H
