#include "mainwindow.h"
#include <QApplication>
#include "logindlg.h"
#include <QtSql>
#include <QtGui>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

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
        return a.exec();
    }
    return 0;
}
