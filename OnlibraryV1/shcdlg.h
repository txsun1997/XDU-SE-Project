#ifndef SHCDLG_H
#define SHCDLG_H

#include <QDialog>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QTextCodec>

namespace Ui {
class ShcDlg;
}

class ShcDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ShcDlg(QString str, QWidget *parent = 0);
    ~ShcDlg();

private slots:
    void on_PayBtn_clicked();

    void on_spinBox_valueChanged(int arg1);

private:
    Ui::ShcDlg *ui;
    QString bookid;
};

#endif // SHCDLG_H
