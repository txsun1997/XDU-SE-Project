#ifndef CARTDETAILDLG_H
#define CARTDETAILDLG_H

#include <QDialog>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QTextCodec>
#include "mainwindow.h"

namespace Ui {
class CartDetailDlg;
}

class CartDetailDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CartDetailDlg(QString str, QWidget *parent = 0);
    ~CartDetailDlg();

private slots:
    void on_buyBtn_clicked();

    void on_DeleteBtn_clicked();

private:
    Ui::CartDetailDlg *ui;
    QString bookid;

};

#endif // CARTDETAILDLG_H
