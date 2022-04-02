#ifndef TESTDIALOG_H
#define TESTDIALOG_H

#include <QDialog>
#include <QFileDialog>

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
    /// \brief on_buttonBox_accepted 确认抓拍
    ///
    void on_buttonBox_accepted();

    ///
    /// \brief on_buttonBox_rejected 取消抓拍
    ///
    void on_buttonBox_rejected();

    ///
    /// \brief on_pushButton_clicked 本地识别
    ///
    void on_pushButton_clicked();

private:
    Ui::TestDialog *ui;

    ///
    /// \brief isChoose 识别本地图片状态
    ///
    bool isChoose;

signals:

    ///
    /// \brief signalRecognitionPicture 识别本地图片
    /// \param imgPath
    ///
    void signalRecognitionPicture(int type,QString img);
};

#endif // TESTDIALOG_H
