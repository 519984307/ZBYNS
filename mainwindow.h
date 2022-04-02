#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSharedPointer>
#include <QCloseEvent>
#include <QPixmap>
#include <QPalette>
#include <QBrush>
#include <QPaintEvent>
#include <QEvent>
#include <QDebug>
#include <QTimer>
#include <QPointer>
#include <QPluginLoader>

#include "settingdialog.h"
#include "logform.h"
#include "dataform.h"
#include "lockdialog.h"
#include "testdialog.h"
#include "ocr_num.h"

#include "capturecamera.h"
#include "LogController/logcontroller.h"
#include "database.h"
#include "DataInterchange/datainterchangeinterface.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();   

    void closeEvent(QCloseEvent *event)Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event)Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private slots:

    ///
    /// \brief on_actionSetting_triggered 设置参数
    ///
    void on_actionSetting_triggered();

    ///
    /// \brief on_actionLog_triggered 查看日志
    ///
    void on_actionLog_triggered();

    ///
    /// \brief on_actionDataBase_triggered 查询数据库
    ///
    void on_actionDataBase_triggered();

    ///
    /// \brief loadParment 加载参数
    ///
    void loadParment();

    ///
    /// \brief on_actionLock_triggered 锁定程序
    ///
    void on_actionLock_triggered();

    ///
    /// \brief on_actionVideo_1_triggered 打开1号相机显示屏
    /// \param checked
    ///
    void on_actionVideo_1_triggered(bool checked);

    ///
    /// \brief on_actionVideo_2_triggered 打开2号相机显示屏
    /// \param checked
    ///
    void on_actionVideo_2_triggered(bool checked);

    ///
    /// \brief on_actionTest_triggered 启动抓拍测试
    /// \param checked
    ///
    void on_actionTest_triggered(bool checked);

private:

    Ui::MainWindow *ui;

    ///
    /// \brief pDataBase 数据库处理类
    ///
    DataBase* pDataBase;

    ///
    /// \brief logFrom 日志页面
    ///
    LogForm logFrom;

    ///
    /// \brief dataForm 数据库页面
    ///
    QSharedPointer<DataForm> dataForm;

    ///
    /// \brief pOCR_NUM 识别器对象
    ///
    OCR_NUM* pOCR_NUM;

    ///
    /// \brief pLog 日志
    ///
    QPointer<LogController> pLog;

    ///
    /// \brief pCapCamera 相机类
    ///
    CaptureCamera* pCapCamera;

    ///
    /// \brief cameraBingIDMap 相机编号和ID
    ///
    QMap<QString,LLONG> cameraBingIDMap;

    /*****************************
    * @brief:系统参数
    ******************************/
    QVector<QString> camera1,camera2;
    QString imgPath,address;
    QString lock;
    QString testImg1,testImg2;
    int quit,model,port,heart,count,channel;
    int capCount,capCountErr;

    ///
    /// \brief imgArrMap 图片流集
    ///
    QMap<QString,QByteArray> imgArrMap;

    ///
    /// \brief rstList 识别结果列表
    ///
    QStringList rstList;

    ///
    /// \brief capTime 抓拍时间戳
    ///
    QString capTime;

    ///
    /// \brief capNum 模拟抓拍次数
    ///
    int capNum;

    ///
    /// \brief capTimer 循环抓拍
    ///
    QTimer* capTimer;

private slots:

    ///
    /// \brief slotLoadPicture 加载本地图片
    /// \param imgPath
    ///
    void slotLoadPicture(int type,QString img);

    ///
    /// \brief slotlDetectResult 识别结果
    /// \param rst
    ///
    void slotlDetectResult(QString rst);

    ///
    /// \brief slotCapTimer 循环模拟抓拍
    ///
    void slotCapTimer();

    ///
    /// \brief slotPictureStream 抓拍图片流
    /// \param lLoginID
    /// \param arrImg
    ///
    void slotPictureStream(LLONG lLoginID,QByteArray arrImg);

    ///
    /// \brief slotCameraStatus 相机状态
    /// \param lLoginID
    /// \param status
    ///
    void slotCameraStatus(LLONG lLoginID,bool status);

    ///
    /// \brief slotSetCameraID 设置通道登录相机ID
    /// \param ID 登录ID
    /// \param cameraIP 相机地址
    ///
    void slotSetCameraID(LLONG ID, QString signature);

    ///
    /// \brief socketReadDataSlot 接受数据
    /// \param data 数据体
    ///
    void socketReadDataSlot(int channel_number,const QString& result);

    ///
    /// \brief TCP_socketLinkStateSlot TCP链接状态
    /// \param address
    /// \param port
    /// \param state
    ///
    void TCP_socketLinkStateSlot(const QString &address,quint16 port,bool state);

signals:

    ///
    /// \brief signalDetectText 识别图片
    /// \param image
    /// \param imgNum
    ///
    void signalDetectText(const QString& image ,const int& imgNum);

    ///
    /// \brief signalOpenTheVideo 打开视频预览
    /// \param ID
    /// \param play
    /// \param winID
    ///
    void signalOpenTheVideo(LLONG ID, bool play, qint64 winID);

    ///
    /// \brief signalInitCamera 初始化相机
    /// \param localAddr
    /// \param addr
    /// \param port
    /// \param user
    /// \param pow
    /// \param signature
    ///
    void signalInitCamera(const QString &localAddr, const QString &addr, const int &port, const QString &user, const QString &pow, const QString &signature);

    ///
    /// \brief signalSetCaptureType 设置抓拍模式
    /// \param capType
    /// \param msgCallBackInd
    ///
    void signalSetCaptureType(const int &capType, const int &msgCallBackInd);

    ///
    /// \brief signalSimulationCapture 抓拍图片
    /// \param ID
    ///
    void signalSimulationCapture(LLONG ID);

    ///
    /// \brief signalInitDataBase 初始化数据库.如果不存在,就创建.
    /// \param connectName 链接名称
    /// \param user 用户名
    /// \param pass密码
    /// \param ip 地址
    /// \param dataBaseType 数据库类型
    ///
    void signalInitDataBase(const QString &connectName,const QString &user,const QString &pass,const QString &ip,const int &dataBaseType);

    ///
    /// \brief signalInsertDataBase 插入数据库
    /// \param data
    ///
    void signalInsertDataBase(QMap<QString, QString> data);

    ///
    /// \brief TCP_InitializationParameterSignal 初始化通讯接口
    /// \param address
    /// \param port
    /// \param serviceType
    /// \param heartBeat
    /// \param serviceMode
    /// \param shortLink
    /// \param newline
    ///
    void TCP_InitializationParameterSignal(const QString& address,const quint16& port,const int& serviceType,const bool& heartBeat, const int& serviceMode,const int& shortLink,const int& newline);

    ///
    /// \brief signalToSendData 发送数据
    /// \param data 数据体
    ///
    void signalToSendData(int channel_number, const QByteArray &data);

    ///
    /// \brief releaseResourcesSignal 释放资源
    ///
    void releaseResourcesSignal();
};
#endif // MAINWINDOW_H
