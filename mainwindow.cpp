#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    h=0.1;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./DB.db");
    if (db.open()) {
        qDebug()<<"DB open!";
    } else {
        qDebug()<< "DB open ERROR";
    }

    model = new QSqlTableModel(this, db);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setTable("POINT");
    model->select();
    ui->tableView->setModel(model);


    secondModel = new QSqlTableModel(this, db);
    secondModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    secondModel->setTable("SECONDPOINT");
    secondModel->select();
    ui->tableView_2->setModel(secondModel);

    colorModel = new QSqlTableModel(this, db);
    colorModel ->setEditStrategy(QSqlTableModel::OnManualSubmit);
    colorModel->setTable("COLOR");
    colorModel->select();
    ui->tableView_Color->setModel(colorModel);

}

MainWindow::~MainWindow()
{
    if (ui->tableView->model() != nullptr) {
        int countRow = ui->tableView->model()->rowCount();
        for (int i = 0; i <= countRow; i++) {
            model->removeRow(i);
        }
    }
    model->submitAll();
    delete ui;
}


void MainWindow::on_btn_addRow_clicked()
{
    model->insertRow(model->rowCount());
}


void MainWindow::on_btn_Saved_clicked()
{
    QSqlQuery query = QSqlQuery(db);

    //Чистим таблицу для сохранения новых данных
    if (ui->tableView_2->model() != nullptr) {
        int countRowSecond = ui->tableView_2->model()->rowCount();
        for (int i = 0; i <= countRowSecond; i++) {
            secondModel->removeRow(i);
    }
    }
    //Сщхраняем изменения в таблицах
    model->submitAll();
    secondModel->submitAll();
    //Чистим массивы
    xPoint.clear();
    yPoint.clear();
    //Заполняем вектора координаами из таблицы
    int countRow = ui->tableView->model()->rowCount();
    for (int i = 0; i < countRow; i++) {

        xPoint.push_back(model->record(i).value(0).toInt());
        yPoint.push_back(model->record(i).value(1).toInt());
    }
    //Создаем SQL запрос на вставку строк с даными из первой таблицы во вторую
    for (int i =0; i <countRow; i++) {
    QString first = "INSERT INTO SECONDPOINT(x,y) VALUES(%1 , %2)";
    QString x = QString::number(xPoint[i], 'g', 6);
    QString y = QString::number(yPoint[i],'g', 6);
    QString second = first.arg(x).arg(y);
    query.exec(second);
    qDebug()<< second;
    }
    secondModel->submitAll();
    //Чистим массивы
    xPoint.clear();
    yPoint.clear();

    //Чистим таблицу COLOR
    if (ui->tableView_Color->model() != nullptr) {
        int countRowColor = ui->tableView_Color->model()->rowCount();
        for (int i = 0; i <= countRowColor; i++) {
            colorModel->removeRow(i);
        }
   }
    colorModel->submitAll();



    //Сохранаем цвет графика и название набора
    QString cl = color.name();
    QString firstColor = "INSERT INTO COLOR(color, name) VALUES('%1', '%2')";
    QString secondColor = firstColor.arg(cl).arg(name);
    qDebug()<<secondColor;
    query.exec(secondColor);
}


void MainWindow::on_btn_insertRow_clicked()
{
    int deleteRow = ui->tableView->currentIndex().row();
    if (deleteRow >=0) {
        model->removeRow(deleteRow);
    }
}


