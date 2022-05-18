#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    loadParment();

    ui->actionStatus->setIconText("Not Link!");

    pDataBase=new DataBase(this);
    connect(this,&MainWindow::signalInitDataBase,pDataBase,&DataBase::initDataBaseSlot);
    connect(this,&MainWindow::signalInsertDataBase,pDataBase,&DataBase::insertDataBaseSlot);

    pLog=QPointer<LogController>(new LogController("ZBY_NS",this));
    connect(pLog.data(),SIGNAL(signal_newLogText(QtMsgType,QDateTime,QString)),&logFrom,SLOT(slot_newLogText(QtMsgType,QDateTime,QString)));

    pOCR_NUM=new OCR_NUM (this);
    connect(this,&MainWindow::signalDetectText,pOCR_NUM,&OCR_NUM::decectTcpImageSlot);
    connect(pOCR_NUM,&OCR_NUM::decectRstSignal,this,&MainWindow::slotlDetectResult);

    pCapCamera=new CaptureCamera(this);
    connect(this,&MainWindow::signalInitCamera,pCapCamera,&CaptureCamera::initCameraSlot);
    connect(this,&MainWindow::signalSetCaptureType,pCapCamera,&CaptureCamera::setCaptureTypeSlot);
    connect(this,&MainWindow::signalSimulationCapture,pCapCamera,&CaptureCamera::simulationCaptureSlot);
    connect(this,&MainWindow::signalOpenTheVideo,pCapCamera,&CaptureCamera::openTheVideoSlot);
    connect(pCapCamera,&CaptureCamera::signalPictureStream,this,&MainWindow::slotPictureStream);
    connect(pCapCamera,&CaptureCamera::signalCameraStatus,this,&MainWindow::slotCameraStatus);
    connect(pCapCamera,&CaptureCamera::signal_setCameraID,this,&MainWindow::slotSetCameraID);

    QDir plugin(QCoreApplication::applicationDirPath());
    for(const QString &fileName :plugin.entryList(QDir::Files)){
        QPluginLoader  pluginLoader(plugin.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();

        if(plugin){
            if (DataInterchangeInterface* pDataInterchangeInterface=qobject_cast<DataInterchangeInterface*>(plugin)) {
                if(pDataInterchangeInterface->InterfaceType()=="TCP"){
                    qDebug().noquote()<<QString("TCP plugin load sucess");
                    /* 初始化参数 */
                    connect(this,&MainWindow::TCP_InitializationParameterSignal,pDataInterchangeInterface,&DataInterchangeInterface::InitializationParameterSlot);
                    /* 接收数据 */
                    connect(pDataInterchangeInterface,&DataInterchangeInterface::toSendDataSignal,this,&MainWindow::socketReadDataSlot);
                    /* 绑定SOCKET数量到服务界面 */
                    connect(pDataInterchangeInterface,&DataInterchangeInterface::linkStateSingal,this,&MainWindow::TCP_socketLinkStateSlot);
                    /* 发送数据 */
                    connect(this,&MainWindow::signalToSendData,pDataInterchangeInterface,&DataInterchangeInterface::recvDataSignal);
                    /* 释放资源 */
                    connect(this,&MainWindow::releaseResourcesSignal,pDataInterchangeInterface,&DataInterchangeInterface::releaseResourcesSlot);
                }
            }
            else {
                pluginLoader.unload();
            }
        }
        else {
            delete plugin;
            plugin=nullptr;
        }
    }

    emit signalSetCaptureType(1,0);
    emit signalInitCamera("",camera1.at(0),37777,camera1.at(1),camera1.at(2),"11");
    emit signalInitCamera("",camera2.at(0),37777,camera2.at(1),camera2.at(2),"12");
    emit signalInitCamera("",camera3.at(0),37777,camera1.at(1),camera1.at(2),"21");
    emit signalInitCamera("",camera4.at(0),37777,camera2.at(1),camera2.at(2),"22");
    emit signalInitCamera("",camera5.at(0),37777,camera1.at(1),camera1.at(2),"31");
    emit signalInitCamera("",camera6.at(0),37777,camera2.at(1),camera2.at(2),"32");
    emit signalInitDataBase("ZBYNS","admin","ABCabc123","127.0.0.1",0);

    if(model==1){
        address="127.0.0.1";
    }
    /*****************************
    * @brief:初始化接口服务
    ******************************/
    emit TCP_InitializationParameterSignal(address,port,0,heart,model,0,0);

//    /*****************************
//    * @brief:设置识别器模式
//    ******************************/
//    emit signalDetectText("-m 0",-1);
//    ui->lineEdit_4->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
}

