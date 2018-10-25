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
    /*
     *     InsertWindow insertwindow;
        insertwindow.setModal(true);
        insertwindow.exec();
        */
    RectangleSearchWindow recwindow;
    recwindow.setModal(true);
    recwindow.exec();
}
