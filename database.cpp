#include "database.h"

DataBase::DataBase(QObject *parent)
{
    this->setParent(parent);
}

void DataBase::initDataBaseSlot(const QString &connectName,const QString &user, const QString &pass, const QString &ip,const int &dataBaseType)
{
    Q_UNUSED(user)
    Q_UNUSED(pass)
    Q_UNUSED(ip)
    Q_UNUSED(dataBaseType)

    /*  创建插件目录  */
    QDir dir(QCoreApplication::applicationDirPath());
    const QString pluginPath="Data";
    dir.mkdir(pluginPath);
    dir.cd(pluginPath);

    this->connectName=QString("DataBaseRead_").append(connectName);
    db=QSqlDatabase::addDatabase("QSQLITE",this->connectName);
    db.setDatabaseName(QDir::toNativeSeparators(QString("%1/%2").arg(dir.path()).arg("history")));
//    db.setUserName(user);
//    db.setPassword(pass);
//    db.setHostName(ip);
//    db.setPort(3366);

    if(db.open()){
        QSqlQuery query(db);
        query.prepare(QString("CREATE TABLE `ZBYNS` (\
                      `ID`  INTEGER PRIMARY KEY AUTOINCREMENT,\
                      `Timer`   TEXT,\
                      `Channel` INTEGER NOT NULL,\
                      `Type`    INTEGER,\
                      `Number`  TEXT,\
                      `Check`   INTEGER,\
                      `ImgFront`	TEXT,\
                      `ImgFrontNumber`  TEXT,\
                      `ImgAfter`    TEXT,\
                      `ImgAfterNumber`  TEXT,\
                      `Weight`  INTEGER\
                  )"));
        if(!query.exec()){
                          qWarning().noquote()<<QString("Create table containers error<errorCode=%1>").arg(query.lastError().text());
                      }
                      else {
                          qInfo().noquote()<<QString("Create table Containers sucess");
                      }

                      query.clear();
    }
    else {
        qWarning().noquote()<<QString("Open databse  error<errorCode=%1>").arg(db.lastError().text());
    }
    db.close();
}

void DataBase::statisticalData(QSqlTableModel* model)
{
    QSqlRecord record;

    double correct=0;    double error=0;
    int rows= model->rowCount();

    for(int i=0;i<rows;i++){
        record=model->record(i);
        if(record.value("Check").toBool()){
            correct++;
        }
        else {
            error++;
        }
    }
    record.clear();

    emit statisticalDataSignal(rows,correct,error,correct/rows*100);
}

void DataBase::setDataBaseFilterSlot(const QString &filter)
{
    locker.lockForRead();

    if(db.open()){
        QScopedPointer<QSqlTableModel> model(new QSqlTableModel(this,db));
        //QSqlTableModel* model=new  QSqlTableModel(this,db);/* 在数据库界面已做删除 */
        qDebug().noquote()<<"Query database:"<<filter;
        model->setTable(QString("ZBYNS"));
        model->setFilter(filter);
        model->select();
        while (model->canFetchMore()) {
            model->fetchMore();
        }
\
        /* 统计数据 */
        statisticalData(model.data());
        emit returnModelSingal(model.take());
    }
    else {
        qWarning().noquote()<<QString("Open databse  error<errorCode=%1>").arg(db.lastError().text());
    }
    db.close();
    locker.unlock();
}

void DataBase::insertDataBaseSlot(QMap<QString, QString> data)
{
    locker.lockForWrite();

    if(db.open()){
        QScopedPointer<QSqlTableModel> model(new QSqlTableModel(this,db));
        model->setTable(QString("ZBYNS"));
        QSqlRecord record=model->record();

        foreach (auto key, data.keys()) {
            record.setValue(key,data.value(key));
        }
        if(!model->insertRecord(-1,record)){
            qWarning().noquote()<<QString("%1:Insert data to databse  error<errorCode=%2>").arg(record.value("Timer").toString()).arg(model->lastError().text());
        }
        else {
            qDebug().noquote()<<QString("%1:Insert data to databse  sucessful").arg(record.value("Timer").toString());
        }
        model->submitAll();

        data.clear();
        record.clearValues();
        model->clear();
    }
    else {
        qWarning().noquote()<<QString("Open databse  error<errorCode=%1>").arg(db.lastError().text());
    }
    db.close();
    locker.unlock();
}

void DataBase::updateDataBaseSlot(QMap<QString, QString> data)
{
    locker.lockForWrite();

    if(db.open()){
        QScopedPointer<QSqlTableModel> model(new QSqlTableModel(this,db));
        model->setTable(QString("ZBYNS"));
        model->setFilter(QString("Timer='%1' AND Channel='%2'").arg(data.value("Timer")).arg(data.value("Channel")));
        model->select();
        if(model->rowCount()==1){
            QSqlRecord record=model->record(0);

            foreach (auto key, data.keys()) {
                record.setValue(key,data.value(key));
            }
            if(!model->setRecord(0,record)){
                qWarning().noquote()<<QString("Update data to databse  error<errorCode=%1>").arg(model->lastError().text());
            }
            else {
                QString timer="";
                if(!data.value("Timer","").isEmpty()){
                    timer=data.value("Timer");
                }
                else {
                    timer=data.value("PlateTimer");
                }
                qDebug().noquote()<<QString("Update data to databse  sucessful time=%1").arg(timer);
            }
            model->submitAll();
            data.clear();
            record.clear();
            model->clear();
        }
    }
    db.close();
    locker.unlock();
}