void MainWindow::on_btn_Bild_clicked()
{



    model->submitAll();

    //Чистим массивы
    xPoint.clear();
    yPoint.clear();

    //Заполняем вектора координаами из таблицы
    int countRow = ui->tableView->model()->rowCount();
    for (int i = 0; i < countRow; i++) {

        xPoint.push_back(model->record(i).value(0).toDouble());
        yPoint.push_back(model->record(i).value(1).toDouble());
    }
    xPoint.shrink_to_fit();
    yPoint.shrink_to_fit();



    //Устанаиваем область видимости виджета
    ui->widget->xAxis->setRange(*std::min_element(xPoint.begin(),xPoint.end())-1 , *std::max_element(xPoint.begin(), xPoint.end()) +  1);
    ui->widget->yAxis->setRange(*std::min_element(yPoint.begin(),yPoint.end())-1 , *std::max_element(yPoint.begin(), yPoint.end()) +  1);

    //Отрисовываем график
    ui->widget->clearGraphs();
    ui->widget->addGraph();
    ui->widget->graph(0)->addData(xPoint,yPoint);
    ui->widget->graph(0)->setPen(color);
    ui->widget->replot();



    for (int i = 0; i< xPoint.size(); i++) {
        qDebug() << xPoint[i] << "  " <<yPoint[i];
    }

    //Чистим массивы
    xPoint.clear();
    yPoint.clear();

}


void MainWindow::on_btn_Delete_clicked()
{
    //Удаляем данные из первой таблицы
    if (ui->tableView->model() != nullptr) {
    int deleteRow = ui->tableView->model()->rowCount();
    for (int i = 0; i <= deleteRow; i++) {
        model->removeRow(i);
        }
    model->submitAll();
    }
    //Удаляем график
    ui->widget->removeGraph(0);
    //Удаляем название набора
    ui->lineEdit->setText("");
    //Удаляем цвет
    QString style = QString::fromUtf8("background-color: white;");
    ui->pushButton->setStyleSheet(style);

}





void MainWindow::on_pushButton_clicked()
{
    color = QColorDialog::getColor(Qt::white, this, "Выбор цвета графика");
    if (color.isValid()) {
        if (ui->widget->graph() != nullptr) {
        ui->widget->graph(0)->setPen(color);
    }
        QString style = QString::fromUtf8("background-color: %1;");
        QString superColor = style.arg(color.name());
        ui->pushButton->setStyleSheet(superColor);
    }
        ui->widget->replot();   
}


void MainWindow::on_btn_Download_clicked()
{
    QSqlQuery query = QSqlQuery(db);
    //Чистим таблицу для загрузки новых данных
    if (ui->tableView->model() != nullptr) {
        int countRow = ui->tableView->model()->rowCount();
        for (int i = 0; i <= countRow; i++) {
            model->removeRow(i);
        }
    }
    model->submitAll();
    //Чистим массивы
    xPoint.clear();
    yPoint.clear();
    //Заполняем массивы из второй сохраненной таблицы
    int countRowSecond = ui->tableView_2->model()->rowCount();
    for (int i = 0; i < countRowSecond; i++) {

        xPoint.push_back(secondModel->record(i).value(0).toInt());
        yPoint.push_back(secondModel->record(i).value(1).toInt());
    }
    //Создаем SQL запрос на вставку строк с даными из второй таблцы в первую
    for (int i =0; i <countRowSecond; i++) {
    QString first = "INSERT INTO POINT(x,y) VALUES(%1 , %2)";
    QString x = QString::number(xPoint[i], 'g', 6);
    QString y = QString::number(yPoint[i],'g', 6);
    QString second = first.arg(x).arg(y);
    query.exec(second);
    }
    model->submitAll();
    model->select();
    //Чистим массивы
    xPoint.clear();
    yPoint.clear();

    //Выгружаем сохраненный цвет из БД
    QString superColor = colorModel->record(0).value(0).toString();
    color.setNamedColor(superColor);
    //Устанавливаем цвет кнопки выбра цвета
    QString style = QString::fromUtf8("background-color: %1;");
    QString superColor1 = style.arg(color.name());
    ui->pushButton->setStyleSheet(superColor1);
    //Выгружаем и устанавливаем название набора
    QString superName = colorModel->record(0).value(1).toString();
    ui->lineEdit->setText(superName);

}


void MainWindow::on_lineEdit_editingFinished()
{
    name = ui->lineEdit->text();

}

