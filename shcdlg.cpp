#include "shcdlg.h"
#include "ui_shcdlg.h"
#include <QtGui>
#include <QMessageBox>
#include <QTime>

ShcDlg::ShcDlg(QString str, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShcDlg)
{
    ui->setupUi(this);
    bookid = str;

    QSqlQuery query;
    query.exec(QString("select book_name from books where book_id='%1'").arg(bookid));
    query.next();
    QString name = query.value(0).toString();

    query.exec(QString("SELECT book_price FROM books WHERE book_id = '%1'").arg(bookid));
    query.next();
    double payment = query.value(0).toDouble();
    QString s_payment = QString::number(payment);

    ui->nameEdit->setText(name);
    ui->paymentEdit->setText(s_payment);
}

ShcDlg::~ShcDlg()
{
    delete ui;
}

void ShcDlg::on_PayBtn_clicked()
{
    QString s_payment = ui->paymentEdit->text();
    double payment = s_payment.toDouble();
    int number = ui->spinBox->value();
    extern QString user_id;
    QSqlQuery query;
    query.exec(QString("SELECT stock FROM books WHERE book_id = '%1'").arg(bookid));
    query.next();
    int stock = query.value(0).toInt();
    if(stock < number){
        QMessageBox::about(NULL,tr("消息"),tr("\t库存不足!\t\t"));
    }
    else{

        query.exec(QString("select * from shopcart where book_id='%1' AND user_id='%2'").arg(bookid).arg(user_id));
        query.next();
        QString jianyan = query.value(0).toString();
        if(jianyan.isNull()){
            query.exec(QString("insert into shopcart values('%1','%2',%3,%4)").arg(user_id).arg(bookid).arg(payment).arg(number));
        }
        else{
            query.exec(QString("update shopcart set s_number=s_number+%1 where book_id='%2' AND user_id='%3'").arg(number).arg(bookid).arg(user_id));
            query.exec(QString("update shopcart set payment=payment+%1 where book_id='%2' AND user_id='%3'").arg(payment).arg(bookid).arg(user_id));
        }

        query.exec(QString("update books set stock=stock-%1 where book_id='%2'").arg(number).arg(bookid));
        QMessageBox::about(this,tr("消息"),tr("\t已加入购物车!\t\t"));

        query.exec(QString("select l_score from history where user_id='%1' AND book_id='%2'").arg(user_id).arg(bookid));
        query.next();
        int n = query.value(0).toInt();
        if(n!=3){
            query.exec(QString("update history set l_score=2 where user_id='%1' AND book_id='%2'").arg(user_id).arg(bookid));
        }

        this->close();
    }
}

void ShcDlg::on_spinBox_valueChanged(int arg1)
{
    QSqlQuery query;
    query.exec(QString("SELECT book_price FROM books WHERE book_id = '%1'").arg(bookid));
    query.next();
    double payment = query.value(0).toDouble();
    double total_payment = payment * arg1;
    QString s_payment = QString::number(total_payment);
    ui->paymentEdit->setText(s_payment);
}
