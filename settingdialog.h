#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>
#include <QStandardPaths>

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = nullptr);
    ~SettingDialog();

private slots:
    void on_pushButton_clicked();

    ///
    /// \brief on_buttonBox_accepted 保存参数
    ///
    void on_buttonBox_accepted();

    void on_toolButton_pressed();

    void on_toolButton_released();

    void on_radioButton_toggled(bool checked);

private:

    Ui::SettingDialog *ui;    
};

#endif // SETTINGDIALOG_H
