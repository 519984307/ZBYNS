#include "ocr_num.h"

OCR_NUM::OCR_NUM(QObject *parent) : QObject(parent)
{
    this->setParent(parent);

    isConnected=false;

    pTcpClient=new QTcpSocket(this);
    pTimerAutoLink=new QTimer(this);
    pTimerAutoLink->setSingleShot(true);

    pTimerLinkState=new QTimer(this);

    connect(pTcpClient,&QIODevice::readyRead,this,&OCR_NUM::receiveDataSlot);
    connect(pTcpClient,&QAbstractSocket::connected,this,&OCR_NUM::connectedSlot);
    connect(pTcpClient,&QAbstractSocket::disconnected,this,&OCR_NUM::disconnectedSlot);
    connect(pTcpClient,QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),this,&OCR_NUM::displayErrorSlot);
    connect(pTimerLinkState,&QTimer::timeout,this,&OCR_NUM::heartbeatSlot);
    connect(pTimerAutoLink,&QTimer::timeout,this,&OCR_NUM::startLinkSlot);

    startLinkSlot();
}

OCR_NUM::~OCR_NUM()
{
    if(pTcpClient!=nullptr){
        pTcpClient->close();
        delete  pTcpClient;
        pTcpClient=nullptr;
    }
}

void OCR_NUM::decectTcpImageSlot(const QString &image, const int& imgNum)
{
    Q_UNUSED(imgNum);

    if(isConnected && pTcpClient->isOpen()){
        if(!image.startsWith("-m")){
            pTcpClient->write("-i "+image.toLatin1()+" ");
        }
        else {
            pTcpClient->write(image.toLatin1());
        }
    }
    else {
        emit decectRstSignal(QString("%1-%2").arg(QDir::fromNativeSeparators(image),"RESULT:||0"));
    }
}

void OCR_NUM::releaseResourcesSlot()
{
    if(pTcpClient!=nullptr){
        pTcpClient->close();
        delete  pTcpClient;
        pTcpClient=nullptr;
    }

    pTimerAutoLink->stop();
    delete  pTimerAutoLink;
    pTimerAutoLink=nullptr;

    pTimerLinkState->stop();
    delete pTimerLinkState;
    pTimerLinkState=nullptr;
}

void OCR_NUM::startLinkSlot()
{
    if(!isConnected){
        pTcpClient->close();
        pTcpClient->abort();
        pTcpClient->connectToHost("127.0.0.1",55550);
    }
}

void OCR_NUM::heartbeatSlot()
{
    if(pTcpClient->isOpen()){
        pTcpClient->write("");/* 心跳包数据 */
    }
}

void OCR_NUM::connectedSlot()
{
    isConnected=true;

    pTcpClient->write("-m 0");

    emit decectSatusSignal(isConnected);

    if(!pTimerLinkState->isActive()){
        pTimerLinkState->start(5000);
    }
    qInfo().noquote()<<QString("[%1] %2-%3:Connecting to the server succeeded").arg(this->metaObject()->className(),"127.0.0.1",QString::number(55550));
}

void OCR_NUM::receiveDataSlot()
{
    QByteArray buf=pTcpClient->readAll();
    emit decectRstSignal(buf.data());
    buf.clear();
}

void OCR_NUM::disconnectedSlot()
{
    isConnected=false;
    emit decectSatusSignal(isConnected);
}

void OCR_NUM::displayErrorSlot(QAbstractSocket::SocketError socketError)
{
    isConnected=false;
    emit decectSatusSignal(isConnected);

    pTimerAutoLink->start(15000);
    qWarning().noquote()<<QString("[%1] %2-%3:Link server error<errCode=%4>").arg(this->metaObject()->className(),"127.0.0.1",QString::number(55550),QString::number(socketError));
}
