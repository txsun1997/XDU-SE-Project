#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlQueryModel>
#include <QTextCodec>
#include <QMessageBox>
#include <QHeaderView>
#include <QStandardItemModel>
#include "buttondelegate.h"
#include "orderBtndelegate.h"
#include "newbookdlg.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon("manager.ico"));
    ui->stackedWidget->setCurrentIndex(0);
    model.setQuery(QString("select book_id AS ID,book_name AS Name,book_price AS Price,"
                           "book_press AS Press,writer AS Writer,Options from books"));
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
    ButtonDelegate * m_buttonDelegate = new ButtonDelegate(ui->booksView,this);
    ui->booksView->setItemDelegateForColumn(5, m_buttonDelegate);
    ui->booksView->setColumnWidth(5, 200);
    ui->booksView->setColumnWidth(0, 50);
    ui->booksView->setColumnWidth(1, 175);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_bmBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    model.setQuery(QString("select book_id AS ID,book_name AS Name,book_price AS Price,"
                           "book_press AS Press,writer AS Writer,Options from books"));
    ui->booksView->setModel(&model);
}

void MainWindow::on_omBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    //model.setQuery(QString("select order_id AS ID,book_name AS Name,user_id AS Buyer,book_price AS Price,order_time,order_state AS State,Options "
    //                       "from books,(select * from orders)AS temp "
    //                       "where books.book_id=temp.book_id"));

    model.setQuery(QString("select order_id AS ID,book_name AS Name,user_id AS Buyer,total_payment AS Payment,number AS Num,order_time,order_state AS State,Options "
                           "from books,(select * from orders)AS temp "
                           "where books.book_id=temp.book_id"));

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
    ui->ordersView->setItemDelegateForColumn(7, o_buttonDelegate);

    ui->ordersView->setColumnWidth(5, 150);
    ui->ordersView->setColumnWidth(4, 50);
    ui->ordersView->setColumnWidth(7, 125);
}

void MainWindow::on_umBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    model.setQuery(QString("select user_id AS ID,user_name AS Name,user_type AS Type,user_tel AS Tel,"
                           "user_address AS Address from users"));

    ui->usersView->setModel(&model);
    ui->usersView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->usersView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->usersView->verticalHeader()->setVisible(false);
    ui->usersView->setAlternatingRowColors(true);
    ui->usersView->setFrameShape(QFrame::NoFrame);

    ui->usersView->setStyleSheet("QTableView{background-color: rgb(211,211,211);"
                                 "alternate-background-color: rgb(255,255,255);}");
    ui->usersView->horizontalHeader()->setStretchLastSection(true);
    ui->usersView->setShowGrid(false);
    ui->usersView->resizeColumnsToContents();
    ui->usersView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    //ui->ordersView->setColumnWidth(4, 300);
}

void MainWindow::on_pushButton_clicked()
{
    QString txt = ui->lineEdit->text();
    //qDebug() << txt << endl;
    model.setQuery(QString("select book_id AS ID,book_name AS Name,book_price AS Price,"
                           "book_press AS Press,writer AS Writer,Options from books "
                           "where book_name='%1' or book_id='%2' or writer='%3'").arg(txt).arg(txt).arg(txt));
    ui->booksView->setModel(&model);
}

void MainWindow::on_pushButton_2_clicked()
{
    QString txt = ui->lineEdit_2->text();
    //qDebug() << txt << endl;
    int oid = txt.toInt();
    model.setQuery(QString("select order_id AS ID,book_name AS Name,user_id AS Buyer,book_price AS Price,order_time,order_state AS State,Options "
                           "from books,(select * from orders where user_id='%1' or order_id='%2') AS temp "
                           "where books.book_id=temp.book_id").arg(txt).arg(oid));
    ui->ordersView->setModel(&model);
}

void MainWindow::on_pushButton_3_clicked()
{
    QString txt = ui->lineEdit_3->text();
    //qDebug() << txt << endl;
    model.setQuery(QString("select user_id AS ID,user_name AS Name,user_type AS Type,user_tel AS Tel,"
                           "user_address AS Address from users where user_id='%1'").arg(txt));

    ui->ordersView->setModel(&model);
}

void MainWindow::on_addBtn_clicked()
{
    NewbookDlg newbook;
    newbook.setWindowTitle(tr("添加图书信息"));
    newbook.exec();
    model.setQuery(QString("select book_id AS ID,book_name AS Name,book_price AS Price,"
                           "book_press AS Press,writer AS Writer,Options from books"));
    ui->booksView->setModel(&model);
}
