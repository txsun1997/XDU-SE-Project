#include "updatedlg.h"
#include "ui_updatedlg.h"
#include <QtGui>
#include <QMessageBox>

UpdateDlg::UpdateDlg(QString str, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateDlg)
{
    ui->setupUi(this);
    bookid = str;
    // 执行数据库SQL语句
    QSqlQuery query;
    query.exec(QString("SELECT book_name,book_price,book_press,writer,info FROM books WHERE book_id = '%1';").arg(str));

    // 读取SQL语句查询结果
    query.next();
    QString name = query.value(0).toString();

    QString price = query.value(1).toString();

    QString press = query.value(2).toString();

    QString writer = query.value(3).toString();

    QString info = query.value(4).toString();

    ui->idEdit->setText(bookid.trimmed());
    ui->nameEdit->setText(name.trimmed());
    ui->priceEdit->setText(price);
    ui->pressEdit->setText(press.trimmed());
    ui->writerEdit->setText(writer);
    ui->textEdit->setText(info);
}

UpdateDlg::~UpdateDlg()
{
    delete ui;
}

void UpdateDlg::on_updateBtn_clicked()
{
    QSqlQuery query;
    QString name = ui->nameEdit->text();
    QString price = ui->priceEdit->text();
    QString press = ui->pressEdit->text();
    QString writer = ui->writerEdit->text();
    QString info = ui->textEdit->toPlainText();

    query.exec(QString("update books set book_name='%1' where book_id = '%2'").arg(name).arg(bookid));
    query.exec(QString("update books set book_price='%1' where book_id = '%2'").arg(price).arg(bookid));
    query.exec(QString("update books set book_press='%1' where book_id = '%2'").arg(press).arg(bookid));
    query.exec(QString("update books set writer='%1' where book_id = '%2'").arg(writer).arg(bookid));
    query.exec(QString("update books set info='%1' where book_id = '%2'").arg(info).arg(bookid));
    QMessageBox::about(this,tr("消息"),tr("\t修改成功!\t\t"));
    this->close();

}
