#include "insertwindow.hpp"
#include "ui_insertwindow.h"


InsertWindow::InsertWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InsertWindow)
{
    ui->setupUi(this);
}

InsertWindow::~InsertWindow()
{
    delete ui;
}

void InsertWindow::on_submit_clicked()
{
    vector<string> resultado;
    resultado.push_back(ui->numVert->text().toStdString());
    resultado.push_back(ui->tipoForma->text().toStdString());
    resultado.push_back(ui->ID->text().toStdString());
    fstream saida(INSERT_FORM, ios::out);
    if(saida.is_open()){
        for(auto it: resultado)
            saida << it << endl;
        saida.close();
    }
    close();
}
