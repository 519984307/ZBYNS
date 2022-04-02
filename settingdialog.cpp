#include "settingdialog.h"
#include "ui_settingdialog.h"

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);

    QString path = QStandardPaths::writableLocation( QStandardPaths::AppConfigLocation);
    QSettings set(QDir::toNativeSeparators(QString("%1/Parment.ini").arg(path)),QSettings::IniFormat);
    set.setIniCodec("UTF-8");

    set.beginGroup("Camera1");
    ui->lineEdit_2->setText(set.value("address","192.168.1.101").toString());
    ui->lineEdit_3->setText(set.value("user","admin").toString());
    ui->lineEdit_4->setText(set.value("password","ABCabc123").toString());
    ui->comboBox_2->setCurrentIndex(set.value("camType",0).toInt());
    set.endGroup();

    set.beginGroup("Camera2");
    ui->lineEdit_5->setText(set.value("address","192.168.1.101").toString());
    ui->lineEdit_6->setText(set.value("user",3777).toString());
    ui->lineEdit_7->setText(set.value("password","ABCabc123").toString());
    ui->comboBox_3->setCurrentIndex( set.value("camType",0).toInt());
    set.endGroup();

    set.beginGroup("Main");
    ui->lineEdit->setText(set.value("imgPath","C:\\Images").toString());
    ui->lineEdit_8->setText(set.value("lock","870888").toString());
    ui->checkBox->setCheckState(set.value("quit",1).toInt()?Qt::Checked:Qt::Unchecked);
    ui->spinBox->setValue(set.value("channel",1).toInt());
    set.endGroup();

    set.beginGroup("Server");
    ui->radioButton->setChecked(set.value("model",1).toInt());
    ui->lineEdit_9->setText(set.value("address","127.0.0.1").toString());
    ui->lineEdit_10->setText(set.value("port",55505).toString());
    ui->checkBox_2->setCheckState(set.value("heart",0).toInt()?Qt::Checked:Qt::Unchecked);
    set.endGroup();

    if(ui->radioButton->isCheckable()){
        ui->lineEdit_9->setVisible(false);
        ui->label_12->setVisible(false);
    }
}

SettingDialog::~SettingDialog()
{
    delete ui;
}

void SettingDialog::on_pushButton_clicked()
{
    QString imgPath=QFileDialog::getExistingDirectory(this);
    ui->lineEdit->setText(QString("%1").arg(imgPath).toLocal8Bit());
}

void SettingDialog::on_buttonBox_accepted()
{
    QString path = QStandardPaths::writableLocation( QStandardPaths::AppConfigLocation);
    QSettings set(QDir::toNativeSeparators(QString("%1/Parment.ini").arg(path)),QSettings::IniFormat);
    set.setIniCodec("UTF-8");

    set.beginGroup("Main");
    set.setValue("imgPath",ui->lineEdit->text());
    set.setValue("lock",ui->lineEdit_8->text());
    set.setValue("quit",ui->checkBox->isChecked()?1:0);
    set.setValue("channel",ui->spinBox->value());
    set.endGroup();

    set.endGroup();
    set.beginGroup("Server");
    set.setValue("model",ui->radioButton->isChecked()?1:0);
    set.setValue("address",ui->lineEdit_9->text());
    set.setValue("port",ui->lineEdit_10->text().toInt());
    set.setValue("heart",ui->checkBox_2->isChecked()?1:0);
    set.endGroup();

    set.beginGroup("Camera1");
    set.setValue("address",ui->lineEdit_2->text());
    set.setValue("user",ui->lineEdit_3->text());
    set.setValue("password",ui->lineEdit_4->text());
    set.setValue("camType",ui->comboBox_2->currentIndex());
    set.endGroup();

    set.beginGroup("Camera2");
    set.setValue("address",ui->lineEdit_5->text());
    set.setValue("user",ui->lineEdit_6->text());
    set.setValue("password",ui->lineEdit_7->text());
    set.setValue("camType",ui->comboBox_3->currentIndex());
    set.endGroup();

    QMessageBox::about(this,"Msg","Parameter saved successfully");
}

void SettingDialog::on_toolButton_pressed()
{
    ui->lineEdit_8->setEchoMode(QLineEdit::Normal);
}

void SettingDialog::on_toolButton_released()
{
    ui->lineEdit_8->setEchoMode(QLineEdit::PasswordEchoOnEdit);
}

void SettingDialog::on_radioButton_toggled(bool checked)
{
    if(!checked){
        ui->lineEdit_9->setVisible(true);
        ui->label_12->setVisible(true);
    }
    else {
        ui->lineEdit_9->setVisible(false);
        ui->label_12->setVisible(false);
    }
}
