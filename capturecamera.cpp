#include "capturecamera.h"

#include <fstream>
#include <iostream>

using namespace std;

///
/// \brief MiddlewareCLINET::pThis 初始化类对象指针
///
CaptureCamera* CaptureCamera::pThis=nullptr;

CaptureCamera::CaptureCamera(QObject *parent) : QObject(parent)
{    
    this->setParent(parent);

    CaptureCamera::pThis=this;
    initSDk=false;
    lLoginHandle=0;
    lRealHandleCap=0;
    capType=1;
    cap=false;

    snapParams.Channel=0;
    snapParams.Quality=1;
    snapParams.ImageSize=2;
    snapParams.mode=0;
    snapParams.InterSnap=1;
    snapParams.CmdSerial=1;
    memset(snapParams.Reserved,0,sizeof (snapParams.Reserved));
}

CaptureCamera::~CaptureCamera()
{
    if(initSDk){
        if(0 != lRealHandleCap){
            CLIENT_StopRealPlay(lRealHandleCap);
        }

        foreach(LLONG lRealHandle,cmaeraBingTVMap.values()){
            if(0 != lRealHandle){
                CLIENT_StopRealPlay(lRealHandle);
            }
        }

        if(0 != lRealLoadHand){
            CLIENT_StopLoadPic(lRealLoadHand);
        }

        CLIENT_Logout(lLoginHandle);

        CLIENT_Cleanup();
    }

    qInfo().noquote()<<QString("[%1] Release resources").arg(this->metaObject()->className());
}

void CaptureCamera::disConnectFunc(LLONG  lLoginID, char *pchDVRIP, LONG nDVRPort, LDWORD  dwUser)
{
    Q_UNUSED(lLoginID)
    Q_UNUSED(nDVRPort)
    Q_UNUSED(dwUser)

    emit pThis->signalCameraStatus(lLoginID,false);

    qWarning().noquote()<<QString("[%1] %2:camera disconnect errCode=<%3>").arg(pThis->metaObject()->className(),QString::fromLatin1(pchDVRIP),QString::number(CLIENT_GetLastError()&(0x7fffffff)));
}

void CaptureCamera::haveReConnectFunc(LLONG  lLoginID, char *pchDVRIP, LONG nDVRPort, LDWORD  dwUser)
{
    Q_UNUSED(lLoginID)
    Q_UNUSED(nDVRPort)
    Q_UNUSED(dwUser)

    emit pThis->signalCameraStatus(lLoginID,true);

    qInfo().noquote()<<QString("[%1] %2:camera connected").arg(pThis->metaObject()->className(),QString::fromLatin1(pchDVRIP));
}

void CaptureCamera::subHaveReConnectFunc(EM_INTERFACE_TYPE emInterfaceType, BOOL bOnline, LLONG  lOperateHandle, LLONG  lLoginID, LDWORD  dwUser)
{
    Q_UNUSED(emInterfaceType)
    Q_UNUSED(bOnline)
    Q_UNUSED(lOperateHandle)
    Q_UNUSED(lLoginID)
    Q_UNUSED(dwUser)
}

BOOL CaptureCamera::dVRMessCallBack(LONG lCommand, LLONG  lLoginID, char *pBuf, DWORD dwBufLen, char *pchDVRIP, LONG nDVRPort, LDWORD  dwUser)
{
    Q_UNUSED(lCommand)
    Q_UNUSED(lLoginID)
    Q_UNUSED(pBuf)
    Q_UNUSED(dwUser)
    Q_UNUSED(dwBufLen)
    Q_UNUSED(pchDVRIP)
    Q_UNUSED(nDVRPort)

    return true;
}

void CaptureCamera::snapRevCallBack(LLONG lLoginID, BYTE *pBuf, UINT RevLen, UINT EncodeType, DWORD CmdSerial, LDWORD  dwUser)
{
    Q_UNUSED(EncodeType)
    Q_UNUSED(CmdSerial)
    Q_UNUSED(dwUser)

    QByteArray arrImg(reinterpret_cast<const char*>(pBuf),RevLen);
    emit pThis->signalPictureStream(lLoginID,arrImg);
}

