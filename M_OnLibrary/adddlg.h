#ifndef ADDDLG_H
#define ADDDLG_H

#include <QDialog>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QTextCodec>

namespace Ui {
class AddDlg;
}

class AddDlg : public QDialog
{
    Q_OBJECT

public:
    explicit AddDlg(QString str, QWidget *parent = 0);
    ~AddDlg();

private slots:
    void on_confirmBtn_clicked();

private:
    Ui::AddDlg *ui;
    QString bookid;
};

#endif // ADDDLG_H
