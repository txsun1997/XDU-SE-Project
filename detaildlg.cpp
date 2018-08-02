#include "detaildlg.h"
#include "ui_detaildlg.h"
#include <QtGui>
#include <QMessageBox>
#include "buydlg.h"
#include "shcdlg.h"

DetailDlg::DetailDlg(QString str, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DetailDlg)
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
    QString commence;
    QString temp;
    query.exec(QString("select comment,user_id from scores where book_id='%1';").arg(bookid));
    while(query.next()){
        temp = query.value(1).toString();
        commence += temp.trimmed();
        commence += QString(" :\n");
        commence += query.value(0).toString();
        commence += QString("\n\n");
    }
    ui->commenceBrowser->setText(commence);
}

DetailDlg::~DetailDlg()
{
    delete ui;
}


void DetailDlg::on_buyBtn_clicked()
{
    QSqlQuery query;
    extern QString user_id;
    query.exec(QString("SELECT stock FROM books WHERE book_id = '%1'").arg(bookid));
    query.next();
    int stock = query.value(0).toInt();
    if(stock>=0){
        BuyDlg buy(user_id,bookid);
        buy.setWindowTitle(tr("订单信息"));
        buy.exec();
        query.exec(QString("select l_score from history where user_id='%1' AND book_id='%2'").arg(user_id).arg(bookid));
        query.next();
        int s = query.value(0).toInt();
        if(s!=3){
            query.exec(QString("update history set l_score=2 where user_id='%1' AND book_id='%2'").arg(user_id).arg(bookid));
        }

    }
    else{
        QMessageBox::about(NULL,tr("消息"),tr("\t库存不足!\t\t"));
    }
}

void DetailDlg::on_ShoppingCartBtn_clicked()
{
    ShcDlg shopcart(bookid);
    shopcart.setWindowTitle(tr("Shopping Cart"));
    shopcart.exec();
}
