#ifndef CAPTURECAMERA_H
#define CAPTURECAMERA_H

#include <QObject>

class CaptureCamera : public QObject
{
    Q_OBJECT
public:
    explicit CaptureCamera(QObject *parent = nullptr);

signals:

};

#endif // CAPTURECAMERA_H
