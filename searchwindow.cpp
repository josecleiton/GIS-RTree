#include "searchwindow.hpp"
#include "ui_searchwindow.h"

SearchWindow::SearchWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchWindow)
{
    ui->setupUi(this);
}

SearchWindow::~SearchWindow()
{
    delete ui;
}

void SearchWindow::on_retangulo_clicked(){
    interseccaoBool = false;
    RectangleSearchWindow recwindow;
    recwindow.setModal(true);
    recwindow.exec();
    recwindow.close();
}

void SearchWindow::on_interseccao_clicked(){
    interseccaoBool = true;
    RectangleSearchWindow RSW;
    DiskAPI::Registro* R[2];
    for(unsigned j=0; j<2; j++){
        RSW.setModal(true);
        RSW.exec();
        R[j] = RSW.GetRegistro();
        RSW.close();
    }

    for(unsigned j=0; j<2; j++)
        delete R[j];
}
