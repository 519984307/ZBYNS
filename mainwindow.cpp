#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    loadParment();

    pDataBase=new DataBase(this);
    connect(this,&MainWindow::signalInitDataBase,pDataBase,&DataBase::initDataBaseSlot);
    connect(this,&MainWindow::signalInsertDataBase,pDataBase,&DataBase::insertDataBaseSlot);

    pLog=QPointer<LogController>(new LogController("ZBY_NS",this));
    connect(pLog.data(),SIGNAL(signal_newLogText(QtMsgType,QDateTime,QString)),&logFrom,SLOT(slot_newLogText(QtMsgType,QDateTime,QString)));

    capNum=-1;
    capTimer=new QTimer(this);
    connect(capTimer,&QTimer::timeout,this,&MainWindow::slotCapTimer);

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
    emit signalInitCamera("",camera1.at(0),37777,camera1.at(1),camera1.at(2),"1");
    emit signalInitCamera("",camera2.at(0),37777,camera2.at(1),camera2.at(2),"2");
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

    ui->lineEdit_4->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
}

MainWindow::~MainWindow()
{
//    delete pOCR_NUM;
//    pOCR_NUM=nullptr;

    capTimer->stop();
    delete capTimer;
    capTimer=nullptr;

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

    slotLoadPicture(1,testImg1);
    slotLoadPicture(2,testImg2);
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
    set.setIniCodec("UTF-8");

    set.beginGroup("Camera1");
    camera1.append(set.value("address","192.168.1.101").toString());
    camera1.append(set.value("user","admin").toString());
    camera1.append(set.value("password","ABCabc123").toString());
    camera1.append(QString::number(set.value("camType","0").toInt()));
    set.endGroup();

    set.beginGroup("Camera2");
    camera2.append(set.value("address","192.168.1.101").toString());
    camera2.append(set.value("user",3777).toString());
    camera2.append(set.value("password","ABCabc123").toString());
    camera2.append(QString::number(set.value("camType","0").toInt()));
    set.endGroup();

    set.beginGroup("Main");
    imgPath=set.value("imgPath","C:\\Images").toString();
    lock=set.value("lock","870888").toString();
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

void MainWindow::on_actionVideo_1_triggered(bool checked)
{
    emit signalOpenTheVideo(cameraBingIDMap.value("1"),checked,ui->label->winId());
}

void MainWindow::on_actionVideo_2_triggered(bool checked)
{
    emit signalOpenTheVideo(cameraBingIDMap.value("2"),checked,ui->label_2->winId());
}

void MainWindow::on_actionTest_triggered(bool checked)
{
    testImg1.clear();
    testImg2.clear();

    imgArrMap.clear();

    if(checked){
        capTime=QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz");

        QSharedPointer<TestDialog> testDlg=QSharedPointer<TestDialog>(new TestDialog(this));
        connect(testDlg.data(),&TestDialog::signalRecognitionPicture,this,&MainWindow::slotLoadPicture);
        capNum= testDlg->exec();

        if(capNum==-1){
            qDebug().noquote()<<QString("Cancel capture identification");
            ui->actionTest->setChecked(false);
        }
        else if (capNum==99) {
            emit signalDetectText(testImg1,1);
            emit signalDetectText("testImg2",2);

            ui->actionTest->setChecked(false);
        }
        else if (capNum==1) {
            emit signalSimulationCapture(cameraBingIDMap.value("1"));
            emit signalSimulationCapture(cameraBingIDMap.value("2"));
            ui->actionTest->setChecked(false);
            //ui->actionTest->setCheckable(Qt::Unchecked);
        }
        else
        {
            /*****************************
            * @brief:循环抓拍
            ******************************/
            capTimer->setSingleShot(false);
            capTimer->start(10000);
        }
    }
    else {
        capTimer->stop();
        capTimer->setSingleShot(true);
        capNum=-1;
    }
}

void MainWindow::slotLoadPicture(int type, QString img)
{
    QPixmap pix(QDir::toNativeSeparators(img));

    if(0==type){
        testImg1=img;
    }

    if(pix.isNull()){
        return;
    }
    QPalette palette;

    if(1==type || 0 == type){
        palette.setBrush(QPalette::Background, QBrush(pix.scaled(ui->label->size(), Qt::IgnoreAspectRatio)));
        ui->label->setPalette(palette);

        QFile f(img);
        if (f.open(QIODevice::ReadOnly))
        {
            imgArrMap.insert(img,f.readAll());
            f.close();
        }

    }
    else {
        QPalette palette;
        ui->label->setPalette(palette);
    }


    if(2==type){
        palette.setBrush(QPalette::Background, QBrush(pix.scaled(ui->label_2->size(), Qt::IgnoreAspectRatio)));
        ui->label_2->setPalette(palette);

        QFile f(img);
        if (f.open(QIODevice::ReadOnly))
        {
            imgArrMap.insert(img,f.readAll());
            f.close();
        }
    }
    else {
        QPalette palette;
        ui->label_2->setPalette(palette);
    }
}

void MainWindow::slotlDetectResult(QString rst)
{
    //qDebug()<<"rst:"<<rst;
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();

    if(rstList.size()!=2){
        if(rst.trimmed().indexOf(" ")==-1){
            rstList.append(rst);
        }
        else {
            rstList=rst.trimmed().split(' ');
        }
    }

    if(rstList.size()==2){
        QMap<QString,QString> dataMap;
        dataMap.insert("Channel",QString::number(channel));
        dataMap.insert("Timer",QDateTime::fromString(capTime,"yyyyMMddhhmmsszzz").toString("yyyy-MM-dd hh:mm:ss zzz"));

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
        ui->lineEdit->setText(result);
        ui->lineEdit_2->setText(QString::number(source));
        ui->lineEdit_3->setText(QDateTime::fromString(capTime,"yyyyMMddhhmmsszzz").toString("yyyy-MM-dd hh:mm:ss zzz"));

        dataMap.insert("Number",result);
        dataMap.insert("Check",result.isEmpty()?"0":"1");

        emit signalInsertDataBase(dataMap);

        /*****************************
        * @brief:抓拍数量
        ******************************/
        ui->lineEdit_5->setText(QString::number(++capCount));
        if(result.isEmpty()){
            ++capCountErr;
        }
        ui->lineEdit_6->setText(QString("%1%").arg(100-capCountErr/capCount*100));

        int code=1;
        if(!img1.isEmpty() && !img2.isEmpty()){
            code=0;
        }
        QString msg=QString("time=%1,channel=%2,check=%3,result=%4,code=%5,imgPath=").arg(QDateTime::fromString(capTime,"yyyyMMddhhmmsszzz").toString("yyyy-MM-dd hh:mm:ss zzz"),
                                                                                           QString::number(channel),
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

void MainWindow::slotCapTimer()
{
    if(capNum==0){
        ui->actionTest->setChecked(false);
        capTimer->stop();
    }
    else {
        capTime=QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz");

        emit signalSimulationCapture(cameraBingIDMap.value("1"));
        emit signalSimulationCapture(cameraBingIDMap.value("2"));
    }
    capNum--;
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

    if(cameraBingIDMap.key(lLoginID)=="1"){
        palette.setBrush(QPalette::Background, QBrush(pix.data()->scaled(ui->label->size(), Qt::IgnoreAspectRatio)));
        ui->label->setPalette(palette);
    }
    if(cameraBingIDMap.key(lLoginID)=="2"){
        palette.setBrush(QPalette::Background, QBrush(pix.data()->scaled(ui->label_2->size(), Qt::IgnoreAspectRatio)));
        ui->label_2->setPalette(palette);
    }

    QString img=QDir::toNativeSeparators(QString("%1/%2%3.jpg").arg(dir.path(),capTime,cameraBingIDMap.key(lLoginID)));
    pix->save(img);

    if(cameraBingIDMap.key(lLoginID)=="1"){
        testImg1=img;
    }
    if(cameraBingIDMap.key(lLoginID)=="2"){
        testImg2=img;
    }

    imgArrMap.insert(QDir::fromNativeSeparators(img),arrImg);

    signalDetectText(img,cameraBingIDMap.key(lLoginID).toInt());
}

void MainWindow::slotCameraStatus(LLONG lLoginID, bool status)
{
    if(cameraBingIDMap.key(lLoginID)=="1"){
        ui->actionVideo_1->setEnabled(status);
        ui->actionVideo_1->setChecked(status);
    }
    if(cameraBingIDMap.key(lLoginID)=="2"){
        ui->actionVideo_2->setEnabled(status);
        ui->actionVideo_2->setChecked(status);
    }
}

void MainWindow::slotSetCameraID(LLONG ID, QString signature)
{
    //qInfo().noquote()<<QString("Bind camera ID");
    cameraBingIDMap.insert(signature,ID);
}

void MainWindow::socketReadDataSlot(int channel_number, const QString &result)
{
    Q_UNUSED(channel_number)
    Q_UNUSED(result)

    testImg2.clear();
    testImg2.clear();
    imgArrMap.clear();

    QStringList tmpR = result.split("=");
    if(tmpR.size()==2 && tmpR.at(1).toInt()==channel){
        capTime=QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz");

        emit signalSimulationCapture(cameraBingIDMap.value("1",0));
        emit signalSimulationCapture(cameraBingIDMap.value("2",0));
    }
}

void MainWindow::TCP_socketLinkStateSlot(const QString &address,quint16 port,bool state)
{
    Q_UNUSED(address);
    Q_UNUSED(port);

    if(state){
        ui->lineEdit_7->setText(QString::number(++count));
    }
    else {
        if(count>0){
            ui->lineEdit_7->setText(QString::number(--count));
        }
    }

    if(count>0){
        ui->label_6->setStyleSheet("background-color: rgb(0, 170, 0);color: rgb(255, 255, 255);");
    }
    else
    {
        ui->label_6->setStyleSheet("background-color: rgb(170, 0, 0);color: rgb(255, 255, 255);");
    }
}
