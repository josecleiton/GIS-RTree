#include "findwindow.hpp"
#include "ui_findwindow.h"

FindWindow::FindWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Forma encontrada!");
}

FindWindow::~FindWindow()
{
    delete ui;
}

void FindWindow::on_remove_clicked()
{
    this->remove = true;
    QMessageBox QMB;
    QMB.information(nullptr, "Sucesso", "Forma removida do banco de dados.");
}

void FindWindow::setReg(DiskAPI::Registro* R){
    this->ui->ra->setReg(R);
}

bool FindWindow::GetRemove(){
    return this->remove;
}
