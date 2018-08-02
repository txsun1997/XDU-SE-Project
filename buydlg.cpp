#include "buydlg.h"
#include "ui_buydlg.h"
#include <QtGui>
#include <QMessageBox>

BuyDlg::BuyDlg(QString str1, QString str2, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BuyDlg)
{
    ui->setupUi(this);
    user_id = str1;
    book_id = str2;
    QSqlQuery query;
    query.exec(QString("SELECT book_price FROM books WHERE book_id = '%1'").arg(book_id));
    query.next();
    double payment = query.value(0).toDouble();
    query.exec(QString("insert into orders(user_id,book_id,total_payment,order_time,order_state,number,flag) "
                       "values('%1','%2',%3,GETDATE(),'UNSHIPPED',1,0)").arg(user_id).arg(book_id).arg(payment));
    query.exec(QString("select max(order_id) from orders"));
    query.next();
    int order_id = query.value(0).toInt();
    qDebug() << order_id << endl;
    query.exec(QString("select order_time from orders where order_id=%1").arg(order_id));
    query.next();
    QString time = query.value(0).toString();
    time.replace(QString("T"),QString("  "));
    query.exec(QString("select book_name from books where book_id='%1'").arg(book_id));
    query.next();
    QString name = query.value(0).toString();
    QString s_order = QString::number(order_id,10);
    ui->orderidEdit->setText(s_order);
    ui->nameEdit->setText(name);
    ui->useridEdit->setText(user_id);
    QString s_payment = QString::number(payment);
    ui->paymentEdit->setText(s_payment);
    ui->timeEdit->setText(time);
}

BuyDlg::~BuyDlg()
{
    delete ui;
}

void BuyDlg::on_spinBox_valueChanged(int arg1)
{
    QSqlQuery query;
    query.exec(QString("SELECT book_price FROM books WHERE book_id = '%1'").arg(book_id));
    query.next();
    double payment = query.value(0).toDouble();
    double total_payment = payment * arg1;
    QString s_payment = QString::number(total_payment);
    ui->paymentEdit->setText(s_payment);
}

void BuyDlg::on_PayBtn_clicked()
{
    int number = ui->spinBox->value();
    QSqlQuery query;
    query.exec(QString("SELECT stock FROM books WHERE book_id = '%1'").arg(book_id));
    query.next();
    int stock = query.value(0).toInt();
    if(stock < number){
        QMessageBox::about(NULL,tr("消息"),tr("\t库存不足!\t\t"));
    }
    else{
        query.exec(QString("select max(order_id) from orders"));
        query.next();
        int order_id = query.value(0).toInt();
        query.exec(QString("update orders set flag=1 where order_id=%1").arg(order_id));
        query.exec(QString("update orders set number=%1 where order_id=%2").arg(number).arg(order_id));
        QString s_payment = ui->paymentEdit->text();
        double payment = s_payment.toDouble();
        query.exec(QString("update orders set total_payment=%1 where order_id=%2").arg(payment).arg(order_id));
        query.exec(QString("update books set stock=stock-%1 where book_id='%2'").arg(number).arg(book_id));
        QMessageBox::about(NULL,tr("消息"),tr("\t购买成功!\t\t"));
        this->close();
    }
}
