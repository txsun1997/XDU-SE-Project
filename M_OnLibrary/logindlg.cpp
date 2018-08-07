#include "logindlg.h"
#include "ui_logindlg.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QTextCodec>
#include <QtGui>
#include "mainwindow.h"

loginDlg::loginDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::loginDlg)
{
    ui->setupUi(this);
    ui->lineEdit_pwd->setEchoMode(QLineEdit::Password);
    ui->lineEdit_user->setFocus();
}

loginDlg::~loginDlg()
{
    delete ui;
}

void loginDlg::on_loginBtn_clicked()
{
    QString user_id;
    user_id = ui->lineEdit_user->text().trimmed();
    QString pwd = ui->lineEdit_pwd->text().trimmed();

    // 执行数据库SQL语句
    QSqlQuery query;
    query.exec(QString("SELECT pwd,user_type FROM users WHERE user_id = '%1';").arg(user_id));

    // 读取SQL语句查询结果
    query.next();
    QString sql_pwd = query.value(0).toString();
    QString type = query.value(1).toString();
    //qDebug() << type << endl;
    // 验证用户名是否存在
    QString admin = "admin";
    if(sql_pwd.isNull() || type.trimmed() != admin){ // 用户不存在
        QMessageBox::warning(this,QObject::tr("警告"),QObject::tr("\t用户名不存在!\t\t"),QMessageBox::Yes);
        ui->lineEdit_user->clear();
        ui->lineEdit_pwd->clear();
        ui->lineEdit_user->setFocus();
    }
    else{ //用户存在
        // 处理读取的密码，删除空格
        int endOfPwd = sql_pwd.indexOf(" ");
        QString correct_pwd = sql_pwd.left(endOfPwd);

        // 验证密码是否正确
        if(pwd == correct_pwd)
            accept();
        else{
            QMessageBox::warning(this,QObject::tr("警告"),QObject::tr("\t密码错误!\t\t"),QMessageBox::Yes);
            ui->lineEdit_user->clear();
            ui->lineEdit_pwd->clear();
            ui->lineEdit_user->setFocus();
        }
    }
}
