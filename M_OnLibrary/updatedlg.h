#ifndef UPDATEDLG_H
#define UPDATEDLG_H

#include <QDialog>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QTextCodec>

namespace Ui {
class UpdateDlg;
}

class UpdateDlg : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateDlg(QString str, QWidget *parent = 0);
    ~UpdateDlg();

private slots:
    void on_updateBtn_clicked();

private:
    Ui::UpdateDlg *ui;
    QString bookid;
};

#endif // UPDATEDLG_H
