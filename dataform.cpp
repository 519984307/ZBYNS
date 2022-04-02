#include "dataform.h"
#include "ui_dataform.h"

DataForm::DataForm(QWidget *parent, const QString &imgPath) :
    QWidget(parent),
    ui(new Ui::DataForm)
{
    ui->setupUi(this);
    ui->pushButton_3->setEnabled(false);

    this->path=imgPath;

    pDataBase=new DataBase(this);
    connect(pDataBase,&DataBase::returnModelSingal,this,&DataForm::slot_returnModel);
    connect(pDataBase,&DataBase::statisticalDataSignal,this,&DataForm::slot_statisticalData);
    connect(this,&DataForm::signalInitDataBase,pDataBase,&DataBase::initDataBaseSlot);

    emit signalInitDataBase("READ","admin","ABCabc123","127.0.0.1",1);
}

DataForm::~DataForm()
{
    delete pDataBase;
    pDataBase=nullptr;

    delete ui;
}

void DataForm::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
}

void DataForm::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    QPalette palette;
    QScopedPointer<QPixmap> labelPix(new QPixmap());

    if(!img1.isEmpty()){
        if(labelPix->load(img1)){
            palette.setBrush(QPalette::Background, QBrush(labelPix.data()->scaled(ui->label->size(), Qt::IgnoreAspectRatio)));
            ui->label->setPalette(palette);
        }
    }
    if(!img2.isEmpty()){
        if(labelPix->load(img2)){
            palette.setBrush(QPalette::Background, QBrush(labelPix.data()->scaled(ui->label_2->size(), Qt::IgnoreAspectRatio)));
            ui->label_2->setPalette(palette);
        }
    }
}

void DataForm::on_pushButton_clicked()
{
    QPointer<FilterDialog> filter(new FilterDialog(this));
    connect(filter,&FilterDialog::signal_filterData,pDataBase,&DataBase::setDataBaseFilterSlot);
    filter->exec();
}

void DataForm::slot_returnModel(QSqlTableModel *ml)
{
    ui->tableView->setModel(ml);
    ui->tableView->setColumnHidden(ID,true);
    ui->tableView->setColumnHidden(ImgFront,true);
    ui->tableView->setColumnHidden(ImgFrontNumber,true);
    ui->tableView->setColumnHidden(ImgAfter,true);
    ui->tableView->setColumnHidden(ImgAfterNumber,true);
}

void DataForm::slot_statisticalData(int total, double correct, double error, double statistical)
{
    QString statis=QString("total[%1] correct[%2] error[%3] statistical[%4%]").arg(QString::number(total),QString::number(correct),QString::number(error),QString::number(statistical));
    ui->label_3->setText(statis);
}

void DataForm::on_tableView_clicked(const QModelIndex &index)
{
    img1.clear();
    img2.clear();

    ui->pushButton_3->setEnabled(true);
    this->index=index;

    QPalette palette;
    ui->label->setPalette(palette);
    ui->label_2->setPalette(palette);

    ui->lineEdit->setText(index.sibling(index.row(),Number).data().toString());
    ui->lineEdit_2->setText(QString::number(index.sibling(index.row(),Weight).data().toInt()));

    ui->lineEdit_3->setText(index.sibling(index.row(),ImgFrontNumber).data().toString());
    ui->lineEdit_4->setText(index.sibling(index.row(),ImgAfterNumber).data().toString());

    if(index.sibling(index.row(),Checkt).data().toInt()==1){
        ui->lineEdit->setStyleSheet("background-color: rgb(0, 170, 0);color: rgb(255, 255, 255);");
    }
    else {
        ui->lineEdit->setStyleSheet("background-color: rgb(255, 0, 0);color: rgb(255, 255, 255);");
    }

    QScopedPointer<QPixmap> labelPix(new QPixmap());

    QString imgTmp=index.sibling(index.row(),ImgFront).data().toString();
    if(imgTmp!=""){
        if(labelPix->load(QDir::toNativeSeparators(QString("%1/%2").arg(path,imgTmp)))){
            palette.setBrush(QPalette::Background, QBrush(labelPix.data()->scaled(ui->label->size(), Qt::IgnoreAspectRatio)));
            ui->label->setPalette(palette);
        }
        img1=QDir::toNativeSeparators(QString("%1/%2").arg(path,imgTmp));
    }
    imgTmp=index.sibling(index.row(),ImgAfter).data().toString();
    if(imgTmp!=""){
        if(labelPix->load(QDir::toNativeSeparators(QString("%1/%2").arg(path,imgTmp)))){
            palette.setBrush(QPalette::Background, QBrush(labelPix.data()->scaled(ui->label_2->size(), Qt::IgnoreAspectRatio)));
            ui->label_2->setPalette(palette);
        }
        img2=QDir::toNativeSeparators(QString("%1/%2").arg(path,imgTmp));
    }
}

