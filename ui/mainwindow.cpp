#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Filhos de Jorge");
    QIcon icone("jorge.png");
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

void MainWindow::on_info_clicked()
{
    QMessageBox w;
    string nos = to_string(root.GetCount());
    string nivel = to_string(root.GetNivel());
    string folhas = to_string(root.GetRegistros());
    w.information(nullptr, "ED", "Estrutura utilizada: Original R-Tree\nAltura: "+QString::fromStdString(nivel)+"\nNúmero de nós: "+QString::fromStdString(nos)+"\nNúmero de registros: "+QString::fromStdString(folhas)+"\n");
}
