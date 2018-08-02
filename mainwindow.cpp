#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlQueryModel>
#include <QTextCodec>
#include <QMessageBox>
#include <QHeaderView>
#include <QStandardItemModel>
#include "buttondelegate.h"
#include "orderBtndelegate.h"
#include "cartbuttondelegate.h"
#include <math.h>
#include <QShortcut>
#include "logindlg.h"
#include <QThread>

#define MAXHC 50    //最大历史记录数为50
#define MAXUN 10    //最大用户量为10

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon("logo.ico"));
    ui->mg1label->show();
    ui->mg2label->hide();
    ui->mg3label->hide();
    ui->mg4label->hide();
    ui->mg5label->hide();
    ui->mg6label->hide();
    extern QString user_id;
    ui->uidlabel->setText(user_id);

    ui->searchEdit->setPlaceholderText(QStringLiteral("Search books..."));
    ui->stackedWidget->setCurrentIndex(0);
    model.setQuery(QString("select book_id AS ID,book_name AS Name,book_price AS Price,"
                           "book_press AS Press,writer AS Writer,Options from books where stock>0"));
    ui->booksView->setModel(&model);
    ui->booksView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->booksView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->booksView->verticalHeader()->setVisible(false);
    ui->booksView->setAlternatingRowColors(true);
    ui->booksView->setFrameShape(QFrame::NoFrame);

    ui->booksView->setStyleSheet("QTableView{background-color: rgb(211,211,211);"
                                 "alternate-background-color: rgb(255,255,255);}");
    ui->booksView->horizontalHeader()->setStretchLastSection(true);
    ui->booksView->setShowGrid(false);
    ui->booksView->resizeColumnsToContents();
    ui->booksView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    //实现排序功能
    /*
    QSortFilterProxyModel *sqlproxy = new QSortFilterProxyModel(this);
    sqlproxy->setSourceModel(&model);
    ui->booksView->setModel(sqlproxy);
    ui->booksView->setSortingEnabled(true);
    */
    QString str = QString("select book_id AS ID,book_name AS Name,book_price AS Price,"
                          "book_press AS Press,writer AS Writer,Options from books where stock>0");
    ButtonDelegate * m_buttonDelegate = new ButtonDelegate(str,ui->booksView,this);
    ui->booksView->setItemDelegateForColumn(5, m_buttonDelegate);
    ui->booksView->setColumnWidth(5, 200);
    ui->booksView->setColumnWidth(0, 50);
    ui->booksView->setColumnWidth(1, 175);
    ui->booksView->setColumnWidth(3, 200);
    // 给出消息
    QWidget::setTabOrder(ui->oldpwdEdit,ui->newpwdEdit);
    QWidget::setTabOrder(ui->newpwdEdit,ui->newpwdEdit_2);

    QWidget::setTabOrder(ui->searchEdit,ui->searchBtn);
    QWidget::setTabOrder(ui->searchBtn,ui->searchEdit);

    //this->setWindowFlags (Qt::CustomizeWindowHint);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_recommendBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->mg1label->show();
    ui->mg2label->hide();
    ui->mg3label->hide();
    ui->mg4label->hide();
    ui->mg5label->hide();
    ui->mg6label->hide();
    model.setQuery(QString("select book_id AS ID,book_name AS Name,book_price AS Price,"
                           "book_press AS Press,writer AS Writer,Options from books where stock>0"));
    ui->booksView->setModel(&model);
    ui->rAllBtn->setChecked(true);
    ui->mg1label->show();
}

