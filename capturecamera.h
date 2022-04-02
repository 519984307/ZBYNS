#ifndef CAPTURECAMERA_H
#define CAPTURECAMERA_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QCoreApplication>

#include "CLINET/include/dhnetsdk.h"

//#pragma comment (lib,"dhnetsdk.lib")

class CaptureCamera : public QObject
{
    Q_OBJECT

public:
     CaptureCamera(QObject *parent = nullptr);
    ~CaptureCamera();

    static CaptureCamera* pThis;

    ///
    /// \brief DisConnectFunc 链接断开回调函数
    /// \param lLoginID
    /// \param pchDVRIP
    /// \param nDVRPort
    /// \param dwUser
    ///
    static void __stdcall disConnectFunc(LLONG  lLoginID, char *pchDVRIP, LONG nDVRPort, LDWORD dwUser);

    ///
    /// \brief HaveReConnectFunc 自动重连回调函数
    /// \param lLoginID
    /// \param pchDVRIP
    /// \param nDVRPort
    /// \param dwUser
    ///
    static void __stdcall haveReConnectFunc(LLONG  lLoginID, char *pchDVRIP, LONG nDVRPort, LDWORD dwUser);

    ///
    /// \brief SubHaveReConnectFunc 子链接重连回调函数
    /// \param emInterfaceType
    /// \param bOnline
    /// \param lOperateHandle
    /// \param lLoginID
    /// \param dwUser
    ///
    static void __stdcall subHaveReConnectFunc(EM_INTERFACE_TYPE emInterfaceType, BOOL bOnline, LLONG  lOperateHandle, LLONG  lLoginID, LDWORD dwUser);

    ///
    /// \brief DVRMessCallBack 报警消息回调函数
    /// \param lCommand
    /// \param lLoginID
    /// \param pBuf
    /// \param dwBufLen
    /// \param pchDVRIP
    /// \param nDVRPort
    /// \param dwUser
    /// \return
    ///
    static BOOL __stdcall dVRMessCallBack(LONG lCommand, LLONG  lLoginID, char *pBuf, DWORD dwBufLen, char *pchDVRIP, LONG nDVRPort, LDWORD dwUser);

    ///
    /// \brief SnapRevCallBack 抓图回调函数
    /// \param lLoginID
    /// \param pBuf
    /// \param RevLen
    /// \param EncodeType
    /// \param CmdSerial
    /// \param dwUser
    ///
    static void __stdcall snapRevCallBack(LLONG  lLoginID,  BYTE   *pBuf,UINT   RevLen,  UINT   EncodeType,  DWORD  CmdSerial,  LDWORD dwUser);

private:

    ///
    /// \brief initSDk 动态库加载状态
    ///
    bool initSDk;

    LLONG lLoginHandle,lRealLoadHand,lRealHandleCap;

    int capType;


    ///
    /// \brief snapParams 抓拍结构体
    ///
    SNAP_PARAMS snapParams;

    ///
    /// \brief cap 抓图动作
    ///
    bool cap;

    ///
    /// \brief pic 抓拍图片保存本地文件名
    ///
    QByteArray pic;

    ///
    /// \brief cmaeraBingIDMap 相机绑定ID
    ///
    QMap<LLONG,QString> cmaeraBingIDMap;

    ///
    /// \brief cmaeraBingTVMap 相机绑定ID
    ///
    QMap<LLONG,LLONG> cmaeraBingTVMap;

public slots:

    ///
    /// \brief openTheVideoSlot 打开视频预览
    /// \param ID
    /// \param play
    /// \param winID
    ///
    void openTheVideoSlot(LLONG ID, bool play, qint64 winID);

    ///
    /// \brief initCameraSlot 初始化相机
    /// \param localAddr
    /// \param addr
    /// \param port
    /// \param user
    /// \param pow
    /// \param signature
    ///
    void initCameraSlot(const QString &localAddr, const QString &addr, const int &port, const QString &user, const QString &pow, const QString &signature);

    ///
    /// \brief setCaptureTypeSlot 设置抓拍模式
    /// \param capType
    /// \param msgCallBackInd
    ///
    void setCaptureTypeSlot(const int &capType, const int &msgCallBackInd);

    ///
    /// \brief simulationCaptureSlot 抓拍图片
    /// \param ID
    ///
    void simulationCaptureSlot(LLONG ID);

signals:

    ///
    /// \brief signalPictureStream 抓拍图片流
    /// \param lLoginID
    /// \param arrImg
    ///
    void signalPictureStream(LLONG lLoginID,QByteArray arrImg);

    ///
    /// \brief signalCameraStatus 相机状态
    /// \param lLoginID
    /// \param status
    ///
    void signalCameraStatus(LLONG lLoginID,bool status);

    ///
    /// \brief signal_setCameraID 设置通道登录相机ID
    /// \param ID 登录ID
    /// \param cameraIP 相机地址
    ///
    void signal_setCameraID(LLONG ID,QString signature);
};

#endif // CAPTURECAMERA_H
