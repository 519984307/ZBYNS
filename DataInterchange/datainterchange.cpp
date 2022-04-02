#include "datainterchange.h"
#include <QDebug>

DataInterchange::DataInterchange(QObject *parent)
{
    this->setParent(parent);

    isHeartBeat=false;
    isConnected=false;
    pTcpServer=nullptr;
    pTcpClient=nullptr;
    pTimerLinkState=nullptr;
    pTimerAutoLink=nullptr;

#ifdef Q_OS_LINUX
    eol = "\n";
#endif
#ifdef Q_OS_WIN
    eol = "\r\n";
#endif
}

DataInterchange::~DataInterchange()
{
}

QString DataInterchange::InterfaceType()
{
    return QString("TCP");
}

void DataInterchange::InitializationParameterSlot(const QString& address, const quint16& port, const int& serviceType,const bool& heartBeat, const int& serviceMode,const int& shortLink,const int& newline)
{
    this->address=address;
    this->port=port;
    this->shortLink=shortLink;
    this->newline=newline;

    if(serviceMode==1){/* 服务器模式 */
        pTcpServer=new TcpServer (this);
        pTcpServer->setServiceType(serviceType);/* 设置服务模式 */
        /* 日志信息 */
        connect(pTcpServer,&TcpServer::messageSignal,this,&DataInterchange::messageSignal);
        /* 心跳包状态设置 */
        connect(this,&DataInterchange::setHeartbeatPackStateSignal,pTcpServer,&TcpServer::setHeartPacketStateSlot);
        /* 绑定客户端数量 */
        connect(pTcpServer,&TcpServer::connectCountSignal,this,&DataInterchange::connectCountSignal);
        /* 发送识别结果 */
        connect(this,&DataInterchange::recvDataSignal,pTcpServer,&TcpServer::toSendDataSlot);
        /* 设置数据格式 */
        connect(this,&DataInterchange::signal_setMessageFormat,pTcpServer,&TcpServer::slot_setMessageFormat);
        /* 发送成功写入日志和UI */
        connect(pTcpServer,&TcpServer::signal_sendDataSuccToLog,this,&DataInterchange::signal_sendDataSuccToLog);
        /* tcp链接状态 */
        connect(pTcpServer,&TcpServer::linkStateSingal,this,&DataInterchange::linkStateSingal);
        /* tcp链接开闸 */
        connect(pTcpServer,&TcpServer::signal_lifting,this,&DataInterchange::signal_lifting);
        /* 释放资源 */
        connect(this,&DataInterchange::signal_releaseResources,pTcpServer,&TcpServer::releaseResourcesSlot);



        /* 发送识别结果,内部调用 */
        connect(pTcpServer,&TcpServer::toSendDataSignal,this,&DataInterchange::toSendDataSignal);

        startListenSlot();
    }
    else if (serviceMode==0) {/* 客户端模式 */
        pTcpClient=new QTcpSocket();
        pTimerLinkState=new QTimer(this);
        pTimerAutoLink=new QTimer(this);
        pTimerAutoLink->setSingleShot(true);

        /* 发送识别结果 */
        connect(this,&DataInterchange::recvDataSignal,this,&DataInterchange::toSendDataSlot);
        /* 心跳包状态设置 */
        connect(this,&DataInterchange::setHeartbeatPackStateSignal,this,&DataInterchange::setHeartPacketStateSlot);

        connect(pTcpClient,&QIODevice::readyRead,this,&DataInterchange::receiveDataSlot);
        connect(pTcpClient,&QAbstractSocket::connected,this,&DataInterchange::connectedSlot);
        connect(pTcpClient,&QAbstractSocket::disconnected,this,&DataInterchange::disconnectedSlot);
        connect(pTcpClient,QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),this,&DataInterchange::displayErrorSlot);
        connect(pTimerLinkState,&QTimer::timeout,this,&DataInterchange::heartbeatSlot);
        connect(pTimerAutoLink,&QTimer::timeout,this,&DataInterchange::startLinkSlot);

        /*****************************
        * @brief:不是短链接，服务器启动主动链接
        ******************************/
        if(!shortLink){
            startLinkSlot();
        }
    }

    emit setHeartbeatPackStateSignal(heartBeat);
    emit signal_setMessageFormat(newline);
}