void MainWindow::on_orderBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->mg1label->hide();
    ui->mg2label->hide();
    ui->mg3label->show();
    ui->mg4label->hide();
    ui->mg5label->hide();
    ui->mg6label->hide();
    extern QString user_id;
    QSqlQuery query;
    query.exec(QString("delete from orders where flag=0"));
    model.setQuery(QString("select order_id AS ID,book_name AS Name,total_payment AS Payment,order_time,order_state AS State,Options "
                           "from books,(select * from orders where user_id='%1')AS temp "
                           "where books.book_id=temp.book_id").arg(user_id));

    ui->ordersView->setModel(&model);
    ui->ordersView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->ordersView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->ordersView->verticalHeader()->setVisible(false);
    ui->ordersView->setAlternatingRowColors(true);
    ui->ordersView->setFrameShape(QFrame::NoFrame);

    ui->ordersView->setStyleSheet("QTableView{background-color: rgb(211,211,211);"
                                 "alternate-background-color: rgb(255,255,255);}");
    ui->ordersView->horizontalHeader()->setStretchLastSection(true);
    ui->ordersView->setShowGrid(false);
    ui->ordersView->resizeColumnsToContents();
    ui->ordersView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

    o_ButtonDelegate * o_buttonDelegate = new o_ButtonDelegate(ui->ordersView,this);
    ui->ordersView->setItemDelegateForColumn(5, o_buttonDelegate);

    ui->ordersView->setColumnWidth(0, 50);
    ui->ordersView->setColumnWidth(1, 150);
    ui->ordersView->setColumnWidth(2, 75);
    ui->ordersView->setColumnWidth(3, 200);
    ui->ordersView->setColumnWidth(4, 125);
    ui->ordersView->setColumnWidth(5, 125);
}

void MainWindow::on_collectionBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->mg1label->hide();
    ui->mg2label->hide();
    ui->mg3label->hide();
    ui->mg4label->hide();
    ui->mg5label->show();
    ui->mg6label->hide();
}

void MainWindow::on_shopcarBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
    ui->mg1label->hide();
    ui->mg2label->hide();
    ui->mg3label->hide();
    ui->mg4label->show();
    ui->mg5label->hide();
    ui->mg6label->hide();
    extern QString user_id;
    model.setQuery(QString("select books.book_id AS ID,book_name AS Name,payment AS Payment,"
                           "s_number AS Number,writer AS Writer,Options from books,(select * from shopcart where user_id='%1')AS TEMP "
                           "where books.book_id = TEMP.book_id").arg(user_id));
    ui->cartView->setModel(&model);
    ui->cartView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->cartView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->cartView->verticalHeader()->setVisible(false);
    ui->cartView->setAlternatingRowColors(true);
    ui->cartView->setFrameShape(QFrame::NoFrame);

    ui->cartView->setStyleSheet("QTableView{background-color: rgb(211,211,211);"
                                 "alternate-background-color: rgb(255,255,255);}");
    ui->cartView->horizontalHeader()->setStretchLastSection(true);
    ui->cartView->setShowGrid(false);
    ui->cartView->resizeColumnsToContents();
    ui->cartView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    c_ButtonDelegate * m_buttonDelegate = new c_ButtonDelegate(ui->cartView,this);
    ui->cartView->setItemDelegateForColumn(5, m_buttonDelegate);
    ui->cartView->setColumnWidth(5, 200);
    ui->cartView->setColumnWidth(0, 50);
    ui->cartView->setColumnWidth(1, 175);
}

void MainWindow::on_booksView_activated(const QModelIndex &index)
{

}

void MainWindow::on_collectionBtn_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_searchBtn_clicked()
{
    QTime t;
    t.start();
    QString txt = ui->searchEdit->text();
    //qDebug() << txt << endl;
    ui->stackedWidget->setCurrentIndex(0);
    model.setQuery(QString("select book_id AS ID,book_name AS Name,book_price AS Price,"
                           "book_press AS Press,writer AS Writer,Options from books "
                           "where book_name like'%%1%' or writer like'%%2%';").arg(txt).arg(txt));
    ui->booksView->setModel(&model);
    qDebug() << t.elapsed() << endl;
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->oldpwdEdit->clear();
    ui->newpwdEdit->clear();
    ui->newpwdEdit_2->clear();
    ui->oldpwdEdit->setFocus();
}

