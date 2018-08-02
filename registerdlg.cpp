#include "registerdlg.h"
#include "ui_registerdlg.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QTextCodec>
#include <QtDebug>
#include <QTime>

registerDlg::registerDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::registerDlg)
{
    ui->setupUi(this);
    ui->lineEdit_rpwd->setEchoMode(QLineEdit::Password);
    QRegExp regx("[0-9]+$");
    QValidator *validator = new QRegExpValidator(regx );
    ui->lineEdit_rtel->setValidator( validator );
    QRegExp regx2("[0-9a-zA-Z@.]+$");
    QValidator *validator2 = new QRegExpValidator(regx2 );
    ui->lineEdit_ruid->setValidator( validator2 );
}

registerDlg::~registerDlg()
{
    delete ui;
}

void registerDlg::on_commitBtn_clicked()
{
    QTime t;
    t.start();
    QString r_uid = ui->lineEdit_ruid->text().trimmed();
    QString r_pwd = ui->lineEdit_rpwd->text().trimmed();
    QString r_name = ui->lineEdit_rname->text();
    QString r_address = ui->lineEdit_raddress->text();
    QString r_tel = ui->lineEdit_rtel->text();
    if(r_pwd==""){
        QMessageBox::warning(this,QObject::tr("警告"),QObject::tr("\tPassword Cannot be Null!\t\t"),QMessageBox::Yes);
        return;
    }
    // 查询账户名是否已经存在
    QSqlQuery query;
    query.exec(QString("SELECT * FROM users WHERE user_id = '%1'").arg(r_uid));
    if(query.next()){//用户名已存在
        QMessageBox::warning(this,QObject::tr("警告"),QObject::tr("\t用户名已存在!\t\t"),QMessageBox::Yes);
        ui->lineEdit_ruid->clear();
        ui->lineEdit_rpwd->clear();
        ui->lineEdit_rname->clear();
        ui->lineEdit_raddress->clear();
        ui->lineEdit_rtel->clear();
        ui->lineEdit_ruid->setFocus();
    }
    else{
        QSqlQuery query2;
        query2.exec(QString("select substring(sys.fn_VarBinToHexStr(hashbytes('MD5','%1')),3,32)").arg(r_pwd));
        query2.next();
        r_pwd = query2.value(0).toString();
        query2.prepare("INSERT INTO users "
                       "VALUES (?, ?, ?, ?, ?, ?, ?)");
        query2.addBindValue(r_uid);
        query2.addBindValue(r_pwd);
        query2.addBindValue(r_name);
        query2.addBindValue("normal");
        query2.addBindValue(r_address);
        query2.addBindValue(r_tel);
        query2.addBindValue(0);
        query2.exec();
        query2.exec(QString("select * from users where user_id='%1'").arg(r_uid));
        query2.next();
        QString jianyan = query2.value(0).toString();
        if(jianyan.isNull()){
            QMessageBox::information(NULL,QObject::tr("消息"),QObject::tr("\t注册失败!\t\t"),QMessageBox::Yes);
        }
        else{
            QMessageBox::information(NULL,QObject::tr("消息"),QObject::tr("\t注册成功!\t\t"),QMessageBox::Yes);
        }

        accept();
        qDebug() << t.elapsed() << endl;
    }
}
