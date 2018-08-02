#ifndef BUYDLG_H
#define BUYDLG_H

#include <QDialog>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QTextCodec>

namespace Ui {
class BuyDlg;
}

class BuyDlg : public QDialog
{
    Q_OBJECT

public:
    explicit BuyDlg(QString str1, QString str2, QWidget *parent = 0);
    ~BuyDlg();

private slots:
    void on_spinBox_valueChanged(int arg1);

    void on_PayBtn_clicked();

private:
    Ui::BuyDlg *ui;
    QString user_id;
    QString book_id;
};

#endif // BUYDLG_H
