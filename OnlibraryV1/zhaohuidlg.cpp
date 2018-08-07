#include "zhaohuidlg.h"
#include "ui_zhaohuidlg.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QTextCodec>
#include <QtDebug>
#include <QTime>

ZhaohuiDlg::ZhaohuiDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ZhaohuiDlg)
{
    ui->setupUi(this);
    QRegExp regx("[0-9a-zA-Z@.]+$");
    QValidator *validator = new QRegExpValidator(regx );
    ui->lineEdit_ruid->setValidator( validator );
    QRegExp regx2("[0-9]+$");
    QValidator *validator2 = new QRegExpValidator(regx );
    ui->lineEdit_tel->setValidator( validator2 );
    QWidget::setTabOrder(ui->lineEdit_ruid,ui->lineEdit_name);
    QWidget::setTabOrder(ui->lineEdit_name,ui->lineEdit_tel);
}

ZhaohuiDlg::~ZhaohuiDlg()
{
    delete ui;
}

void ZhaohuiDlg::on_commitBtn_clicked()
{
    QTime t;
    t.start();
    QString email = ui->lineEdit_ruid->text();
    QString name = ui->lineEdit_name->text();
    QString tel = ui->lineEdit_tel->text();
    QSqlQuery query;
    query.exec(QString("select user_name,user_tel from users where user_id = '%1'").arg(email));
    query.next();
    QString cname = query.value(0).toString().trimmed();
    QString ctel = query.value(1).toString().trimmed();
    if(cname.isNull()){
        QMessageBox::about(this,tr("消息"),tr("  E-mail dosn't Exit!\t  "));
    }
    else{
        if( !QString::compare(name,cname) && !QString::compare(tel,ctel)){
            query.exec(QString("update users set pwd='e10adc3949ba59abbe56e057f20f883e' where user_id='%1'").arg(email));
            QMessageBox::information(NULL,QObject::tr("消息"),"  Your Password has been Updated As '123456'!  \n"
                                                             "          Please Modify Your Password!",QMessageBox::Yes);
            this->close();
        }
        else{
            QMessageBox::about(this,tr("消息"),tr("  Error in Name or Tel!\t  "));
        }
    }
    qDebug() << t.elapsed() << endl;

}
