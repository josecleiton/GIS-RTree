#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "insertwindow.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_inserir_clicked()
{
    InsertWindow insertwindow;
    insertwindow.setModal(true);
    insertwindow.exec();
}
