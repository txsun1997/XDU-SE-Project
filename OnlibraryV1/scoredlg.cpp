#include "scoredlg.h"
#include "ui_scoredlg.h"
#include <QtGui>
#include <QMessageBox>"

ScoreDlg::ScoreDlg(int str, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScoreDlg)
{
    ui->setupUi(this);
    QSqlQuery query;
    query.exec(QString("SELECT book_id FROM orders WHERE order_id = %1;").arg(str));
    // 读取SQL语句查询结果
    query.next();
    bookid = query.value(0).toString();
    query.exec(QString("SELECT book_name FROM books WHERE book_id = '%1';").arg(bookid));
    query.next();
    QString book_name = query.value(0).toString();
    ui->nameEdit->setText(book_name.trimmed());
    ui->textEdit->setPlaceholderText(QString("Write your Review..."));
}

ScoreDlg::~ScoreDlg()
{
    delete ui;
}

void ScoreDlg::on_commitBtn_clicked()
{
    int score = ui->scorespinBox->value();
    /*int a[100];
    QString comment = ui->textEdit->toPlainText();
    int len = comment.length();
    for(int i = 0; i < len; ++i)
    {
        QChar t = comment.at(i);
        a[i] = t.toAscii() - '0';
    }*/
    QString comment = ui->textEdit->toPlainText();
    extern QString user_id;
    QSqlQuery query;
    query.exec(QString("INSERT INTO scores VALUES('%1','%2',%3,'%4')").arg(bookid).arg(user_id).arg(score).arg(comment));
    QMessageBox::about(this,tr("消息"),tr("\t提交成功!\t\t"));
    this->close();
}
