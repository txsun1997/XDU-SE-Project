#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_recommendBtn_clicked();

    void on_orderBtn_clicked();

    void on_collectionBtn_clicked();

    void on_shopcarBtn_clicked();

    void on_booksView_activated(const QModelIndex &index);

//private:
    void on_collectionBtn_2_clicked();

    void on_searchBtn_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_InfoBtn_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_chargeBtn_clicked();

    void on_clearBtn_clicked();

    void on_rAllBtn_clicked();

    void on_rLiterBtn_clicked();

    void on_rSocialBtn_clicked();

    void on_rTextBtn_clicked();

    void on_rttBtn_clicked();

    void on_rNovelBtn_clicked();

    void on_rEcoBtn_clicked();

    void on_rTechBtn_clicked();

    void on_rCSBtn_clicked();

    void on_RecBtn_clicked();

    void on_pushButton_3_clicked();

    void on_stackedWidget_currentChanged(int arg1);

public:
    Ui::MainWindow *ui;
    QSqlQueryModel model;
};

#endif // MAINWINDOW_H
