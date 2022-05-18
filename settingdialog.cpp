#include "settingdialog.h"
#include "ui_settingdialog.h"

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);

    QString path = QStandardPaths::writableLocation( QStandardPaths::AppConfigLocation);
    QSettings set(QDir::toNativeSeparators(QString("%1/Parment.ini").arg(path)),QSettings::IniFormat);
    set.setIniCodec("GBK");

    set.beginGroup("Main");
    ui->lineEdit->setText(set.value("imgPath","C:\\Images").toString());
    ui->lineEdit_8->setText(set.value("lock","123456").toString());
    ui->checkBox->setChecked(set.value("quit",1).toInt());
    set.endGroup();

    set.beginGroup("Server");
    ui->radioButton->setChecked(set.value("model",1).toInt());
    ui->lineEdit_9->setText(set.value("address","127.0.0.1").toString());
    ui->lineEdit_10->setText(set.value("port",55505).toString());
    ui->checkBox_2->setCheckState(set.value("heart",0).toInt()?Qt::Checked:Qt::Unchecked);
    set.endGroup();

    set.beginGroup("channel1");
    ui->lineEdit_2->setText(set.value("address1","192.168.1.101").toString());
    ui->lineEdit_3->setText(set.value("user1","admin").toString());
    ui->lineEdit_4->setText(set.value("password1","ABCabc123").toString());
    ui->comboBox_2->setCurrentIndex(set.value("camType1",0).toInt());

    ui->lineEdit_5->setText(set.value("address2","192.168.1.101").toString());
    ui->lineEdit_6->setText(set.value("user2","admin").toString());
    ui->lineEdit_7->setText(set.value("password2","ABCabc123").toString());
    ui->comboBox_3->setCurrentIndex(set.value("camType2",0).toInt());
    set.endGroup();

    set.beginGroup("channel2");
    ui->lineEdit_11->setText(set.value("address1","192.168.1.101").toString());
    ui->lineEdit_12->setText(set.value("user1",3777).toString());
    ui->lineEdit_13->setText(set.value("password1","ABCabc123").toString());
    ui->comboBox_4->setCurrentIndex( set.value("camType1",0).toInt());

    ui->lineEdit_14->setText(set.value("address2","192.168.1.101").toString());
    ui->lineEdit_15->setText(set.value("user2",3777).toString());
    ui->lineEdit_16->setText(set.value("password2","ABCabc123").toString());
    ui->comboBox_5->setCurrentIndex( set.value("camType2",0).toInt());
    set.endGroup();

    set.beginGroup("channel3");
    ui->lineEdit_17->setText(set.value("address1","192.168.1.101").toString());
    ui->lineEdit_18->setText(set.value("user1",3777).toString());
    ui->lineEdit_19->setText(set.value("password1","ABCabc123").toString());
    ui->comboBox_6->setCurrentIndex( set.value("camType1",0).toInt());

    ui->lineEdit_20->setText(set.value("address2","192.168.1.101").toString());
    ui->lineEdit_21->setText(set.value("user2",3777).toString());
    ui->lineEdit_22->setText(set.value("password2","ABCabc123").toString());
    ui->comboBox_7->setCurrentIndex( set.value("camType2",0).toInt());
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
    set.endGroup();

    set.beginGroup("Server");
    set.setValue("model",ui->radioButton->isChecked()?1:0);
    set.setValue("address",ui->lineEdit_9->text());
    set.setValue("port",ui->lineEdit_10->text().toInt());
    set.setValue("heart",ui->checkBox_2->isChecked()?1:0);
    set.endGroup();

    set.beginGroup("channel1");
    set.setValue("address1",ui->lineEdit_2->text());
    set.setValue("user1",ui->lineEdit_3->text());
    set.setValue("password1",ui->lineEdit_4->text());
    set.setValue("camType1",ui->comboBox_2->currentIndex());
    set.setValue("address2",ui->lineEdit_5->text());
    set.setValue("user2",ui->lineEdit_6->text());
    set.setValue("password2",ui->lineEdit_7->text());
    set.setValue("camType2",ui->comboBox_3->currentIndex());
    set.endGroup();

    set.beginGroup("channel2");
    set.setValue("address1",ui->lineEdit_11->text());
    set.setValue("user1",ui->lineEdit_12->text());
    set.setValue("password1",ui->lineEdit_13->text());
    set.setValue("camType1",ui->comboBox_4->currentIndex());
    set.setValue("address2",ui->lineEdit_14->text());
    set.setValue("user2",ui->lineEdit_15->text());
    set.setValue("password2",ui->lineEdit_16->text());
    set.setValue("camType2",ui->comboBox_5->currentIndex());
    set.endGroup();

    set.beginGroup("channel3");
    set.setValue("address1",ui->lineEdit_17->text());
    set.setValue("user1",ui->lineEdit_18->text());
    set.setValue("password1",ui->lineEdit_19->text());
    set.setValue("camType1",ui->comboBox_6->currentIndex());
    set.setValue("address2",ui->lineEdit_20->text());
    set.setValue("user2",ui->lineEdit_21->text());
    set.setValue("password2",ui->lineEdit_22->text());
    set.setValue("camType2",ui->comboBox_7->currentIndex());
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
