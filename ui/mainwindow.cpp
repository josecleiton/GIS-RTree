#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Filhos de Jorge");
    this->setWindowIcon(QIcon(ICON));
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
    else
        EmptyDS();
}

void MainWindow::on_info_clicked()
{
    QMessageBox w;
    if(root.GetPtr() != nullptr){
        string nos = to_string(root.GetCount());
        string nivel = root.GetNivel();
        string folhas = to_string(root.GetRegistros());
        w.information(nullptr, "ED", "Estrutura utilizada: Original R-Tree\nAltura: "+QString::fromStdString(nivel)+"\nNúmero de nós: "+QString::fromStdString(nos)+"\nNúmero de registros: "+QString::fromStdString(folhas)+"\n");
    }
    else
        EmptyDS();
}

void MainWindow::EmptyDS(){
    QMessageBox mbox;
    mbox.critical(nullptr, "Erro", "Primeiro insira elementos na estrutura de dados.");
}


void MainWindow::on_cancel_clicked(){
    close();
}

void MainWindow::on_creditos_clicked()
{
    QMessageBox QMB;
    QString feito("Feito por: Danilo Nascimento, José Cleiton Borges, Matheus Souza.\n");
    QString orientador("Orientador: Jorge Sampaio Farias\n");
    QString universidade("UNEB - Universidade do Estado da Bahia\n");
    QString curso("Sistemas de Informação - Estruturas de Dados II\n");
    QString ano("Período: 2018.2\n");
    QMB.information(nullptr, "Créditos", feito+universidade+curso+orientador+ano);
}
