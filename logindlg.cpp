#include "logindlg.h"
#include "ui_logindlg.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QTextCodec>
#include <QtGui>
#include "mainwindow.h"
#include <QStandardItemModel>
#include <QCompleter>
#include "zhaohuidlg.h"

loginDlg::loginDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::loginDlg)
{
    ui->setupUi(this);
    //ui->lineEdit_pwd->setEchoMode(QLineEdit::Password);
    ui->lineEdit_user->setPlaceholderText(QStringLiteral("[E-mail]"));
    ui->lineEdit_pwd->setPlaceholderText(QStringLiteral("[Password]"));
    m_Model = new QStandardItemModel(0, 1, this);
    m_completer = new QCompleter(m_Model, this);
    ui->lineEdit_user->setCompleter(m_completer);
    QRegExp regx("[a-zA-Z0-9@.]+$");
    QValidator *validator = new QRegExpValidator(regx);
    ui->lineEdit_user->setValidator( validator );

    connect(m_completer, SIGNAL(activated(const QString&)), this, SLOT(onUserChoosed(const QString&)));
    connect(ui->lineEdit_user, SIGNAL(textChanged(const QString&)), this, SLOT(onTextChanged(const QString&)));

    ui->PLabel->setPixmap(QPixmap("E:/SE_work/Qt_project/OnlibraryV1/BGP.jpg"));
    ui->ULabel->setPixmap(QPixmap("E:/SE_work/Qt_project/OnlibraryV1/user.png"));
}

loginDlg::~loginDlg()
{
    delete ui;
}

void loginDlg::on_loginBtn_clicked()
{
    extern QString user_id;
    user_id = ui->lineEdit_user->text();
    //QString pwd = ui->lineEdit_pwd->text().trimmed();
    QString pwd = ui->lineEdit_pwd->getPassword();
    // 执行数据库SQL语句
    QSqlQuery query;
    query.exec(QString("SELECT pwd FROM users WHERE user_id = '%1';").arg(user_id));

    // 读取SQL语句查询结果
    query.next();
    QString sql_pwd = query.value(0).toString().trimmed();

    // 验证用户名是否存在
    if(sql_pwd.isNull()){ // 用户不存在
        QMessageBox::warning(this,QObject::tr("警告"),QObject::tr("\t用户名不存在!\t\t"),QMessageBox::Yes);
        ui->lineEdit_user->clear();
        //ui->lineEdit_pwd->clear();
        ui->lineEdit_user->setFocus();
    }
    else{ //用户存在
        // 处理读取的密码，删除空格
        //int endOfPwd = sql_pwd.indexOf(" ");
        //QString correct_pwd = sql_pwd.left(endOfPwd);
        query.exec(QString("select substring(sys.fn_VarBinToHexStr(hashbytes('MD5','%1')),3,32)").arg(pwd));
        query.next();
        pwd = query.value(0).toString().trimmed();
        // 验证密码是否正确
        if(pwd == sql_pwd)
            accept();
        else{
            QMessageBox::warning(this,QObject::tr("警告"),QObject::tr("\t密码错误!\t\t"),QMessageBox::Yes);
            //ui->lineEdit_user->clear();
            //ui->lineEdit_pwd->clear();
            ui->lineEdit_pwd->setFocus();
        }
    }
}

void loginDlg::on_registerBtn_clicked()
{
    registerDlg my_register;
    my_register.setWindowTitle(QObject::tr("注册"));
    if(my_register.exec()==QDialog::Accepted){
        my_register.close();
    }
}

void loginDlg::onUserChoosed(const QString& user)
{
    ui->lineEdit_user->clear();    // 清除已存在的文本更新内容
    ui->lineEdit_user->setText(user);
}

void loginDlg::onTextChanged(const QString& str)
{
    if(str.contains("@"))   return;
    QStringList strlist;
    strlist << "@126.com" << "@hotmail.com" << "@gmail.com" << "@163.com" << "@qq.com" ;

    m_Model->removeRows(0, m_Model->rowCount());
    for (int i = 0; i < strlist.size(); ++i)
    {
        m_Model->insertRow(0);
        m_Model->setData(m_Model->index(0, 0), str + strlist.at(i));
    }
}

void loginDlg::on_zhaohuiBtn_clicked()
{
    ZhaohuiDlg zhaohui;
    zhaohui.exec();
}
