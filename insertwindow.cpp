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
