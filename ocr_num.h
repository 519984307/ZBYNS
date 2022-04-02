#ifndef OCR_NUM_H
#define OCR_NUM_H

#include <QObject>
#include <QtNetwork/QTcpSocket>
#include <QTimer>
#include <QDir>

class OCR_NUM : public QObject
{
    Q_OBJECT
public:
    explicit OCR_NUM(QObject *parent = nullptr);
    ~OCR_NUM();

    ///
    /// \brief isConnected 客户端链接状态
    ///
    bool isConnected;

signals:

    ///
    /// \brief decectSatusSignal 识别器状态
    /// \param status
    ///
    void decectSatusSignal(bool status);

    ///
    /// \brief decectRstSignal 识别结果
    /// \param rst
    ///
    void decectRstSignal(const QString& rst);

public slots:

    ///
    /// \brief decectTcpImageSlot TCP识别箱号
    /// \param image
    ///
    void decectTcpImageSlot(const QString& image ,const int& imgNum);

    ///
    /// \brief releaseResourcesSlot 释放资源
    ///
    void releaseResourcesSlot();

private:

    ///
    /// \brief pTcpClient 客户端
    ///
    QTcpSocket *pTcpClient;

    ///
    /// \brief pTimerAutoLink 定时自动链接
    ///
    QTimer *pTimerAutoLink;

    ///
    /// \brief pTimerLinkState 定时心跳包检测
    ///
    QTimer *pTimerLinkState;   

private slots:

    ///
    /// \brief startLinkSlot 链接到服务器,客户端模式
    ///
    void startLinkSlot();

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

};

#endif // OCR_NUM_H
