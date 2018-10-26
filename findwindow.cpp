#include "findwindow.hpp"
#include "ui_findwindow.h"

FindWindow::FindWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindWindow)
{
    ui->setupUi(this);
}

FindWindow::~FindWindow()
{
    delete ui;
}

void FindWindow::on_remove_clicked()
{
    this->remove = true;
    QMessageBox QMB;
    QMB.setText("Forma removida do BD.");
    QMB.exec();
}

void FindWindow::setReg(DiskAPI::Registro* R){
    this->REG = R;
}

bool FindWindow::GetRemove(){
    return this->remove;
}