void DataForm::on_pushButton_2_clicked()
{
    QString filepath = QFileDialog::getSaveFileName(this, QString("Save as..."),
                                                  QString(), QString("EXCEL files (*.xls);;HTML-Files (*.txt);;"));

    if (filepath != "")
    {
        int row = ui->tableView->model()->rowCount();
        int col = ui->tableView->model()->columnCount();
        QList<QString> list;
        //添加列标题
        QString HeaderRow;
        for (int i = 0; i < col; i++)
        {
            HeaderRow.append(ui->tableView->model()->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString() + "\t");
        }
        list.push_back(HeaderRow);
        for (int i = 0; i < row; i++)
        {
            QString rowStr = "";
            for (int j = 0; j < col; j++){
                QModelIndex index = ui->tableView->model()->index(i, j);
                rowStr += ui->tableView->model()->data(index).toString() + "\t";
            }
            list.push_back(rowStr);
        }
        QTextEdit textEdit;
        for (int i = 0; i < list.size(); i++)
        {
            textEdit.append(list.at(i));
        }

        QFile file(filepath);
        if (file.open(QFile::WriteOnly | QIODevice::Text))
        {
            QTextStream ts(&file);
            ts.setCodec("GB2312");/* 这个地方大家自己判断是否用“utf-8” */
            ts << textEdit.document()->toPlainText();
            file.close();
            QMessageBox::information(this,"excel data","Data export successful");
        }else{
            QMessageBox::warning(this,"excel data","Data export failed");
        }
    }
}

void DataForm::on_pushButton_3_clicked()
{
    bool picErr=false;

    if(img1.isEmpty() || img2.isEmpty()){
        picErr=true;
    }
    QByteArray rst=QString("time=%1,channel=%2,check=%3,result=%4,code=%5,imgPath=").arg(index.sibling(index.row(),Timer).data().toString(),
                                                                                      QString::number(index.sibling(index.row(),Channel).data().toInt()),
                                                                                      QString::number(index.sibling(index.row(),Checkt).data().toInt()),
                                                                                      index.sibling(index.row(),Number).data().toString(),
                                                                                      QString::number(picErr)).toLatin1();
    if(!img1.isEmpty()){
        emit signalSendResult(index.sibling(index.row(),Channel).data().toInt(),rst.append(img1));
    }
    else if (img2.isEmpty()) {
        emit signalSendResult(index.sibling(index.row(),Channel).data().toInt(),rst.append(img2));
    }
    else {
            emit signalSendResult(index.sibling(index.row(),Channel).data().toInt(),rst);
    }


//    QFile f;

//    QByteArray imgArr="";
//    //QPixmap pixmap;

//    bool picErr=false;

//    if(!img1.isEmpty()){
//        //pixmap.load(img1);
//        f.setFileName(img1);
//    }
//    else if(!img2.isEmpty()){
//        //pixmap.load(img2);
//        f.setFileName(img2);
//    }
//    else {
//        picErr=true;
//    }

//    if(!picErr){
////        QBuffer buffer(&bytes);
////        buffer.open(QIODevice::WriteOnly);
////        pixmap.save(&buffer,"JPG");

//        if (f.open(QIODevice::ReadOnly))
//        {
//            imgArr = f.readAll();
//            f.close();
//        }
//    }

//    QByteArray rst=QString("time=%1,channel=%2,check=%3,result=%4,code=%5,imgPath=%6").arg(index.sibling(index.row(),Timer).data().toString(),
//                                                                                      QString::number(index.sibling(index.row(),Channel).data().toInt()),
//                                                                                      QString::number(index.sibling(index.row(),Checkt).data().toInt()),
//                                                                                      index.sibling(index.row(),Number).data().toString(),
//                                                                                      QString::number(picErr)).toLatin1();

//    emit signalSendResult(index.sibling(index.row(),Channel).data().toInt(),rst);
//    if(imgArr.size()!=0){
//        QThread::msleep(100);
//        emit signalSendResult(index.sibling(index.row(),Channel).data().toInt(),imgArr);
//    }
}