void MainWindow::on_pushButton_clicked()
{
    QTime t;
    t.start();
    QSqlQuery query;
    extern QString user_id;
    query.exec(QString("select pwd from users where user_id='%1'").arg(user_id));
    query.next();
    QString old = ui->oldpwdEdit->text();
    QString str = query.value(0).toString().trimmed(); //str为原正确md5加密后的密码
    query.exec(QString("select substring(sys.fn_VarBinToHexStr(hashbytes('MD5','%1')),3,32)").arg(old));
    query.next();
    old = query.value(0).toString().trimmed();
    if(old == str)
    {
        QString new1 = ui->newpwdEdit->text();
        QString new2 = ui->newpwdEdit_2->text();
        if(new1==""){
            QMessageBox::information(NULL,QObject::tr("消息"),QObject::tr("\tPassword Cannot be Null!\t\t"),QMessageBox::Yes);
            return;
        }
        if(new1 == new2)
        {
            query.exec(QString("select substring(sys.fn_VarBinToHexStr(hashbytes('MD5','%1')),3,32)").arg(new1));
            query.next();
            new1 = query.value(0).toString().trimmed();
            if(new1 == old)
            {
                QMessageBox::information(this,QObject::tr("消息"),QObject::tr("\t不能与原密码相同!\t\t"),QMessageBox::Yes);
                ui->oldpwdEdit->clear();
                ui->newpwdEdit->clear();
                ui->newpwdEdit_2->clear();
                ui->oldpwdEdit->setFocus();
            }
            else
            {
                query.exec(QString("update users set pwd='%1' where user_id='%2'").arg(new1).arg(user_id));
                QMessageBox::information(this,QObject::tr("消息"),QObject::tr("\t修改成功!\t\t"),QMessageBox::Yes);
            }
        }
        else
        {
            QMessageBox::warning(this,"warning"," Password  Inconsistency!  ",QMessageBox::Yes);
            ui->oldpwdEdit->clear();
            ui->newpwdEdit->clear();
            ui->newpwdEdit_2->clear();
            ui->oldpwdEdit->setFocus();
        }
    }
    else
    {
        QMessageBox::warning(this,"warning"," Password  Error!  ",QMessageBox::Yes);
        ui->oldpwdEdit->clear();
        ui->newpwdEdit->clear();
        ui->newpwdEdit_2->clear();
        ui->oldpwdEdit->setFocus();
    }
    qDebug() << t.elapsed() << endl;
}

void MainWindow::on_InfoBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    ui->mg1label->hide();
    ui->mg2label->hide();
    ui->mg3label->hide();
    ui->mg4label->hide();
    ui->mg5label->hide();
    ui->mg6label->show();
    ui->userplabel->setPixmap(QPixmap("E:/SE_work/Qt_project/OnlibraryV1/design.png"));
    QSqlQuery query;
    extern QString user_id;
    query.exec(QString("select * from users where user_id='%1'").arg(user_id));
    query.next();
    QString name = query.value(2).toString();
    QString type = query.value(3).toString();
    QString address = query.value(4).toString();
    QString tel = query.value(5).toString();
    ui->g_useridEdit->setText(user_id);
    ui->g_nameEdit->setText(name.trimmed());
    ui->g_typeEdit->setText(type);
    ui->g_telEdit->setText(tel.trimmed());
    ui->g_addressEdit->setText(address.trimmed());
}

void MainWindow::on_pushButton_5_clicked()
{
    ui->g_nameEdit->setReadOnly(false);
    ui->g_addressEdit->setReadOnly(false);
    ui->g_telEdit->setReadOnly(false);
    ui->g_nameEdit->setFocus();
}

