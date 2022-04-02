#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include "datainterchange_global.h"

class TcpClient : public QTcpSocket
{
    Q_OBJECT

public:
    explicit TcpClient(QObject *parent=nullptr);

private:

    ///
    /// \brief channel_number 通道号
    ///
    int channel_number;

signals:

    ///
    /// \brief getLastResultSignal 获取最后一次结果
    /// \param socktID
    ///
    void getLastResultSignal(qintptr socktID);

    ///
    /// \brief setClientChannelNumberSignal 设置通道号
    /// \param channel_number 通道号
    /// \param socktID 客户端ID
    ///
    void setClientChannelNumberSignal(int channel_number,qintptr socktID);

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

public slots:

    ///
    /// \brief receiveClientDataSlot 接收数据
    ///
    void receiveDataSlot();
};

#endif // TCPCLIENT_H