MainWindow::~MainWindow()
{
    delete pLog;
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(quit){
        QSharedPointer<LockDialog> lockDlg=QSharedPointer<LockDialog>(new LockDialog(this));
        if(lockDlg->exec()){
            event->accept();

            emit releaseResourcesSignal();
        }
        else {
            event->ignore();
        }
    }
    else
    {
        emit releaseResourcesSignal();
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    foreach(QString label,labelImgMap.keys()){
        QPixmap pix(QDir::toNativeSeparators(labelImgMap.value(label)));

        if(pix.isNull()){
            continue;
        }
        QPalette palette;

        palette.setBrush(QPalette::Background, QBrush(pix.scaled(ui->label->size(), Qt::IgnoreAspectRatio)));
        ui->label->setPalette(palette);

        if(label=="11"){
            palette.setBrush(QPalette::Background, QBrush(pix.scaled(ui->label->size(), Qt::IgnoreAspectRatio)));
            ui->label->setPalette(palette);
        }
        if(label=="12"){
            palette.setBrush(QPalette::Background, QBrush(pix.scaled(ui->label_7->size(), Qt::IgnoreAspectRatio)));
            ui->label_7->setPalette(palette);
        }

        if(label=="21"){
            palette.setBrush(QPalette::Background, QBrush(pix.scaled(ui->label_2->size(), Qt::IgnoreAspectRatio)));
            ui->label_2->setPalette(palette);
        }
        if(label=="22"){
            palette.setBrush(QPalette::Background, QBrush(pix.scaled(ui->label_8->size(), Qt::IgnoreAspectRatio)));
            ui->label_8->setPalette(palette);
        }

        if(label=="31"){
            palette.setBrush(QPalette::Background, QBrush(pix.scaled(ui->label_9->size(), Qt::IgnoreAspectRatio)));
            ui->label_9->setPalette(palette);
        }
        if(label=="32"){
            palette.setBrush(QPalette::Background, QBrush(pix.scaled(ui->label_10->size(), Qt::IgnoreAspectRatio)));
            ui->label_10->setPalette(palette);
        }
    }
}

void MainWindow::on_actionSetting_triggered()
{
    QSharedPointer<LockDialog> lockDlg=QSharedPointer<LockDialog>(new LockDialog(this));
    if(lockDlg->exec()){
        SettingDialog dlg;
        dlg.exec();
    }
}

void MainWindow::on_actionLog_triggered()
{
    if(logFrom.isVisible()){
        logFrom.setVisible(false);
    }
    else {
        logFrom.setVisible(true);
    }
}

void MainWindow::on_actionDataBase_triggered()
{
    dataForm=QSharedPointer<DataForm>(new DataForm(nullptr,imgPath));
    connect(dataForm.data(),&DataForm::signalSendResult,this,&MainWindow::signalToSendData);
    dataForm.data()->show();
}

void MainWindow::loadParment()
{
    QString path = QStandardPaths::writableLocation( QStandardPaths::AppConfigLocation);
    QSettings set(QDir::toNativeSeparators(QString("%1/Parment.ini").arg(path)),QSettings::IniFormat);
    set.setIniCodec("GBK");

    set.beginGroup("channel1");
    camera1.append(set.value("address1","192.168.1.101").toString());
    camera1.append(set.value("user1","admin").toString());
    camera1.append(set.value("password1","ABCabc123").toString());
    camera1.append(QString::number(set.value("camType1","0").toInt()));
    camera2.append(set.value("address2","192.168.1.101").toString());
    camera2.append(set.value("user2",3777).toString());
    camera2.append(set.value("password2","ABCabc123").toString());
    camera2.append(QString::number(set.value("camType2","0").toInt()));
    set.endGroup();

    set.beginGroup("channel2");
    camera3.append(set.value("address1","192.168.1.101").toString());
    camera3.append(set.value("user1","admin").toString());
    camera3.append(set.value("password1","ABCabc123").toString());
    camera3.append(QString::number(set.value("camType1","0").toInt()));
    camera4.append(set.value("address2","192.168.1.101").toString());
    camera4.append(set.value("user2",3777).toString());
    camera4.append(set.value("password2","ABCabc123").toString());
    camera4.append(QString::number(set.value("camType2","0").toInt()));
    set.endGroup();

    set.beginGroup("channel3");
    camera5.append(set.value("address1","192.168.1.101").toString());
    camera5.append(set.value("user1","admin").toString());
    camera5.append(set.value("password1","ABCabc123").toString());
    camera5.append(QString::number(set.value("camType1","0").toInt()));
    camera6.append(set.value("address2","192.168.1.101").toString());
    camera6.append(set.value("user2",3777).toString());
    camera6.append(set.value("password2","ABCabc123").toString());
    camera6.append(QString::number(set.value("camType2","0").toInt()));
    set.endGroup();

    set.beginGroup("Main");
    imgPath=set.value("imgPath","C:\\Images").toString();
    lock=set.value("lock","123456").toString();
    quit=set.value("quit",1).toInt();
    channel=set.value("channel",1).toInt();
    set.endGroup();

    set.beginGroup("Server");
    model = set.value("model",1).toInt();
    address = set.value("address","127.0.0.1").toString();
    port = set.value("port",55506).toInt();
    heart = set.value("heart",0).toInt();
    set.endGroup();

    count=0;
    capCount=0;
}

void MainWindow::on_actionLock_triggered()
{
    QSharedPointer<LockDialog> lockDlg=QSharedPointer<LockDialog>(new LockDialog(this));
    bool lock=true;
    while (lock) {
        if(lockDlg->exec()){
            lock=false;
        }
    }
}

void MainWindow::on_actionTest_triggered()
{
    QSharedPointer<TestDialog> testDlg=QSharedPointer<TestDialog>(new TestDialog(this));
    connect(testDlg.data(),&TestDialog::signalRecognitionPicture,this,&MainWindow::slotLoadPicture);
    connect(testDlg.data(),&TestDialog::signalTestCap,this,&MainWindow::slotTestCap);
    testDlg->exec();
}

void MainWindow::slotTestCap(int channel)
{
    switch (channel) {
    case 1:
        ui->lineEdit->clear();
        ui->lineEdit_2->clear();
        ui->lineEdit_3->clear();
        break;
    case 2:
        ui->lineEdit_8->clear();
        ui->lineEdit_9->clear();
        ui->lineEdit_10->clear();
        break;
    case 3:
        ui->lineEdit_11->clear();
        ui->lineEdit_12->clear();
        ui->lineEdit_13->clear();
        break;
    }


    capTime=QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz");

    emit signalSimulationCapture(cameraBingIDMap.value(QString("%1%2").arg(channel).arg(1),-1));
    emit signalSimulationCapture(cameraBingIDMap.value(QString("%1%2").arg(channel).arg(2),-1));
}

void MainWindow::slotLoadPicture(int type, QString img)
{
    Q_UNUSED(type)

    QPixmap pix(QDir::toNativeSeparators(img));

    if(pix.isNull()){
        return;
    }
    QPalette palette;

    palette.setBrush(QPalette::Background, QBrush(pix.scaled(ui->label->size(), Qt::IgnoreAspectRatio)));
    ui->label->setPalette(palette);

    QFile f(img);
    if (f.open(QIODevice::ReadOnly))
    {
        imgArrMap.insert(img,f.readAll());
        f.close();
    }

    labelImgMap.insert("11",img);

    signalDetectText(img,11);
    signalDetectText(img,11);
}

void MainWindow::slotlDetectResult(QString rst)
{
    qDebug()<<"rst:"<<rst;

    if(rstList.size()!=2){
        if(rst.trimmed().indexOf(" ")==-1){
            rstList.append(rst);
        }
        else {
            rstList=rst.trimmed().split(' ');
        }
    }

    if(rstList.size()==2){

        bool localCap=false;
        /*****************************
        * @brief:识别本地图片，不写入数据库
        ******************************/
        if(labelImgMap.key(rstList.at(0).split('-').at(0))==labelImgMap.key(rstList.at(1).split('-').at(0))){
            localCap=true;
        }

        QString tmp=rstList.at(0);
        if(tmp.indexOf('-')==-1){
            tmp=rstList.at(1);
        }

        int cl=0;
        if(localCap){
            cl=labelImgMap.key(tmp.split("-").at(0)).toInt();
        }
        else {
            cl=tmp.mid(tmp.indexOf(".jpg")-2,1).toInt();
        }

        QMap<QString,QString> dataMap;
        dataMap.insert("Channel",QString::number(cl));
        if(localCap){
            dataMap.insert("Timer",QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz"));
        }
        else {
            dataMap.insert("Timer",QDateTime::fromString(capTime,"yyyyMMddhhmmsszzz").toString("yyyy-MM-dd hh:mm:ss zzz"));
        }

        double source=0;
        QString result="";
        QString img1="";
        QString img2="";

        foreach(QString rr,rstList){
            QStringList tmpR=rr.split("-");
            if(tmpR.size()==2){
                if(tmpR.at(0).endsWith("1.jpg")){
                    img1=tmpR.at(0);
                    dataMap.insert("ImgFront",tmpR.at(0).mid(imgPath.size()));
                }
                else if(tmpR.at(0).endsWith("2.jpg")){
                    img2=tmpR.at(0);
                    dataMap.insert("ImgAfter",tmpR.at(0).mid(imgPath.size()));
                }
                else {
                    if(tmpR.at(0).endsWith(".jpg")){
                        img1=tmpR.at(0);
                    }
                }

                QStringList tmpRR=tmpR.at(1).mid(7).split("|");
                if(tmpRR.size()==3){

                    if(tmpR.at(0).endsWith("1.jpg")){
                        dataMap.insert("ImgFrontNumber",tmpRR.at(0));
                    }
                    else{
                        dataMap.insert("ImgAfterNumber",tmpRR.at(0));
                    }

                    if(source==0){
                        source=tmpRR.at(1).toDouble();
                        result=tmpRR.at(0);
                    }
                    else {
                        if(source<tmpRR.at(1).toDouble()){
                            result=tmpRR.at(0).at(0);
                            source=tmpRR.at(1).toDouble();
                        }
                    }
                }
            }
        }

        if(localCap){
            cl-=10;
        }

        switch (cl) {
        case 1:
            ui->lineEdit->setText(result);
            ui->lineEdit_2->setText(QString::number(source));
            if(localCap){
                ui->lineEdit_3->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz"));
            }
            else {
                ui->lineEdit_3->setText(QDateTime::fromString(capTime,"yyyyMMddhhmmsszzz").toString("yyyy-MM-dd hh:mm:ss zzz"));
            }
            break;
        case 2:
            ui->lineEdit_8->setText(result);
            ui->lineEdit_9->setText(QString::number(source));
            ui->lineEdit_10->setText(QDateTime::fromString(capTime,"yyyyMMddhhmmsszzz").toString("yyyy-MM-dd hh:mm:ss zzz"));
            break;
        case 3:
            ui->lineEdit_11->setText(result);
            ui->lineEdit_12->setText(QString::number(source));
            ui->lineEdit_13->setText(QDateTime::fromString(capTime,"yyyyMMddhhmmsszzz").toString("yyyy-MM-dd hh:mm:ss zzz"));
            break;
        }
//        ui->lineEdit->setText(result);
//        ui->lineEdit_2->setText(QString::number(source));
//        ui->lineEdit_3->setText(QDateTime::fromString(capTime,"yyyyMMddhhmmsszzz").toString("yyyy-MM-dd hh:mm:ss zzz"));

        dataMap.insert("Number",result);
        dataMap.insert("Check",result.isEmpty()?"0":"1");

        if(!localCap){
            emit signalInsertDataBase(dataMap);
        }

        /*****************************
        * @brief:抓拍数量
        ******************************/
//        ui->lineEdit_5->setText(QString::number(++capCount));
//        if(result.isEmpty()){
//            ++capCountErr;
//        }
//        ui->lineEdit_6->setText(QString("%1%").arg(100-capCountErr/capCount*100));

        int code=1;
        if(!img1.isEmpty() && !img2.isEmpty()){
            code=0;
        }
        QString msg=QString("time=%1,channel=%2,check=%3,result=%4,code=%5,imgPath=").arg(QDateTime::fromString(capTime,"yyyyMMddhhmmsszzz").toString("yyyy-MM-dd hh:mm:ss zzz"),
                                                                                           QString::number(cl),
                                                                                           result.isEmpty()?"0":"1",
                                                                                           result,
                                                                                           QString::number(code));
        if(!imgArrMap.value(img1,"").isEmpty()){
            emit signalToSendData(channel,msg.append(img1).toLocal8Bit());
//            QThread::msleep(100);
//            emit signalToSendData(channel,imgArrMap.value(img1));
        }
        else if(!imgArrMap.value(img2,"").isEmpty()){
            emit signalToSendData(channel,msg.append(img2).toLocal8Bit());
//            QThread::msleep(100);
//            emit signalToSendData(channel,imgArrMap.value(img2));
        }
        else{
            QByteArray msg=QString("time=%1,channel=%2,check=%3,result=%4,code=%5,imgPath=%6").arg(QDateTime::fromString(capTime,"yyyyMMddhhmmsszzz").toString("yyyy-MM-dd hh:mm:ss zzz"),
                                                                                               QString::number(channel),
                                                                                               "0",
                                                                                               "",
                                                                                               QString::number(1),
                                                                                               "").toLatin1();
            emit signalToSendData(channel,msg);
        }

        imgArrMap.clear();
        dataMap.clear();
        rstList.clear();
    }
}

void MainWindow::slotPictureStream(LLONG lLoginID, QByteArray arrImg)
{
    QDir dir(imgPath);
    QString suffixPath = QDir::toNativeSeparators(QString("%1/%2/%3").arg(imgPath,QDateTime::currentDateTime().toString("yyyy"),QDateTime::currentDateTime().toString("MM")));
    dir.mkpath(suffixPath);
    dir.cd(suffixPath);

    QScopedPointer<QPixmap> pix(new QPixmap());
    if(arrImg!=nullptr){
        pix->loadFromData(arrImg);
    }
    else {
        slotlDetectResult("RESULT:|0|0");
        return;
    }

    QPalette palette;

    if(cameraBingIDMap.key(lLoginID)=="11"){
        palette.setBrush(QPalette::Background, QBrush(pix.data()->scaled(ui->label->size(), Qt::IgnoreAspectRatio)));
        ui->label->setPalette(palette);
    }
    if(cameraBingIDMap.key(lLoginID)=="12"){
        palette.setBrush(QPalette::Background, QBrush(pix.data()->scaled(ui->label_7->size(), Qt::IgnoreAspectRatio)));
        ui->label_7->setPalette(palette);
    }

    if(cameraBingIDMap.key(lLoginID)=="21"){
        palette.setBrush(QPalette::Background, QBrush(pix.data()->scaled(ui->label_2->size(), Qt::IgnoreAspectRatio)));
        ui->label_2->setPalette(palette);
    }
    if(cameraBingIDMap.key(lLoginID)=="22"){
        palette.setBrush(QPalette::Background, QBrush(pix.data()->scaled(ui->label_8->size(), Qt::IgnoreAspectRatio)));
        ui->label_8->setPalette(palette);
    }

    if(cameraBingIDMap.key(lLoginID)=="31"){
        palette.setBrush(QPalette::Background, QBrush(pix.data()->scaled(ui->label_9->size(), Qt::IgnoreAspectRatio)));
        ui->label_9->setPalette(palette);
    }
    if(cameraBingIDMap.key(lLoginID)=="32"){
        palette.setBrush(QPalette::Background, QBrush(pix.data()->scaled(ui->label_10->size(), Qt::IgnoreAspectRatio)));
        ui->label_10->setPalette(palette);
    }

    QString img=QDir::toNativeSeparators(QString("%1/%2%3.jpg").arg(dir.path(),capTime,cameraBingIDMap.key(lLoginID)));
    pix->save(img);

    labelImgMap.insert(cameraBingIDMap.key(lLoginID),QDir::fromNativeSeparators(img));
    imgArrMap.insert(QDir::fromNativeSeparators(img),arrImg);

    signalDetectText(img,cameraBingIDMap.key(lLoginID).toInt());
}

void MainWindow::slotCameraStatus(LLONG lLoginID, bool status)
{
    if(cameraBingIDMap.key(lLoginID)=="11"){
        ui->checkBox->setEnabled(status);
        ui->checkBox->setChecked(status);
    }
    if(cameraBingIDMap.key(lLoginID)=="12"){
        ui->checkBox_4->setEnabled(status);
        ui->checkBox_4->setChecked(status);
    }
    if(cameraBingIDMap.key(lLoginID)=="21"){
        ui->checkBox_2->setEnabled(status);
        ui->checkBox_2->setChecked(status);
    }
    if(cameraBingIDMap.key(lLoginID)=="22"){
        ui->checkBox_5->setEnabled(status);
        ui->checkBox_5->setChecked(status);
    }
    if(cameraBingIDMap.key(lLoginID)=="31"){
        ui->checkBox_3->setEnabled(status);
        ui->checkBox_3->setChecked(status);
    }
    if(cameraBingIDMap.key(lLoginID)=="32"){
        ui->checkBox_6->setEnabled(status);
        ui->checkBox_6->setChecked(status);
    }
}

void MainWindow::slotSetCameraID(LLONG ID, QString signature)
{
    cameraBingIDMap.insert(signature,ID);
}

void MainWindow::socketReadDataSlot(int channel_number, const QString &result)
{
    qDebug().noquote()<<result;

    switch (channel_number) {
    case 1:
        ui->lineEdit->clear();
        ui->lineEdit_2->clear();
        ui->lineEdit_3->clear();
        break;
    case 2:
        ui->lineEdit_8->clear();
        ui->lineEdit_9->clear();
        ui->lineEdit_10->clear();
        break;
    case 3:
        ui->lineEdit_11->clear();
        ui->lineEdit_12->clear();
        ui->lineEdit_13->clear();
        break;
    }

    imgArrMap.clear();

    if(result.startsWith("cap=")){
        capTime=QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz");

        emit signalSimulationCapture(cameraBingIDMap.value(QString("%1%2").arg(channel_number).arg(1),-1));
        emit signalSimulationCapture(cameraBingIDMap.value(QString("%1%2").arg(channel_number).arg(2),-1));
    }
}

void MainWindow::TCP_socketLinkStateSlot(const QString &address,quint16 port,bool state)
{
    Q_UNUSED(address);
    Q_UNUSED(port);

    if(state){
        ++count;
    }
    else {
        if(count>0){
            --count;
        }
    }

    if(count>0){
        ui->actionStatus->setIconText(QString("Link:%1").arg(count));
    }
    else
    {
        ui->actionStatus->setIconText("Not Link!");
    }
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    emit signalOpenTheVideo(cameraBingIDMap.value("11"),arg1,ui->label->winId());
}

void MainWindow::on_checkBox_2_stateChanged(int arg1)
{
    emit signalOpenTheVideo(cameraBingIDMap.value("21"),arg1,ui->label_2->winId());
}

void MainWindow::on_checkBox_3_stateChanged(int arg1)
{
    emit signalOpenTheVideo(cameraBingIDMap.value("31"),arg1,ui->label_9->winId());
}

void MainWindow::on_checkBox_4_stateChanged(int arg1)
{
    emit signalOpenTheVideo(cameraBingIDMap.value("12"),arg1,ui->label_7->winId());
}

void MainWindow::on_checkBox_5_stateChanged(int arg1)
{
    emit signalOpenTheVideo(cameraBingIDMap.value("22"),arg1,ui->label_8->winId());
}

void MainWindow::on_checkBox_6_stateChanged(int arg1)
{
    emit signalOpenTheVideo(cameraBingIDMap.value("32"),arg1,ui->label_10->winId());
}
