#ifndef DETAILDLG_H
#define DETAILDLG_H

#include <QDialog>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QTextCodec>

namespace Ui {
class DetailDlg;
}

class DetailDlg : public QDialog
{
    Q_OBJECT

public:
    explicit DetailDlg(QString str, QWidget *parent = 0);
    ~DetailDlg();

private slots:

    void on_buyBtn_clicked();

    void on_ShoppingCartBtn_clicked();

private:
    Ui::DetailDlg *ui;
    QString bookid;
};

#endif // DETAILDLG_H
