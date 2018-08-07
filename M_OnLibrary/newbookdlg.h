#ifndef NEWBOOKDLG_H
#define NEWBOOKDLG_H

#include <QDialog>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QTextCodec>

namespace Ui {
class NewbookDlg;
}

class NewbookDlg : public QDialog
{
    Q_OBJECT

public:
    explicit NewbookDlg(QWidget *parent = 0);
    ~NewbookDlg();

private slots:
    void on_updateBtn_clicked();

private:
    Ui::NewbookDlg *ui;
};

#endif // NEWBOOKDLG_H
