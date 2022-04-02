#include "testdialog.h"
#include "ui_testdialog.h"

TestDialog::TestDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestDialog)
{
    ui->setupUi(this);

    isChoose=false;
}

TestDialog::~TestDialog()
{
    delete ui;
}

void TestDialog::on_buttonBox_accepted()
{
    if(isChoose){
        this->done(99);
    }
    else {
        this->done(ui->lineEdit->text().toInt());
    }
    this->accepted();
    isChoose=false;
}

void TestDialog::on_buttonBox_rejected()
{
    this->done(-1);
    this->rejected();
    isChoose=false;
}

void TestDialog::on_pushButton_clicked()
{
    QString imgPath=QFileDialog::getOpenFileName(this,QString::fromLocal8Bit("Choose Image"),"","Images (*.*)");
    if(!imgPath.isEmpty()){
        isChoose=true;
        emit signalRecognitionPicture(0,imgPath);
    }
}