void DataInterchange::startLinkSlot()
{
    if(!isConnected){
        pTcpClient->close();
        pTcpClient->abort();
        pTcpClient->connectToHost(address,port);
    }
}

void DataInterchange::startListenSlot()
{
    if(!pTcpServer->listen(QHostAddress::AnyIPv4,port)){/* 服务器使用本机地址 */
        qWarning().noquote()<<QString("IP:%1 Listen error<errocCode=%2>").arg(address).arg(pTcpServer->errorString());
    }
    else {
        qDebug().noquote()<<QString("IP:%1 Start listen.").arg(address);
    }
}

void DataInterchange::heartbeatSlot()
{
    if(isHeartBeat && pTcpClient->isOpen()){
        pTcpClient->write("[H]");/* 心跳包数据 */
    }
}

void DataInterchange::connectedSlot()
{
    isConnected=true;

    if(!pTimerLinkState->isActive()){
        pTimerLinkState->start(15000);
    }

    emit connectCountSignal(1);
    emit linkStateSingal(address,port,true);
    qDebug().noquote()<<QString("IP:%1:%2 link successful").arg(address).arg(port);
}

void DataInterchange::receiveDataSlot()
{
    QByteArray buf=pTcpClient->readAll();    
    if(buf.trimmed().startsWith("cap")){
        emit toSendDataSignal(channel,buf);
    }
    else {
        /*****************************
        * @brief:202105062343(接收到不是指定数据，源数据转发输出)
        ******************************/
        pTcpClient->write(buf);
    }
    buf.clear();
}

void DataInterchange::disconnectedSlot()
{
    isConnected=false;
    emit connectCountSignal(-1);
    emit linkStateSingal(address,port,false);
}

void DataInterchange::displayErrorSlot(QAbstractSocket::SocketError socketError)
{
    isConnected=false;

    //emit connectCountSignal(-1);
    emit linkStateSingal(address,port,false);

    if(!shortLink){
        //QTimer::singleShot(15000, this, SLOT(startLinkSlot()));
        pTimerAutoLink->start(15000);
    }
    qWarning().noquote()<<QString("IP:%1:%3  link error<errorCode=%2>").arg(address).arg(socketError).arg(port);
}

void DataInterchange::setHeartPacketStateSlot(bool state)
{
    isHeartBeat=state;
}

void DataInterchange::toSendDataSlot(int channel_number,const QByteArray &data)
{
    Q_UNUSED(channel_number);

    resultOfMemory=data;

    if(!pTcpClient->isOpen()){
        startLinkSlot();
        pTcpClient->waitForConnected(3000);
    }
    pTcpClient->write(data);
    if(newline){
        pTcpClient->write(eol.toUtf8());
    }

    emit signal_sendDataSuccToLog(channel_number,data);

    if(shortLink){
        /*****************************
        * @brief:发送完成主动断开
        ******************************/
        pTcpClient->waitForBytesWritten();
        pTcpClient->close();
        pTcpClient->abort();
    }
}

void DataInterchange::releaseResourcesSlot()
{
    if(pTcpServer!=nullptr && pTcpServer->isListening()){
        emit signal_releaseResources();
        pTcpServer->close();
    }

    if(pTcpClient != nullptr && pTcpClient->isOpen()){
        pTcpClient->disconnected();
        pTcpClient->close();
        pTcpClient->abort();
    }

    isConnected=false;

    if(pTimerLinkState!=nullptr){
        pTimerLinkState->stop();
    }
    if(pTimerAutoLink!=nullptr){
        pTimerAutoLink->stop();
    }

    qDebug().noquote()<<QString("DataInterchange::releaseResourcesSlot");
}
