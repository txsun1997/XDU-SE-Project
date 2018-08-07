#ifndef LOGINDLG_H
#define LOGINDLG_H

#include <QDialog>
#include "registerdlg.h"
#include <QtGui>
#include <QCompleter>
#include "QPasswordLineEdit.h"

namespace Ui {
class loginDlg;
}

class loginDlg : public QDialog
{
    Q_OBJECT

public:
    explicit loginDlg(QWidget *parent = 0);

    ~loginDlg();

private slots:
    void on_loginBtn_clicked();

    void on_registerBtn_clicked();

    void onUserChoosed(const QString& user);

    void onTextChanged(const QString& str);

    void on_zhaohuiBtn_clicked();

private:
    Ui::loginDlg *ui;
    QStandardItemModel* m_Model ;
    QCompleter*  m_completer;

};

#endif // LOGINDLG_H
