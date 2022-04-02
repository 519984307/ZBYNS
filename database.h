#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QDir>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlRecord>
#include <QCoreApplication>
#include <QReadWriteLock>
#include <QDebug>

class DataBase:public QObject
{
    Q_OBJECT

public:
    DataBase(QObject* parent=nullptr);

private:

    ///
    /// \brief connectName 数据库链接名称
    ///
    QString connectName;

    ///
    /// \brief db 数据库对象
    ///
    QSqlDatabase db;

    ///
    /// \brief locker
    ///
    QReadWriteLock locker;

    ///
    /// \brief statisticalData 统计识别率
    /// \param model
    ///
    void statisticalData(QSqlTableModel *model);

public slots:

    ///
    /// \brief initDataBaseSlot 初始化数据库.如果不存在,就创建.
    /// \param connectName 链接名称
    /// \param user 用户名
    /// \param pass密码
    /// \param ip 地址
    /// \param dataBaseType 数据库类型
    ///
    void initDataBaseSlot(const QString &connectName,const QString &user,const QString &pass,const QString &ip,const int &dataBaseType);

    ///
    /// \brief setDataBaseFilterSlot 设置数据库筛选条件
    /// \param filter 筛选条件
    ///
    void setDataBaseFilterSlot(const QString &filter);

    ///
    /// \brief insertDataBaseSlot 插入数据
    /// \param data
    ///
    void insertDataBaseSlot(QMap<QString, QString> data);

    ///
    /// \brief updateDataBaseSlot 更新数据
    /// \param data
    ///
    void updateDataBaseSlot(QMap<QString, QString> data);

signals:

    ///
    /// \brief returnModelSingal 返回数据模型
    /// \param model 数据模型
    ///
    void returnModelSingal( QSqlTableModel *model);

    ///
    /// \brief statisticalDataSignal
    /// \param total 总计
    /// \param correct 正确
    /// \param error 错误
    /// \param statistical 统计
    ///
    void statisticalDataSignal(int total,double correct,double error,double statistical);
};

#endif // DATABASE_H
