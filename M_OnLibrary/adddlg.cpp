#include "adddlg.h"
#include "ui_adddlg.h"
#include <QtGui>
#include <QMessageBox>

AddDlg::AddDlg(QString str, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDlg)
{
    ui->setupUi(this);
    bookid = str;
    QSqlQuery query;
    query.exec(QString("SELECT stock FROM books WHERE book_id = '%1';").arg(str));
    // 读取SQL语句查询结果
    query.next();
    int stock = query.value(0).toInt();
    QString s_stock = QString::number(stock,10);
    ui->stockEdit->setText(s_stock);
}

AddDlg::~AddDlg()
{
    delete ui;
}

void AddDlg::on_confirmBtn_clicked()
{
    int inc = ui->spinBox->value();
    QSqlQuery query;
    query.exec(QString("UPDATE books SET stock=stock+%1 WHERE book_id = '%2';").arg(inc).arg(bookid));
    query.exec(QString("SELECT stock FROM books WHERE book_id = '%1';").arg(bookid));
    // 读取SQL语句查询结果
    query.next();
    int ans = query.value(0).toInt();
    QString s_old = ui->stockEdit->text();
    int old = s_old.toInt();
    int th = old + inc;
    if(ans == th){
        QMessageBox::about(this,tr("消息"),tr("\t新增成功!\t\t"));
    }
    else{
        QMessageBox::about(this,tr("消息"),tr("\t新增失败!\t\t"));
    }
    this->close();
}
