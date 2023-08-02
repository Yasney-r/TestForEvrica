#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QVector>
#include <QSqlRecord>
#include <QColorDialog>
#include <cstring>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_btn_addRow_clicked();

    void on_btn_Saved_clicked();

    void on_btn_insertRow_clicked();

    void on_btn_Bild_clicked();

    void on_btn_Delete_clicked();



    void on_pushButton_clicked();

    void on_btn_Download_clicked();

    void on_lineEdit_editingFinished();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QSqlTableModel *model;
    QSqlTableModel *secondModel;
    QSqlTableModel *colorModel;
    QString name;
    QVector<double> xPoint, yPoint;
    QColor color;
    int xBegin, xEnd, N;
    double h;
};
#endif // MAINWINDOW_H
