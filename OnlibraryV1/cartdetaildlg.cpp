#include "cartdetaildlg.h"
#include "ui_cartdetaildlg.h"
#include <QtGui>
#include <QMessageBox>

CartDetailDlg::CartDetailDlg(QString str, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CartDetailDlg)
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
    ui->nameEdit->setText(name.trimmed());
    ui->priceEdit->setText(price);
    ui->pressEdit->setText(press.trimmed());
    ui->writerEdit->setText(writer);
    if(info.isNull()){
        ui->textBrowser->setText(QString(" No Introduction yet..."));
    }
    else{
        ui->textBrowser->setText(info);
    }
    query.exec(QString("select AVG(b_score) from scores where book_id='%1';").arg(bookid));
    query.next();
    double score = query.value(0).toDouble();
    if(score==0){
        ui->scoreEdit->setText(" No scores yet...");
    }
    else{
        QString s_score = QString::number(score);
        ui->scoreEdit->setText(s_score);
    }

}

CartDetailDlg::~CartDetailDlg()
{
    delete ui;
}

void CartDetailDlg::on_buyBtn_clicked()
{
    QSqlQuery query;
    extern QString user_id;
    query.exec(QString("SELECT payment,s_number FROM shopcart WHERE book_id='%1' AND user_id='%2'").arg(bookid).arg(user_id));
    query.next();
    double payment = query.value(0).toDouble();
    int number = query.value(1).toInt();
    //order_id,user_id,bookid,payment
    //QString state = tr("未发货");
    query.exec(QString("insert into orders(user_id,book_id,total_payment,order_time,order_state,number,flag) "
                       "values('%1','%2',%3,GETDATE(),'UNSHIPPED',%4,1)").arg(user_id).arg(bookid).arg(payment).arg(number));
    QMessageBox::information(this,tr("消息"),tr("\t购买成功!\t\t"),QMessageBox::Yes);
    query.exec(QString("delete from shopcart where user_id='%1' AND book_id='%2'").arg(user_id).arg(bookid));

    query.exec(QString("update history set l_score=3 where user_id='%1' AND book_id='%2'").arg(user_id).arg(bookid));
    this->close();
}

void CartDetailDlg::on_DeleteBtn_clicked()
{

    if (QMessageBox::Yes == QMessageBox::question(NULL, tr("DELETE"),tr(" Confirm to delete?\t "),
                                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
    {
        QSqlQuery query;
        extern QString user_id;
        query.exec(QString("select s_number from shopcart where user_id='%1' AND book_id='%2'").arg(user_id).arg(bookid));
        query.next();
        int number = query.value(0).toInt();
        query.exec(QString("update books set stock=stock+%1 where book_id='%2'").arg(number).arg(bookid));

        query.exec(QString("delete from shopcart where user_id='%1' AND book_id='%2'").arg(user_id).arg(bookid));
        QMessageBox::information(this,tr("消息"),tr("\t删除成功!\t\t"),QMessageBox::Yes);
        this->close();
    }

}
