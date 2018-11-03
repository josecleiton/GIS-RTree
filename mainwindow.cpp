#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Filhos de Jorge");
    QIcon icone("../GIS/ui/jorge.png");
    this->setWindowIcon(icone);
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

void MainWindow::on_buscar_clicked(){
    if(root.GetPtr() != nullptr){
        SearchWindow searchwindow;
        searchwindow.setModal(true);
        searchwindow.exec();
    }
    else{
        QMessageBox mbox;
        mbox.critical(nullptr, "Erro", "Primeiro insira elementos no banco de dados.");
    }
}

void MainWindow::on_cancel_clicked(){
    close();
}