void MainWindow::on_pushButton_6_clicked()
{
    QString name = ui->g_nameEdit->text();
    QString address = ui->g_addressEdit->text();
    QString tel = ui->g_telEdit->text();
    extern QString user_id;
    QSqlQuery query;
    query.exec(QString("update users set user_name='%1',user_address='%2',"
                       "user_tel='%3' where user_id='%4'").arg(name).arg(address).arg(tel).arg(user_id));
    QMessageBox::information(this,tr("消息"),tr("\t修改成功!\t\t"),QMessageBox::Yes);
    /*
    query.exec(QString("select * from users where user_id='%1'").arg(user_id));
    query.next();
    name = query.value(2).toString();
    address = query.value(4).toString();
    tel = query.value(5).toString();
    ui->g_nameEdit->setText(name);
    ui->g_telEdit->setText(tel);
    ui->g_addressEdit->setText(address);
`   */
}

void MainWindow::on_chargeBtn_clicked()
{
    if (QMessageBox::Yes == QMessageBox::question(NULL, tr("Settlement"),tr(" Confirm to settle accounts?\t "),
                                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
    {
        extern QString user_id;
        model.setQuery(QString("select books.book_id AS ID,book_name AS Name,payment AS Payment,"
                               "s_number AS Number,writer AS Writer,Options from books,(select * from shopcart where user_id='%1')AS TEMP "
                               "where books.book_id = TEMP.book_id").arg(user_id));
        int i=0;

        QString bookid;
        while(1){
            bookid=model.data(model.index(i,0)).toString();
            if(bookid.isNull()){
                break;
            }
            else{
                i++;
                QSqlQuery query;
                extern QString user_id;
                query.exec(QString("SELECT payment,s_number FROM shopcart WHERE book_id='%1' AND user_id='%2'").arg(bookid).arg(user_id));
                query.next();
                double payment = query.value(0).toDouble();
                int number = query.value(1).toInt();
                query.exec(QString("insert into orders(user_id,book_id,total_payment,order_time,order_state,number,flag) "
                                   "values('%1','%2',%3,GETDATE(),'UNSHIPPED',%4,1)").arg(user_id).arg(bookid).arg(payment).arg(number));
                query.exec(QString("delete from shopcart where user_id='%1' AND book_id='%2'").arg(user_id).arg(bookid));
                // 做历史记录
                query.exec(QString("update history set l_score=3 where user_id='%1' AND book_id='%2'").arg(user_id).arg(bookid));
            }
        }
        model.setQuery(QString("select books.book_id AS ID,book_name AS Name,payment AS Payment,"
                               "s_number AS Number,writer AS Writer,Options from books,(select * from shopcart where user_id='%1')AS TEMP "
                               "where books.book_id = TEMP.book_id").arg(user_id));

        QMessageBox::information(this,tr("消息"),tr("\t结算成功!\t\t"),QMessageBox::Yes);
        ui->cartView->setModel(&model);      
    }
}

void MainWindow::on_clearBtn_clicked()
{
    if (QMessageBox::Yes == QMessageBox::question(NULL, tr("DELETE"),tr(" Confirm to delete?\t "),
                                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
    {
        extern QString user_id;
        model.setQuery(QString("select books.book_id AS ID,book_name AS Name,payment AS Payment,"
                               "s_number AS Number,writer AS Writer,Options from books,(select * from shopcart where user_id='%1')AS TEMP "
                               "where books.book_id = TEMP.book_id").arg(user_id));
        int i=0;

        QString bookid;
        while(1){
            bookid=model.data(model.index(i,0)).toString();
            if(bookid.isNull()){
                break;
            }
            else{
                i++;
                QSqlQuery query;
                extern QString user_id;
                query.exec(QString("select s_number from shopcart where user_id='%1' AND book_id='%2'").arg(user_id).arg(bookid));
                query.next();
                int number = query.value(0).toInt();
                query.exec(QString("update books set stock=stock+%1 where book_id='%2'").arg(number).arg(bookid));

                query.exec(QString("delete from shopcart where user_id='%1' AND book_id='%2'").arg(user_id).arg(bookid));
            }
        }
        model.setQuery(QString("select books.book_id AS ID,book_name AS Name,payment AS Payment,"
                               "s_number AS Number,writer AS Writer,Options from books,(select * from shopcart where user_id='%1')AS TEMP "
                               "where books.book_id = TEMP.book_id").arg(user_id));
        ui->cartView->setModel(&model);
    }
}

void MainWindow::on_rAllBtn_clicked()
{
    QString str = QString("select book_id AS ID,book_name AS Name,book_price AS Price,"
                          "book_press AS Press,writer AS Writer,Options from books where stock>0");
    model.setQuery(str);
    ui->booksView->setModel(&model);
    ButtonDelegate * m_buttonDelegate = new ButtonDelegate(str,ui->booksView,this);
    ui->booksView->setItemDelegateForColumn(5, m_buttonDelegate);
    ui->booksView->setColumnWidth(5, 200);
}

void MainWindow::on_rLiterBtn_clicked()
{
    QString str = QString("select book_id AS ID,book_name AS Name,book_price AS Price,"
                          "book_press AS Press,writer AS Writer,Options from books where stock>0 AND book_type='literature'");
    model.setQuery(str);
    ui->booksView->setModel(&model);
    ButtonDelegate * m_buttonDelegate = new ButtonDelegate(str,ui->booksView,this);
    ui->booksView->setItemDelegateForColumn(5, m_buttonDelegate);
    ui->booksView->setColumnWidth(5, 200);
}

void MainWindow::on_rSocialBtn_clicked()
{
    QString str = QString("select book_id AS ID,book_name AS Name,book_price AS Price,"
                          "book_press AS Press,writer AS Writer,Options from books where stock>0 AND book_type='Social Science'");
    model.setQuery(str);
    ui->booksView->setModel(&model);
    ButtonDelegate * m_buttonDelegate = new ButtonDelegate(str,ui->booksView,this);
    ui->booksView->setItemDelegateForColumn(5, m_buttonDelegate);
    ui->booksView->setColumnWidth(5, 200);
}

void MainWindow::on_rTextBtn_clicked()
{
    QString str = QString("select book_id AS ID,book_name AS Name,book_price AS Price,"
                          "book_press AS Press,writer AS Writer,Options from books where stock>0 AND book_type='textbook'");
    model.setQuery(str);
    ui->booksView->setModel(&model);
    ButtonDelegate * m_buttonDelegate = new ButtonDelegate(str,ui->booksView,this);
    ui->booksView->setItemDelegateForColumn(5, m_buttonDelegate);
    ui->booksView->setColumnWidth(5, 200);
}

void MainWindow::on_rttBtn_clicked()
{
    QString str = QString("select book_id AS ID,book_name AS Name,book_price AS Price,"
                      "book_press AS Press,writer AS Writer,Options from books where stock>0 AND book_type='ttcj'");
    model.setQuery(str);
    ui->booksView->setModel(&model);
    ButtonDelegate * m_buttonDelegate = new ButtonDelegate(str,ui->booksView,this);
    ui->booksView->setItemDelegateForColumn(5, m_buttonDelegate);
    ui->booksView->setColumnWidth(5, 200);
}

void MainWindow::on_rNovelBtn_clicked()
{
    QString str = QString("select book_id AS ID,book_name AS Name,book_price AS Price,"
                          "book_press AS Press,writer AS Writer,Options from books where stock>0 AND book_type='Novel'");
    model.setQuery(str);
    ui->booksView->setModel(&model);
    ButtonDelegate * m_buttonDelegate = new ButtonDelegate(str,ui->booksView,this);
    ui->booksView->setItemDelegateForColumn(5, m_buttonDelegate);
    ui->booksView->setColumnWidth(5, 200);
}

void MainWindow::on_rEcoBtn_clicked()
{
    QString str = QString("select book_id AS ID,book_name AS Name,book_price AS Price,"
                          "book_press AS Press,writer AS Writer,Options from books where stock>0 AND book_type='Economy management'");
    model.setQuery(str);
    ui->booksView->setModel(&model);
    ButtonDelegate * m_buttonDelegate = new ButtonDelegate(str,ui->booksView,this);
    ui->booksView->setItemDelegateForColumn(5, m_buttonDelegate);
    ui->booksView->setColumnWidth(5, 200);
}

void MainWindow::on_rTechBtn_clicked()
{
    QString str = QString("select book_id AS ID,book_name AS Name,book_price AS Price,"
                          "book_press AS Press,writer AS Writer,Options from books where stock>0 AND book_type='Technology'");
    model.setQuery(str);
    ui->booksView->setModel(&model);
    ButtonDelegate * m_buttonDelegate = new ButtonDelegate(str,ui->booksView,this);
    ui->booksView->setItemDelegateForColumn(5, m_buttonDelegate);
    ui->booksView->setColumnWidth(5, 200);
}

void MainWindow::on_rCSBtn_clicked()
{
    QString str = QString("select book_id AS ID,book_name AS Name,book_price AS Price,"
                          "book_press AS Press,writer AS Writer,Options from books where stock>0 AND book_type='Computer Science'");
    model.setQuery(str);
    ui->booksView->setModel(&model);
    ButtonDelegate * m_buttonDelegate = new ButtonDelegate(str,ui->booksView,this);
    ui->booksView->setItemDelegateForColumn(5, m_buttonDelegate);
    ui->booksView->setColumnWidth(5, 200);
}

void MainWindow::on_RecBtn_clicked()  //书籍推荐系统
{
    ui->stackedWidget->setCurrentIndex(5);
    QTime time;
    time.start();
    ui->mg1label->hide();
    ui->mg2label->show();
    ui->mg3label->hide();
    ui->mg4label->hide();
    ui->mg5label->hide();
    ui->mg6label->hide();
    ui->tuijianlabel->setPixmap(QPixmap("E:/SE_work/Qt_project/OnlibraryV1/tuijian.png"));
    ui->tuijianlabel->show();
    qApp->processEvents();

    extern QString user_id;
    QSqlQuery query;
    query.exec(QString("delete from recommend"));
    query.exec(QString("select count(*) from history where user_id='%1'").arg(user_id));
    query.next();
    int panduan = query.value(0).toInt();
    if(panduan>3){
        qDebug() << "TYPE: Customer recommend" << endl;
        // 一、根据用户操作记录进行推荐
        // 读取该用户的历史记录
        query.exec(QString("select book_id,l_score from history where user_id='%1'").arg(user_id));
        QString bid[MAXHC]; //bid数组用来存储用户操作过的book_id
        int ls[MAXHC];      //ls数组用来存储用户操作过图书的偏好等级，为整数1,2,3
        int i;
        for(i=0;i<MAXHC && query.next();i++){
            bid[i] = query.value(0).toString();
            ls[i] = query.value(1).toInt();
        }
        int m = i;          //m存储用户历史操作的记录数
        //读取其他用户的历史记录，得到各用户与当前用户偏好的绝对值距离
        query.exec(QString("select distinct user_id from history where user_id!='%1'").arg(user_id));
        QString uid[MAXUN];
        i=0;
        QString temp;
        while(query.next()){
            temp = query.value(0).toString();
            uid[i++] = temp.trimmed();          //uid存储其他用户名
        }
        int n = i;          //n为其他用户个数

        int rs[MAXUN];//rc存储每个用户与当前用户的偏好距离
        int d,t;
        for(i=0; i<n; i++){ //  对其他用户遍历
            d = 0;
            for(int a=0; a<m; a++){ // 对用户有记录的书籍遍历
                query.exec(QString("select l_score from history where user_id='%1' AND book_id='%2'").arg(uid[i]).arg(bid[a]));
                query.next();
                t = query.value(0).toInt();
                //qDebug() << "t:" << t << endl;
                d += abs(t-ls[a]);
            }
            rs[i] = d;
        }
        // 得到了各用户与当前用户的偏好绝对值距离

        QString ruid[MAXUN];   //ruid存储可用于为当前用户推荐的用户名
        int j=0;
        for(i=0;i<n;i++){
            if(rs[i]<1.5*m){  //此时认为该用户与当前用户属于同一偏好群体
               ruid[j++] = uid[i];
            }
        }
        int rn = j; //rn为可用于为当前用户推荐的用户数
        //qDebug() << rn << endl;
        // 基于有效用户名找到推荐书籍信息
        QSqlQuery qt;
        for(i=0;i<rn;i++){
            query.exec(QString("select book_id from orders where user_id='%1'").arg(ruid[i]));
            while(query.next()){
                temp = query.value(0).toString();
                //应当删除当前用户已经购买或加入购物车的书籍
                qt.exec(QString("select book_id from history where user_id='%1' AND "
                                "l_score>1 AND book_id='%2'").arg(user_id).arg(temp));
                //qt的查询结果是当前用户是否已经将该书购买或加入购物车
                if(!qt.next()){
                    qt.exec(QString("insert into recommend values('%1',%2)").arg(temp).arg(rs[i]));
                }
            }
        }
        // 二、根据用户评分进行推荐
        query.exec(QString("select book_id,b_score from scores where user_id='%1'").arg(user_id));
        QString bid2[MAXHC]; //bid数组用来存储用户评分过的book_id
        int ls2[MAXHC];      //ls数组用来存储用户对各图书的评分，为整数1,2,3,..,10
        for(i=0;i<MAXHC && query.next();i++){
            bid2[i] = query.value(0).toString();
            ls2[i] = query.value(1).toInt();
        }
        int m2 = i;          //m2存储用户评分的记录数
        //qDebug() << "Commences:" << m2 << endl;
        //读取其他用户的历史评分，得到各用户与当前用户偏好的绝对值距离
        query.exec(QString("select distinct user_id from scores where user_id!='%1'").arg(user_id));
        QString uid2[MAXUN];
        i=0;
        while(query.next()){
            temp = query.value(0).toString();
            uid2[i++] = temp.trimmed();          //uid2存储其他用户名
        }
        int n2 = i;          //n为其他用户个数

        int rs2[MAXUN];//rc存储每个用户与当前用户的偏好距离
        for(i=0; i<n2; i++){ //  对其他用户遍历
            d = 0;
            for(int a=0; a<m2; a++){ // 对用户有记录的书籍遍历
                query.exec(QString("select b_score from scores where user_id='%1' AND book_id='%2'").arg(uid2[i]).arg(bid2[a]));
                query.next();
                t = query.value(0).toInt();
                //qDebug() << "t:" << t << endl;
                d += abs(t-ls2[a]);
            }
            rs2[i] = d;
        }
        // 得到了各用户与当前用户的偏好绝对值距离

        QString ruid2[MAXUN];   //ruid存储可用于为当前用户推荐的用户名
        j=0;
        for(i=0;i<n2;i++){
            if(rs2[i]<0.5*m2){  //此时认为该用户与当前用户属于同一偏好群体
               ruid2[j++] = uid2[i];
            }
        }
        rn = j; //rn为可用于为当前用户推荐的用户数
        // 基于有效用户名找到推荐书籍信息
        for(i=0;i<rn;i++){
            query.exec(QString("select book_id from scores where user_id='%1' AND b_score>8").arg(ruid2[i]));
            while(query.next()){
                temp = query.value(0).toString();
                //应当删除当前用户已经购买或加入购物车的书籍
                qt.exec(QString("select book_id from history where user_id='%1' AND "
                                "l_score>1 AND book_id='%2'").arg(user_id).arg(temp));
                //qt的查询结果是当前用户是否已经将该书购买或加入购物车
                if(!qt.next()){
                    qt.exec(QString("insert into recommend values('%1',%2)").arg(temp).arg(rs2[i]));
                }
            }
        }

        // 推荐表已建立完成
        ui->TypeINtroLabel->setText("System Type : Customized Recommand.");
        model.setQuery(QString("select books.book_id AS ID,book_name AS Name,book_price AS Price,"
                               "book_press AS Press,writer AS Writer,Options from books,recommend "
                               "where books.book_id = recommend.book_id order by r_score ASC"));


    }
    else{
        qDebug() << "TYPE: Genral recommend." << endl;
        query.exec(QString("select book_id,SUM(number) from orders group by book_id order by SUM(number) DESC"));
        QString temp2;  int sc2;
        QSqlQuery qt2;
        while(query.next()){
            temp2 = query.value(0).toString();
            sc2 = query.value(1).toInt();
            qt2.exec(QString("select book_id from history where user_id='%1' AND "
                            "l_score>1 AND book_id='%2'").arg(user_id).arg(temp2));
            //qt的查询结果是当前用户是否已经将该书购买或加入购物车
            if(!qt2.next()){
                qt2.exec(QString("insert into recommend values('%1',%2)").arg(temp2).arg(sc2));
            }
        }

        // 推荐表已建立完成
        ui->TypeINtroLabel->setText("System Type : General Recommand");
        model.setQuery(QString("select books.book_id AS ID,book_name AS Name,book_price AS Price,"
                               "book_press AS Press,writer AS Writer,Options from books,recommend "
                               "where books.book_id = recommend.book_id order by r_score DESC"));

    }
    ui->TypeINtroLabel->hide();
    qDebug() << "Object time:" << time.elapsed() << endl;
    while(time.elapsed()<600){
        QThread::usleep(10000);
    }
    ui->tuijianlabel->hide();
    qApp->processEvents();

    ui->TypeINtroLabel->show();
    ui->recView->setModel(&model);
    ui->recView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->recView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->recView->verticalHeader()->setVisible(false);
    ui->recView->setAlternatingRowColors(true);
    ui->recView->setFrameShape(QFrame::NoFrame);

    ui->recView->setStyleSheet("QTableView{background-color: rgb(211,211,211);"
                                 "alternate-background-color: rgb(255,255,255);}");
    ui->recView->horizontalHeader()->setStretchLastSection(true);
    ui->recView->setShowGrid(false);
    ui->recView->resizeColumnsToContents();
    ui->recView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    QString str = QString("select books.book_id AS ID,book_name AS Name,book_price AS Price,"
                          "book_press AS Press,writer AS Writer,Options from books,recommend "
                          "where books.book_id = recommend.book_id order by r_score ASC");
    ButtonDelegate * rm_buttonDelegate = new ButtonDelegate(str,ui->recView,this);
    ui->recView->setItemDelegateForColumn(5, rm_buttonDelegate);

    ui->recView->setColumnWidth(0, 50);
    ui->recView->setColumnWidth(1, 150);
    ui->recView->setColumnWidth(2, 75);
    ui->recView->setColumnWidth(3, 200);
    ui->recView->setColumnWidth(4, 125);
    ui->recView->setColumnWidth(5, 125);

}

void MainWindow::on_pushButton_3_clicked()
{
    this->hide();
    loginDlg login;
    login.setWindowTitle(QString("登录"));
    if(login.exec()==QDialog::Accepted){
        //刷新操作
        ui->mg1label->show();
        ui->mg2label->hide();
        ui->mg3label->hide();
        ui->mg4label->hide();
        ui->mg5label->hide();
        ui->mg6label->hide();
        extern QString user_id;
        ui->uidlabel->setText(user_id);
        ui->stackedWidget->setCurrentIndex(0);
        this->show();
        QSqlQuery query;
        query.exec(QString("select msg from users where user_id='%1'").arg(user_id));
        query.next();
        int oid = query.value(0).toInt();
        if(oid){
            QMessageBox::about(NULL,"New Message",QString("\tYour order( No.%1 ) has been deleted!\t\t").arg(oid));
            query.exec(QString("update users set msg=0 where user_id='%1'").arg(user_id));
        }
        model.setQuery(QString("select book_id AS ID,book_name AS Name,book_price AS Price,"
                               "book_press AS Press,writer AS Writer,Options from books where stock>0"));
        ui->booksView->setModel(&model);
    }
}

void MainWindow::on_stackedWidget_currentChanged(int arg1)
{

}
