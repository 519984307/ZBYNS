#ifndef LOCKDIALOG_H
#define LOCKDIALOG_H

#include <QDialog>
#include <QStandardPaths>
#include <QSettings>
#include <QDir>
#include <QMessageBox>

namespace Ui {
class LockDialog;
}

class LockDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LockDialog(QWidget *parent = nullptr);
    ~LockDialog();

private slots:
    void on_buttonBox_accepted();

    void on_toolButton_pressed();

    void on_toolButton_released();

private:
    Ui::LockDialog *ui;
};

#endif // LOCKDIALOG_H
