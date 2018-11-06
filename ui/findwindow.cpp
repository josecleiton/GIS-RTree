#include "findwindow.hpp"
#include "ui_findwindow.h"

FindWindow::FindWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Forma encontrada!");
    this->remove = false;
}

FindWindow::~FindWindow()
{
    delete ui;
}

void FindWindow::on_remove_clicked()
{
    QMessageBox ask;
    int escolha = ask.warning(nullptr, "Tem certeza?", "Sua decisão afetará significativamente o banco de dados.", QMessageBox::Ok|QMessageBox::Cancel, QMessageBox::Cancel);
    if(escolha == QMessageBox::Ok){
        this->remove = true;
        QMessageBox QMB;
        QMB.information(nullptr, "Sucesso", "Forma removida do banco de dados.");
        this->close();
    }
}

bool FindWindow::setReg(DiskAPI::Registro* R){
    if(R == nullptr or R->tam == 0){
        QMessageBox A;
        A.critical(nullptr, "Registro vazio.", "Um registro vazio não pode ser mostrado.");
        this->close();
        return false;
    }
    if(!count)
        this->ui->ra->setReg(R);
    else
        this->ui->ra->setReg2(R);
    count++;
    return true;
}

bool FindWindow::GetRemove(){
    return this->remove;
}
