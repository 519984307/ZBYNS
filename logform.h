#ifndef LOGFORM_H
#define LOGFORM_H

#include <QWidget>
#include <QDateTime>

namespace Ui {
class LogForm;
}

class LogForm : public QWidget
{
    Q_OBJECT

public:
    explicit LogForm(QWidget *parent = nullptr);
    ~LogForm();

public slots:

    void slot_newLogText(QtMsgType type,QDateTime time,QString value);

private slots:
    void on_pushButton_clicked();

    void on_plainTextEdit_textChanged();

private:
    Ui::LogForm *ui;

    ///
    /// \brief startTime 开始时间戳
    ///
    QString startTime;
};

#endif // LOGFORM_H
