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
    void on_bmBtn_clicked();

    void on_omBtn_clicked();

    void on_umBtn_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_addBtn_clicked();

public:
    Ui::MainWindow *ui;
    QSqlQueryModel model;
};

#endif // MAINWINDOW_H
