#ifndef DATAINTERCHANGE_GLOBAL_H
#define DATAINTERCHANGE_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QDebug>

#if defined(DATAINTERCHANGE_LIBRARY)
#  define DATAINTERCHANGESHARED_EXPORT Q_DECL_EXPORT
#else
#  define DATAINTERCHANGESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // DATAINTERCHANGE_GLOBAL_H
