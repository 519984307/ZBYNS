#ifndef TESTDIALOG_H
#define TESTDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QTimer>

namespace Ui {
class TestDialog;
}

class TestDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TestDialog(QWidget *parent = nullptr);
    ~TestDialog();

private slots:

    ///
    /// \brief slotCapTimer 循环模拟抓拍
    ///
    void slotCapTimer();

    ///
    /// \brief on_pushButton_clicked 本地识别
    ///
    void on_pushButton_clicked();

    void on_checkBox_clicked(bool checked);

    void on_radioButton_2_clicked(bool checked);

    void on_pushButton_2_clicked();

private:
    Ui::TestDialog *ui;

    ///
    /// \brief capTimer 循环抓拍
    ///
    QTimer* capTimer;

signals:

    ///
    /// \brief signalRecognitionPicture 识别本地图片
    /// \param imgPath
    ///
    void signalRecognitionPicture(int type,QString img);


    ///
    /// \brief signalTestCap 测试抓拍图片
    /// \param signature
    ///
    void signalTestCap(int channel);
};

#endif // TESTDIALOG_H
