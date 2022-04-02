#ifndef DATAFORM_H
#define DATAFORM_H

#include <QWidget>
#include <QPointer>
#include <QSqlTableModel>
#include <QScopedPointer>
#include <QPixmap>
#include <QPalette>
#include <QBrush>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QTextEdit>
#include <QMessageBox>
#include <QFileDialog>
#include <QBuffer>
#include <QFile>
#include <QThread>

#include "filterdialog.h"
#include "database.h"

namespace Ui {
class DataForm;
}

class DataForm : public QWidget
{
    Q_OBJECT

public:
    explicit DataForm(QWidget *parent = nullptr,const QString &imgPath="C:/images");
    ~DataForm();

    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private slots:
    void on_pushButton_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::DataForm *ui;

    ///
    /// \brief pDataBase
    ///
    DataBase* pDataBase;

    QModelIndex index;

    ///
    /// \brief path 图片路径
    ///
    QString path;

    QString img1,img2;

    enum{
        ID=0,
        Timer=1,
        Channel=2,
        Type=3,
        Number=4,
        Checkt=5,
        ImgFront=6,
        ImgFrontNumber=7,
        ImgAfter=8,
        ImgAfterNumber=9,
        Weight=10
    };

signals:

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
    /// \brief signalSendResult 发送识别结果
    /// \param rst
    ///
    void signalSendResult(int channel_number, const QByteArray &data);

public slots:

    /*****************************
    * @brief:数据库读取
    ******************************/

    ///
    /// \brief slot_returnModel 返回数据模型
    /// \param model 数据模型
    ///
    void slot_returnModel(QSqlTableModel *ml);

    ///
    /// \brief slot_statisticalData
    /// \param total 总计
    /// \param correct 正确
    /// \param error 错误
    /// \param statistical 统计
    ///
    void slot_statisticalData(int total,double correct,double error,double statistical);
};

#endif // DATAFORM_H
