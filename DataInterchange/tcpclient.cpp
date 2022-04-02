﻿#include "tcpclient.h"

TcpClient::TcpClient(QObject *parent):QTcpSocket(parent)
{
    this->setParent(parent);
}

void TcpClient::receiveDataSlot()
{
    QByteArray buf=readAll();

    if(buf.trimmed()=="cap=1"){
        emit toSendDataSignal(0,buf);
    }
    else {
        /*****************************
        * @brief:202105062343(接收到不是指定数据，源数据转发输出)
        ******************************/
        write(buf);
    }
    buf.clear();
}
