#include "mainwindow.h"
#include <QApplication>
#include <QtSql>
#include "logindlg.h"
#include <QtGui>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QTextCodec>
QString user_id;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString user_id;
    // 连接数据库
    QSqlDatabase db=QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName(QString("DRIVER={SQL SERVER};SERVER=(local);DATABASE=onlibrary;UID=sa;PWD=stx15650;"));
    if(!db.open()) {
        QMessageBox::warning(0,QObject::tr("Warning"),QObject::tr("连接数据库失败！"),QMessageBox::Yes);
        return 0;
    }
    else{
        qDebug() << "Connect Success!" <<endl;
    }

    // 创建登录界面
    loginDlg login;
    login.setWindowTitle(QString("登录"));
    if(login.exec()==QDialog::Accepted){
        MainWindow w;
        w.show();

        QSqlQuery query;
        extern QString user_id;
        query.exec(QString("select msg from users where user_id='%1'").arg(user_id));
        query.next();
        int oid = query.value(0).toInt();
        if(oid){
            QMessageBox::about(NULL,"New Message",QString("\tYour order( No.%1 ) has been deleted!\t\t").arg(oid));
            query.exec(QString("update users set msg=0 where user_id='%1'").arg(user_id));
        }
        return a.exec();
    }
    return 0;
}

