#include "testdialog.h"
#include "ui_testdialog.h"

TestDialog::TestDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestDialog)
{
    ui->setupUi(this);

    capTimer=new QTimer(this);
    capTimer->setSingleShot(false);
    connect(capTimer,&QTimer::timeout,this,&TestDialog::slotCapTimer);
}

TestDialog::~TestDialog()
{
    capTimer->stop();
    delete capTimer;
    capTimer=nullptr;

    delete ui;
}

void TestDialog::slotCapTimer()
{
    emit signalTestCap(ui->comboBox->currentIndex()+1);
}

void TestDialog::on_pushButton_clicked()
{
    QString imgPath=QFileDialog::getOpenFileName(this,QString::fromLocal8Bit("Choose Image"),"","Images (*.*)");
    if(!imgPath.isEmpty()){
        emit signalRecognitionPicture(0,imgPath);
    }
}

void TestDialog::on_checkBox_clicked(bool checked)
{
    if(checked){
        capTimer->start(10000);

        ui->pushButton_2->setEnabled(false);
        ui->radioButton_2->setEnabled(false);
    }
    else {
        capTimer->stop();

        ui->pushButton_2->setEnabled(true);
        ui->radioButton_2->setEnabled(true);
    }
}

void TestDialog::on_radioButton_2_clicked(bool checked)
{
    if(checked){
        ui->pushButton_2->setEnabled(false);
        ui->checkBox->setEnabled(false);
    }
    else {
        ui->pushButton_2->setEnabled(true);
        ui->checkBox->setEnabled(true);
    }
}

void TestDialog::on_pushButton_2_clicked()
{
    emit signalTestCap(ui->comboBox->currentIndex()+1);
}
