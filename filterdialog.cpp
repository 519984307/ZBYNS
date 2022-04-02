#include "filterdialog.h"
#include "ui_filterdialog.h"

FilterDialog::FilterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilterDialog)
{
    ui->setupUi(this);

    ui->start_dateTimeEdit->setDate(QDate::currentDate());
    ui->start_dateTimeEdit->setTime(QTime(0,0,0));

    ui->end_dateTimeEdit->setDate(QDate::currentDate());
    ui->end_dateTimeEdit->setTime(QTime(23,59,59));
}

FilterDialog::~FilterDialog()
{
    delete ui;
}

void FilterDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    if(button==ui->buttonBox->button(QDialogButtonBox::Ok)){
        QStringList filterList;

        if(ui->channel_checkBox->isChecked()){
            filterList.append(QString("Channel='%1'").arg(ui->channel_spinBox->value()));
        }
        if(ui->dateTime_groupBox->isChecked()){
            filterList.append(QString("((Timer>='%1' AND Timer<='%2'))").arg(ui->start_dateTimeEdit->dateTime().toString("yyyy-MM-dd HH:mm:ss")).arg(ui->end_dateTimeEdit->dateTime().toString("yyyy-MM-dd HH:mm:ss")));
        }
        if(ui->container_groupBox->isChecked()){
            if(ui->front_lineEdit->text()!=""){
                filterList.append(QString("Number='%1'").arg(ui->front_lineEdit->text()));
            }
        }
        if(ui->plate_checkBox->isChecked()){
            filterList.append(QString("Weight='%1'").arg(ui->plate_lineEdit->text()));
        }
        if(ui->check_checkBox->isChecked()){
            if(ui->correct_radioButton->isChecked()){
                filterList.append(QString("Check='%1'").arg(1));
            }
            if(ui->error_radioButton->isChecked()){
                filterList.append(QString("CheckFront='%1'").arg(0));
            }
        }

        emit signal_filterData(filterList.join(" AND "));
    }
}
