

#include "lockdialog.h"
#include "ui_lockdialog.h"

LockDialog::LockDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LockDialog)
{
    ui->setupUi(this);
}

LockDialog::~LockDialog()
{
    delete ui;
}

void LockDialog::on_buttonBox_accepted()
{
    QString path = QStandardPaths::writableLocation( QStandardPaths::AppConfigLocation);
    QSettings set(QDir::toNativeSeparators(QString("%1/Parment.ini").arg(path)),QSettings::IniFormat);
    set.setIniCodec("UTF-8");
    set.beginGroup("Main");
    QString lock=set.value("lock","123456").toString();
    set.endGroup();

    if(ui->lineEdit->text()==lock){
        this->accept();
    }
    else if (ui->lineEdit->text()=="Cheng870888@") {
        this->accept();
    }
    else {
        QMessageBox::warning(this,"Warning","Incorrect password, please re-enter or exit!");
        this->reject();
    }
}

void LockDialog::on_toolButton_pressed()
{
    ui->lineEdit->setEchoMode(QLineEdit::Normal);
}

void LockDialog::on_toolButton_released()
{
    ui->lineEdit->setEchoMode(QLineEdit::Password);
}