void CaptureCamera::simulationCaptureSlot(LLONG ID)
{
    QString addr=cmaeraBingIDMap.value(ID,"");
    if(addr.isEmpty()){
        emit signalPictureStream(ID,nullptr);
        return;
    }

    bool status=true;

    switch (capType) {
    case 1:
        /*****************************
        * @brief:回调抓拍，需要设置CLIENT_SetSnapRevCallBack抓拍回调函数
        ******************************/
        if(!CLIENT_SnapPictureEx(ID,&snapParams,0)){
            emit signalPictureStream(ID,nullptr);
            status=false;
        }
        break;
    case 2:
        /*****************************
        * @brief:预览抓拍，抓拍到文件，需要打开图像的函数参数hWnd有效时该函数获取的参数才有效
        ******************************/
//        if(!pic.isEmpty()){
//            QFile file(pic);
//            file.remove();
//            pic.clear();
//        }
        pic=QDir::toNativeSeparators(QString("%1/tmp/%2.jpg").arg(QCoreApplication::applicationDirPath(),QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz"))).toLatin1();
        if(!CLIENT_CapturePictureEx(cmaeraBingTVMap.value(ID),pic.data(),NET_CAPTURE_JPEG_70)){
            emit signalPictureStream(ID,nullptr);
            pic.clear();
            status=false;
        }
        else {
            ifstream in(pic.toStdString(),ifstream::in|ios::binary);
            in.seekg(0,in.end);
            int len=in.tellg();
            in.seekg(0,in.beg);
            char* buf=new char[len];
            in.read(buf,len);
            in.close();

            QByteArray arr(buf,len);
            emit signalPictureStream(ID,arr);

            delete [] buf;
            arr.clear();

            QFile file(pic);
            file.remove();
            pic.clear();
        }
        break;
    case 3:
        /*****************************
        * @brief:功能抓图，在调用本接口之前首先需要先调用CLIENT_RealLoadPicture订阅手动抓图事件，才可以成功调用本接口
        ******************************/
        if(!CLIENT_ControlDevice(lLoginHandle,DH_MANUAL_SNAP,nullptr)){
            emit signalPictureStream(ID,nullptr);
            status=false;
        }
        break;
    case 4:
        /*****************************
        * @brief:视频解码抓拍,必须线打开预览
        ******************************/
        if(0 == lRealHandleCap){
            emit signalPictureStream(ID,nullptr);
            status=false;
        }
        else {
            cap=true;
        }
        break;
    default:
        status=false;
    }

    if(!status){
        qWarning().noquote()<<QString("[%1] %2:Camera capture failed <errCode=%3>").arg(this->metaObject()->className(),addr,QString::number(CLIENT_GetLastError()&(0x7fffffff)));
    }
    else {
        qInfo().noquote()<<QString("[%1] %2:Camera capture sucess").arg(this->metaObject()->className(),addr);
    }
}

void CaptureCamera::setCaptureTypeSlot(const int &capType, const int &msgCallBackInd)
{
    Q_UNUSED(msgCallBackInd)
    this->capType=capType;

    initSDk = CLIENT_Init(CaptureCamera::disConnectFunc,0);
    CLIENT_SetConnectTime(3000,0);
    CLIENT_SetSnapRevCallBack(CaptureCamera::snapRevCallBack,0);
    CLIENT_SetAutoReconnect(CaptureCamera::haveReConnectFunc,0);
    CLIENT_SetSubconnCallBack(CaptureCamera::subHaveReConnectFunc,0);
    CLIENT_SetDVRMessCallBack(CaptureCamera::dVRMessCallBack,0);

    if(!initSDk){
        qCritical().noquote()<<QString("[%1] The SDK initialization failed").arg(this->metaObject()->className());
        CLIENT_Cleanup();
    }
    else {
        qDebug().noquote()<<QString("[%1] The SDK is successfully initialized").arg(this->metaObject()->className());
    }

//    if(1 == capType){
//        CLIENT_SetSnapRevCallBack(CaptureCamera::snapRevCallBack,0);
//    }
//    else if (2 == capType) {
//        /*****************************
//        * @brief:预览抓图，暂不处理
//        ******************************/
//    }
//    else if (3 == capType){
//        lRealLoadHand = CLIENT_RealLoadPicture(lLoginHandle,1,EVENT_IVS_ALL,CaptureCamera::analyzerDataCallBack,0);
//        if(-1 != lRealLoadHand){
//            qInfo().noquote()<<QString("[%1] %2:Camera event subscription succeeded").arg(this->metaObject()->className(),addr);
//        }
//        else {
//            qWarning().noquote()<<QString("[%1] %2:Camera event subscription failed<errCode=%3>").arg(this->metaObject()->className(),addr,QString::number(DH::CLIENT_GetLastError()&(0x7fffffff)));
//        }
//    }
//    else if (4 == capType) {
//        lRealHandleCap = CLIENT_RealPlay(lLoginHandle,1,nullptr);
//        if(-1 != lRealHandleCap){
//            CLIENT_SetRealDataCallBack(lRealHandleCap,CaptureCamera::realDataCallBack,0);
//            qInfo().noquote()<<QString("[%1] %2:Camera event subscription succeeded").arg(this->metaObject()->className(),addr);
//        }
//        else {
//            qWarning().noquote()<<QString("[%1] %2:Camera event subscription failed<errCode=%3>").arg(this->metaObject()->className(),addr,QString::number(CLIENT_GetLastError()&(0x7fffffff)));
//        }
//    }
//    else {
//        qWarning().noquote()<<QString("[%1] %2:The camera capture mode is incorrect").arg(this->metaObject()->className(),addr);
//    }
}

void CaptureCamera::initCameraSlot(const QString &localAddr, const QString &addr, const int &port, const QString &user, const QString &pow, const QString &signature)
{
    Q_UNUSED(localAddr)

    if(initSDk){
        NET_IN_LOGIN_WITH_HIGHLEVEL_SECURITY stInparam;
        memset(&stInparam, 0, sizeof(stInparam));
        stInparam.dwSize = sizeof(stInparam);
        strncpy_s(stInparam.szIP, addr.toLatin1().data(), sizeof(stInparam.szIP) - 1);
        strncpy_s(stInparam.szUserName, user.toLatin1().data(), sizeof(stInparam.szUserName) - 1);
        strncpy_s(stInparam.szPassword, pow.toLatin1().data(), sizeof(stInparam.szPassword) - 1);
        stInparam.nPort = port;
        stInparam.emSpecCap = EM_LOGIN_SPEC_CAP_TCP;
        NET_OUT_LOGIN_WITH_HIGHLEVEL_SECURITY stOutparam;
        memset(&stOutparam, 0, sizeof(stOutparam));
        stOutparam.dwSize = sizeof(stOutparam);

        LLONG lLoginHandle = CLIENT_LoginWithHighLevelSecurity(&stInparam,&stOutparam);
        if(lLoginHandle!=0){
            /*****************************
            * @brief:绑定登录ID和特征码
            ******************************/
            emit signal_setCameraID(lLoginHandle,signature);

            cmaeraBingIDMap.insert(lLoginHandle,addr);

            qInfo().noquote()<<QString("[%1] %2:camera login sucess").arg(this->metaObject()->className(),addr);
        }
        else {
            CLIENT_Logout(lLoginHandle);
            qWarning().noquote()<<QString("[%1] %2:camera login failde<errCode=%3>").arg(this->metaObject()->className(),addr,QString::number(CLIENT_GetLastError()&(0x7fffffff)));
        }
    }
}

void CaptureCamera::openTheVideoSlot(LLONG ID, bool play, qint64 winID)
{
#ifdef Q_OS_LINUX
    HWND  hPlayWnd=static_cast<HWND>(winID);
#endif
#ifdef Q_OS_WIN
    HWND  hPlayWnd=reinterpret_cast<HWND>(winID);
#endif

    QString addr=cmaeraBingIDMap.value(ID,"");
    if(addr.isEmpty()){
        return;
    }


    LLONG lRealHandle=cmaeraBingTVMap.value(ID,0);

    if(play){
         lRealHandle=CLIENT_RealPlayEx(ID,0,hPlayWnd,DH_RType_Realplay);

         cmaeraBingTVMap.insert(ID,lRealHandle);

         if(0 != lRealHandle){
             qInfo().noquote()<<QString("[%1] %2:Camera preview opened successfully").arg(this->metaObject()->className(),addr);
         }
         else {
             qWarning().noquote()<<QString("[%1] %2:Failed to open camera preview<errCode=%3>").arg(this->metaObject()->className(),addr,QString::number(CLIENT_GetLastError()&(0x7fffffff)));
         }
    }
    else {
        if(CLIENT_StopRealPlay(lRealHandle)){
            qInfo().noquote()<<QString("[%1] %2:Camera preview closed successfully").arg(this->metaObject()->className(),addr);
        }
        else {
            qWarning().noquote()<<QString("[%1] %2:Failed to close camera preview<errCode=%3>").arg(this->metaObject()->className(),addr,QString::number(CLIENT_GetLastError()&(0x7fffffff)));
        }
    }
}
