#include "logform.h"
#include "ui_logform.h"

LogForm::LogForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogForm)
{
    ui->setupUi(this);

    startTime=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz");
    ui->plainTextEdit->appendPlainText(startTime);
}

LogForm::~LogForm()
{
    delete ui;
}

void LogForm::slot_newLogText(QtMsgType type, QDateTime time, QString value)
{
    QString msgType=QString("");
    switch (type) {
    case QtDebugMsg:
        msgType="Debug";
        break;
    case QtInfoMsg:
        msgType="Info";
        break;
    case QtWarningMsg:
        msgType="Warning";
        break;
    case QtCriticalMsg:
        msgType="Critical";
        break;
    case QtFatalMsg:
        msgType="Fatal";
        abort();
    }

    if(msgType.size()<8){
        int cot=8-msgType.size();
        for(int i=0;i<cot;i++){
            msgType.append(" ");
        }
    }

    ui->plainTextEdit->appendPlainText(QString("%1 %2 %3").arg(time.toString("yyyy-MM-dd hh:mm:ss zzz"),msgType,value));
}

void LogForm::on_pushButton_clicked()
{
    ui->plainTextEdit->clear();
    ui->plainTextEdit->appendPlainText(startTime);
}

void LogForm::on_plainTextEdit_textChanged()
{
    if(ui->plainTextEdit->document()->blockCount()>300){
        ui->plainTextEdit->clear();
        ui->plainTextEdit->appendPlainText(startTime);
    }
}
